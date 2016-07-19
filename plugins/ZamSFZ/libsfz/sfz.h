#include <stdio.h>
#include <string.h>
#include <string>
#include <vector>
#include <cstdlib>
#include <inttypes.h>

namespace sfz {

class Instrument;

class StringSlice {
public:
	StringSlice(const char* startIn, const char* endIn)
		: start(startIn), end(endIn) {}

	unsigned int length() {
		return end - start;
	}

	bool operator==(const char* other) {
		return strncmp(start, other, length()) == 0;
	}
	bool operator!=(const char* other) {
		return strncmp(start, other, length()) != 0;
	}

	const char*	start;
	const char*	end;
};

class SFZRegion {
public:
	enum Trigger {
		ATTACK, RELEASE, FIRST, LEGATO
	};
	enum LoopMode {
		SAMPLE_LOOP, NO_LOOP, ONE_SHOT, LOOP_CONTINUOUS, LOOP_SUSTAIN
	};
	enum OffMode {
		FAST, NORMAL
	};


	SFZRegion(Instrument* i);
	void	clear();
	void	dump();

	bool	matches(unsigned char note, unsigned char velocity, Trigger trigger) {
		return
			note >= lokey && note <= hikey &&
			velocity >= lovel && velocity <= hivel &&
			(trigger == this->trigger ||
			 (this->trigger == ATTACK && (trigger == FIRST || trigger == LEGATO)));
	}

	Instrument* instrument;
	unsigned char lokey, hikey;
	unsigned char lovel, hivel;
	Trigger trigger;
	unsigned long group, off_by;
	OffMode off_mode;

	std::string sample;
	unsigned long offset;
	unsigned long end;
	bool negative_end;
	LoopMode loop_mode;
	unsigned long loop_start, loop_end;
	int transpose;
	int tune;
	int pitch_keycenter, pitch_keytrack;
	int bend_up, bend_down;

	float volume, pan;
	float amp_veltrack;
};

class Instrument
{
public:
	Instrument();
	~Instrument();

	std::vector<SFZRegion*> regions;
};

class SFZParser {
public:
	SFZParser();
	~SFZParser();

	void	read(const char* text, unsigned int length);
	int	readsfz(std::string path);

	Instrument instrument;

protected:
	int	line;

	const char*	handleLineEnd(const char* p);
	const char*	readPathInto(std::string* pathOut, const char* p, const char* end);
	int 	keyValue(const std::string& str);
	int 	triggerValue(const std::string& str);
	int 	loopModeValue(const std::string& str);
	void	finishRegion(SFZRegion* region);
	void	error(const std::string& message);
};

}
