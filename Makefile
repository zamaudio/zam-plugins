PREFIX ?= /usr/local

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

# pseudo target to force (re)making other targets
FORCE:
