#include "sfz.h"
namespace sfz {

SFZParser::SFZParser()
{
}

SFZParser::~SFZParser()
{
}

int SFZParser::readsfz(std::string path)
{
	FILE* fp;
	uint32_t len;

	if (! (fp = fopen(path.c_str(), "rb"))) {
		return -1;
	}

	fseek(fp, 0, SEEK_END);
	len = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	char sfz[len];

	fread(sfz, 1, len, fp);
	fclose(fp);
	
	SFZParser::read(sfz, len);
	return 0;
}

void SFZParser::read(const char* text, unsigned int length)
{
	const char* p = text;
	const char* end = text + length;
	char c;

	SFZRegion curGroup(&instrument);
	SFZRegion curRegion(&instrument);
	SFZRegion *buildingRegion = NULL;
	bool inControl = false;
	std::string defaultPath;

	while (p < end) {
		// We're at the start of a line; skip any whitespace.
		while (p < end) {
			c = *p;
			if (c != ' ' && c != '\t')
				break;
			p += 1;
			}
		if (p >= end)
			break;

		// Check if it's a comment line.
		if (c == '/' && *(p+1) == '/') {
			// Skip to end of line.
			while (p < end) {
				c = *++p;
				if (c == '\n' || c == '\r')
					break;
				}
			p = handleLineEnd(p);
			continue;
			}

		// Check if it's a blank line.
		if (c == '\r' || c == '\n') {
			p = handleLineEnd(p);
			continue;
			}

		// Handle elements on the line.
		while (p < end) {
			c = *p;

			// Tag.
			if (c == '<') {
				p += 1;
				const char* tagStart = p;
				while (p < end) {
					c = *p++;
					if (c == '\n' || c == '\r') {
						error(std::string("Unterminated tag"));
						goto fatalError;
						}
					else if (c == '>')
						break;
					}
				if (p >= end) {
					error(std::string("Unterminated tag"));
					goto fatalError;
					}
				StringSlice tag(tagStart, p - 1);
				if (tag == "region") {
					if (buildingRegion && buildingRegion == &curRegion)
						finishRegion(&curRegion);
					curRegion = curGroup;
					buildingRegion = &curRegion;
					inControl = false;
					}
				else if (tag == "group") {
					if (buildingRegion && buildingRegion == &curRegion)
						finishRegion(&curRegion);
					curGroup.clear();
					buildingRegion = &curGroup;
					inControl = false;
					}
				else if (tag == "control") {
					if (buildingRegion && buildingRegion == &curRegion)
						finishRegion(&curRegion);
					curGroup.clear();
					buildingRegion = NULL;
					inControl = true;
					}
				else
					error(std::string("Illegal tag"));
				}

			// Comment.
			else if (c == '/' && *(p+1) == '/') {
				// Skip to end of line.
				while (p < end) {
					c = *p;
					if (c == '\r' || c == '\n')
						break;
					p += 1;
					}
				}

			// Parameter.
			else {
				// Get the parameter name.
				const char* parameterStart = p;
				while (p < end) {
					c = *p++;
					if (c == '=' || c == ' ' || c == '\t' || c == '\r' || c == '\n')
						break;
					}
				if (p >= end || c != '=') {
					error(std::string("Malformed parameter"));
					goto nextElement;
					}
				StringSlice opcode(parameterStart, p - 1);
				if (inControl) {
					if (opcode == "default_path")
						p = readPathInto(&defaultPath, p, end);
					else {
						const char* valueStart = p;
						while (p < end) {
							c = *p;
							if (c == ' ' || c == '\t' || c == '\n' || c == '\r')
								break;
							p++;
						}
						std::string value(valueStart, p - valueStart);
						std::string fauxOpcode =
							std::string(opcode.start, opcode.length()) + " (in <control>)";
						printf("Unsupported: %s\n", fauxOpcode.c_str());
					}
				}
				else if (opcode == "sample") {
					std::string path;
					p = readPathInto(&path, p, end);
					if (!path.empty()) {
						printf("`%s'\n", path.c_str());
						if (buildingRegion)
							buildingRegion->sample = path;
						else
							error("Adding sample outside a group or region");
						}
					else
						error(std::string("Empty sample path"));
					}
				else {
					const char* valueStart = p;
					while (p < end) {
						c = *p;
						if (c == ' ' || c == '\t' || c == '\n' || c == '\r')
							break;
						p++;
						}
					std::string value(valueStart, p - valueStart);
					if (buildingRegion == NULL)
						error("Setting a parameter outside a region or group");
					else if (opcode == "lokey")
						buildingRegion->lokey = keyValue(value);
					else if (opcode == "hikey")
						buildingRegion->hikey = keyValue(value);
					else if (opcode == "key") {
						buildingRegion->hikey =
						buildingRegion->lokey =
						buildingRegion->pitch_keycenter =
							keyValue(value);
						}
					else if (opcode == "lovel")
						buildingRegion->lovel = atoi(value.c_str());
					else if (opcode == "hivel")
						buildingRegion->hivel = atoi(value.c_str());
					else if (opcode == "trigger")
						buildingRegion->trigger = (SFZRegion::Trigger) triggerValue(value);
					else if (opcode == "group")
						buildingRegion->group = (unsigned long) atol(value.c_str());
					else if (opcode == "off_by")
						buildingRegion->off_by = (unsigned long) atol(value.c_str());
					else if (opcode == "offset")
						buildingRegion->offset = (unsigned long) atol(value.c_str());
					else if (opcode == "end") {
						int64_t end = (unsigned long) atol(value.c_str());
						if (end < 0)
							buildingRegion->negative_end = true;
						else
							buildingRegion->end = end;
						}
					else if (opcode == "loop_mode") {
						bool modeIsSupported =
							value == "no_loop" ||
							value == "one_shot" ||
							value == "loop_continuous";
						if (modeIsSupported)
							buildingRegion->loop_mode = (SFZRegion::LoopMode) loopModeValue(value);
						else {
							std::string fauxOpcode =
								std::string(opcode.start, opcode.length()) + "=" + value;
							printf("Unsupported: %s\n", fauxOpcode.c_str());
						}
					}
					else if (opcode == "loop_start")
						buildingRegion->loop_start = (unsigned long) atol(value.c_str());
					else if (opcode == "loop_end")
						buildingRegion->loop_end = (unsigned long) atol(value.c_str());
					else if (opcode == "transpose")
						buildingRegion->transpose = atoi(value.c_str());
					else if (opcode == "tune")
						buildingRegion->tune = atoi(value.c_str());
					else if (opcode == "pitch_keycenter")
						buildingRegion->pitch_keycenter = keyValue(value);
					else if (opcode == "pitch_keytrack")
						buildingRegion->pitch_keytrack = atoi(value.c_str());
					else if (opcode == "bend_up")
						buildingRegion->bend_up = atoi(value.c_str());
					else if (opcode == "bend_down")
						buildingRegion->bend_down = atoi(value.c_str());
					else if (opcode == "volume")
						buildingRegion->volume = atof(value.c_str());
					else if (opcode == "pan")
						buildingRegion->pan = atof(value.c_str());
					else if (opcode == "amp_veltrack")
						buildingRegion->amp_veltrack = atof(value.c_str());
					else if (opcode == "default_path")
						error(std::string("\"default_path\" outside of <control> tag"));
					else
						error(std::string("Unsupported"));
					}
				}

			// Skip to next element.
nextElement:
			c = 0;
			while (p < end) {
				c = *p;
				if (c != ' ' && c != '\t')
					break;
				p += 1;
				}
			if (c == '\r' || c == '\n') {
				p = handleLineEnd(p);
				break;
				}
			}
		}

fatalError:
	if (buildingRegion && buildingRegion == &curRegion)
		finishRegion(buildingRegion);
}

const char* SFZParser::handleLineEnd(const char* p)
{
	// Check for DOS-style line ending.
	char lineEndChar = *p++;
	if (lineEndChar == '\r' && *p == '\n')
		p += 1;
	line += 1;
	return p;
}

const char* SFZParser::readPathInto(std::string* pathOut, const char* pIn, const char* endIn)
{
	// Paths are kind of funny to parse because they can contain whitespace.
	const char* p = pIn;
	const char* end = endIn;
	const char* pathStart = p;
	const char* potentialEnd = NULL;
	while (p < end) {
		char c = *p;
		if (c == ' ') {
			// Is this space part of the path?  Or the start of the next opcode?  We
			// don't know yet.
			potentialEnd = p;
			p += 1;
			// Skip any more spaces.
			while (p < end && *p == ' ')
				p += 1;
		}
		else if (c == '\n' || c == '\r' || c == '\t')
			break;
		else if (c == '=') {
			// We've been looking at an opcode; we need to rewind to
			// potentialEnd.
			p = potentialEnd;
			break;
		}
		p += 1;
	}
	if (p > pathStart) {
		std::string path(pathStart, p);
		*pathOut = path;
	}
	else
		*pathOut = std::string("");
	return p;
}

int SFZParser::keyValue(const std::string& str)
{
	char c = str[0];
	if (c >= '0' && c <= '9')
		return atoi(str.c_str());

	int note = 0;
	static const int notes[] = {
		12 + 0, 12 + 2, 3, 5, 7, 8, 10,
		};
	if (c >= 'A' && c <= 'G')
		note = notes[c - 'A'];
	else if (c >= 'a' && c <= 'g')
		note = notes[c - 'a'];
	int octaveStart = 1;
	c = str[1];
	if (c == 'b' || c == '#') {
		octaveStart += 1;
		if (c == 'b')
			note -= 1;
		else
			note += 1;
		}
	int octave = atoi(str.substr(octaveStart).c_str());
	// A3 == 57.
	int result = octave * 12 + note + (57 - 4 * 12);
	return result;
}

int SFZParser::triggerValue(const std::string& str)
{
	if (str == "release")
		return SFZRegion::RELEASE;
	else if (str == "first")
		return SFZRegion::FIRST;
	else if (str == "legato")
		return SFZRegion::LEGATO;
	return SFZRegion::ATTACK;
}

int SFZParser::loopModeValue(const std::string& str)
{
	if (str == "no_loop")
		return SFZRegion::NO_LOOP;
	else if (str == "one_shot")
		return SFZRegion::ONE_SHOT;
	else if (str == "loop_continuous")
		return SFZRegion::LOOP_CONTINUOUS;
	else if (str == "loop_sustain")
		return SFZRegion::LOOP_SUSTAIN;
	return SFZRegion::SAMPLE_LOOP;
}

void SFZParser::finishRegion(SFZRegion* region)
{
	SFZRegion* newRegion = new SFZRegion(region->instrument);
	*newRegion = *region;
	instrument.regions.push_back(newRegion);
}

void SFZParser::error(const std::string& message)
{
	std::string fullMessage = message;
	printf("%s\n", fullMessage.c_str());
}

// ----------------------------------------

SFZRegion::SFZRegion(Instrument* i)
{
	instrument = i;
	clear();
}

void SFZRegion::clear()
{
	lokey = 1;
	hikey = 127;
	lovel = 1;
	hivel = 127;
	pitch_keycenter = 60; 	// C4
	pitch_keytrack = 100;
	bend_up = 200;
	bend_down = -200;
	volume = pan = 0.0;
	amp_veltrack = 100.0;
}

void SFZRegion::dump()
{
	printf("%d - %d, vel %d - %d", lokey, hikey, lovel, hivel);
	if (!sample.empty()) {
		printf(": %s", sample.c_str());
	}
	printf("\n");
}


// -----------------------------------------

Instrument::Instrument()
{
}

Instrument::~Instrument()
{
	uint32_t i;
	for (i = 0; i < regions.size(); i++) {
		delete regions[i];
	}
}

}
