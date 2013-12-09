PREFIX ?= /usr/local
LIBDIR ?= lib
OPTIMIZATIONS ?=

NAME    = zam-plugins
VERSION = $(shell cat .version)

SUBMODULES = zamautosat zamcomp zamcompexp zamcompx2 zameq2 zamulticomp \
			 zamvalve zamvalve-tanh zamdither zamtube zamexcite

all: $(SUBMODULES)

$(SUBMODULES): FORCE
	$(MAKE) -C $@

install: all
	install -d $(DESTDIR)$(PREFIX)/$(LIBDIR)/ladspa \
		$(DESTDIR)$(PREFIX)/$(LIBDIR)/lv2
	if test 'x$(OPTIMIZATIONS)' != 'x'; then \
		optimizations='OPTIMIZATIONS=$(OPTIMIZATIONS)'; \
	else \
		optimizations=''; \
	fi; \
	for submodule in $(SUBMODULES); do \
		$(MAKE) PREFIX="$(PREFIX)" LIBDIR="$(LIBDIR)" $$optimizations \
			-C "$$submodule" install; \
	done

clean: FORCE
	for submodule in $(SUBMODULES); do \
		$(MAKE) "PREFIX=$(PREFIX)" LIBDIR="$(LIBDIR)" -C "$$submodule" clean; \
	done

submodules: FORCE
	git submodule init
	git submodule update

.version: FORCE
	if test -d .git; then \
		git describe > .version; \
	fi

_dist_pre: .version FORCE
	rm -rf dist
	mkdir dist
	git clone . dist/$(NAME)-$(VERSION)
	(cd dist/$(NAME)-$(VERSION); git checkout $(VERSION))
	for submodule in $(SUBMODULES); do \
		echo "[submodule \"$$submodule\"]"; \
		echo "	path = $$submodule"; \
		echo "	url = file://$$PWD/$$submodule"; \
	done > dist/$(NAME)-$(VERSION)/.gitmodules
	$(MAKE) -C dist/$(NAME)-$(VERSION) submodules .version
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

# pseudo target to force (re)making other targets
FORCE:
