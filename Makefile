PREFIX ?= /usr/local
LIBDIR ?= lib

NAME    = zam-plugins
VERSION = $(shell cat .version)

PLUGINS=ZamComp ZamCompX2 ZaMultiComp ZaMultiCompX2 ZamTube ZamEQ2 ZamAutoSat

all: libs $(PLUGINS) gen

libs: FORCE
	$(MAKE) -C libs/dgl

gen: plugins libs/lv2_ttl_generator
	@./libs/generate-ttl.sh

libs/lv2_ttl_generator:
	$(MAKE) -C libs/lv2-ttl-generator

$(PLUGINS): libs
	$(MAKE) -C plugins/$@

install: all
	install -d $(DESTDIR)$(PREFIX)/$(LIBDIR)/ladspa \
		$(DESTDIR)$(PREFIX)/$(LIBDIR)/lv2 ; \
	for plugin in $(PLUGINS); do \
		install -d $(DESTDIR)$(PREFIX)/$(LIBDIR)/lv2/"$$plugin".lv2 ; \
		install -t $(DESTDIR)$(PREFIX)/$(LIBDIR)/lv2/"$$plugin".lv2 \
			bin/"$$plugin".lv2/* ; \
	done; \
	install -t $(DESTDIR)$(PREFIX)/$(LIBDIR)/ladspa bin/*-ladspa.so

uninstall:
	for plugin in $(PLUGINS); do \
		rm -rf $(DESTDIR)$(PREFIX)/$(LIBDIR)/lv2/"$$plugin".lv2 ; \
		rm -f $(DESTDIR)$(PREFIX)/$(LIBDIR)/ladspa/"$$plugin"-ladspa.so ;
	done


plugins: FORCE


clean: FORCE
	for plugin in $(PLUGINS); do \
		$(MAKE) PREFIX="$(PREFIX)" LIBDIR="$(LIBDIR)" -C plugins/"$$plugin" clean; \
	done
	$(MAKE) clean -C libs/dgl
	$(MAKE) clean -C libs/lv2-ttl-generator

.version: FORCE
	if test -d .git; then \
		git describe > .version; \
	fi

_dist_pre: .version FORCE
	rm -rf dist
	mkdir dist
	git clone . dist/$(NAME)-$(VERSION)
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

