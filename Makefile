PREFIX ?= /usr/local

NAME    = zam-plugins
VERSION = $(shell cat .version)

all: zamautosat zamcomp zamcompexp zamcompx2 zameq2 zamulticomp zamvalve zamvalve-tanh zamdither zamtube zamexcite Makefile

zamautosat: FORCE
	$(MAKE) -C zamautosat

zamcomp: FORCE
	$(MAKE) -C zamcomp

zamcompx2: FORCE
	$(MAKE) -C zamcompx2

zamcompexp: FORCE
	$(MAKE) -C zamcompexp

zameq2: FORCE
	$(MAKE) -C zameq2

zamulticomp: FORCE
	$(MAKE) -C zamulticomp

zamvalve: FORCE
	$(MAKE) -C zamvalve

zamvalve-tanh: FORCE
	$(MAKE) -C zamvalve-tanh

zamtube: FORCE
	$(MAKE) -C zamtube

zamdither: FORCE
	$(MAKE) -C zamdither

zamexcite: FORCE
	$(MAKE) -C zamexcite

install: all
	install -d $(DESTDIR)$(PREFIX)/lib/ladspa $(DESTDIR)$(PREFIX)/lib/lv2
	$(MAKE) -C zamautosat install
	$(MAKE) -C zamcomp install
	$(MAKE) -C zamcompx2 install
	$(MAKE) -C zamcompexp install
	$(MAKE) -C zameq2 install
	$(MAKE) -C zamulticomp install
	$(MAKE) -C zamvalve install
	$(MAKE) -C zamvalve-tanh install
	$(MAKE) -C zamdither install
	$(MAKE) -C zamtube install
	$(MAKE) -C zamexcite install

clean: FORCE
	$(MAKE) -C zamautosat clean
	$(MAKE) -C zamcomp clean
	$(MAKE) -C zamcompx2 clean
	$(MAKE) -C zamcompexp clean
	$(MAKE) -C zameq2 clean
	$(MAKE) -C zamulticomp clean
	$(MAKE) -C zamvalve clean
	$(MAKE) -C zamvalve-tanh clean
	$(MAKE) -C zamdither clean
	$(MAKE) -C zamtube clean
	$(MAKE) -C zamexcite clean

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
