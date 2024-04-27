PREFIX ?= /usr/local
LIBDIR ?= lib
BINDIR ?= bin
HAVE_DGL ?= true

NAME    = zam-plugins
VERSION = $(shell cat .version)

PLUGINS=ZamComp ZamCompX2 ZaMultiComp ZamTube ZamEQ2 ZamAutoSat ZamGEQ31 ZaMultiCompX2 ZamGate ZamGateX2 ZamHeadX2 ZaMaximX2 ZamDelay ZamDynamicEQ ZamPhono ZamVerb ZamGrains ZamNoise

include dpf/Makefile.base.mk

# --------------------------------------------------------------

all: libs $(PLUGINS) gen

libs:
ifeq ($(HAVE_DGL),true)
	$(MAKE) -C dpf/dgl
endif

gen: $(PLUGINS) dpf/utils/lv2_ttl_generator
	@$(CURDIR)/dpf/utils/generate-ttl.sh

dpf/utils/lv2_ttl_generator:
	$(MAKE) -C dpf/utils/lv2-ttl-generator

$(PLUGINS): libs
	$(MAKE) -C plugins/$@

# --------------------------------------------------------------

install: all
	install -d $(DESTDIR)$(PREFIX)/$(LIBDIR)/ladspa \
		$(DESTDIR)$(PREFIX)/$(LIBDIR)/lv2 \
		$(DESTDIR)$(PREFIX)/$(LIBDIR)/vst \
		$(DESTDIR)$(PREFIX)/$(LIBDIR)/vst3 \
		$(DESTDIR)$(PREFIX)/$(LIBDIR)/clap \
		$(DESTDIR)$(PREFIX)/$(BINDIR)
	for plugin in $(PLUGINS); do \
		install -d $(DESTDIR)$(PREFIX)/$(LIBDIR)/lv2/"$$plugin".lv2 ; \
		install -m 644 bin/"$$plugin".lv2/* \
			$(DESTDIR)$(PREFIX)/$(LIBDIR)/lv2/"$$plugin".lv2/ ; \
		cp -r bin/"$$plugin".vst3 $(DESTDIR)$(PREFIX)/$(LIBDIR)/vst3/ ; \
	done;
	for plugin in $(PLUGINS); do \
		install -m 755 bin/"$$plugin"$(APP_EXT) $(DESTDIR)$(PREFIX)/$(BINDIR)/ ; \
	done;
	install -m 644 bin/*-ladspa$(LIB_EXT) $(DESTDIR)$(PREFIX)/$(LIBDIR)/ladspa/
	install -m 644 bin/*-vst$(LIB_EXT) $(DESTDIR)$(PREFIX)/$(LIBDIR)/vst/
	install -m 644 bin/*.clap $(DESTDIR)$(PREFIX)/$(LIBDIR)/clap/

uninstall:
	for plugin in $(PLUGINS); do \
		rm -rf $(DESTDIR)$(PREFIX)/$(LIBDIR)/lv2/"$$plugin".lv2 ; \
		rm -rf $(DESTDIR)$(PREFIX)/$(LIBDIR)/vst3/"$$plugin".vst3 ; \
		rm -f $(DESTDIR)$(PREFIX)/$(LIBDIR)/ladspa/"$$plugin"-ladspa$(LIB_EXT) ; \
		rm -f $(DESTDIR)$(PREFIX)/$(LIBDIR)/vst/"$$plugin"-vst$(LIB_EXT) ; \
		rm -f $(DESTDIR)$(PREFIX)/$(LIBDIR)/vst/"$$plugin".clap ; \
		rm -f $(DESTDIR)$(PREFIX)/$(BINDIR)/"$$plugin" ; \
	done

# --------------------------------------------------------------

clean:
	for plugin in $(PLUGINS); do \
		$(MAKE) -C plugins/"$$plugin" clean; \
	done
ifeq ($(HAVE_DGL),true)
	$(MAKE) clean -C dpf/dgl
endif
	$(MAKE) clean -C dpf/utils/lv2-ttl-generator
	rm -f lib/zita-convolver-4.0.0/*.o lib/zita-convolver-4.0.0/*.d
	rm -f plugins/ZamSFZ/libsfz/*.o plugins/ZamSFZ/libsfz/*.d

# --------------------------------------------------------------

.version: FORCE
	if test -d .git; then \
		git describe > .version; \
	fi

_dist_pre: .version FORCE
	rm -rf dist
	mkdir dist
	git clone --recursive . dist/$(NAME)-$(VERSION)
	(cd dist/$(NAME)-$(VERSION); git checkout $(VERSION))
	$(MAKE) -C dist/$(NAME)-$(VERSION) plugins .version
	rm -rf dist/$(NAME)-$(VERSION)/.git* \
		dist/$(NAME)-$(VERSION)/*/.git*

_dist_post: FORCE
	rm -rf dist

_dist_bz2: FORCE
	cd dist; tar -cvjf ../$(NAME)-$(VERSION).tar.bz2 $(NAME)-$(VERSION)

_dist_gz: FORCE
	cd dist; tar -cvzf ../$(NAME)-$(VERSION).tar.gz $(NAME)-$(VERSION)

_dist_xz: FORCE
	cd dist; tar -cvJf ../$(NAME)-$(VERSION).tar.xz $(NAME)-$(VERSION)

_dist_zip: FORCE
	cd dist; zip -r ../$(NAME)-$(VERSION).zip $(NAME)-$(VERSION)

dist_bz2: _dist_pre _dist_bz2 _dist_post
dist_gz: _dist_pre _dist_gz _dist_post
dist_xz: _dist_pre _dist_xz _dist_post
dist_zip: _dist_pre _dist_zip _dist_post
dist: dist_xz

FORCE:

