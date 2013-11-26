PREFIX ?= /usr/local

all: zamautosat zamcomp zamcompexp zamcompx2 zameq2 zamulticomp zamvalve zamvalve-tanh zamdither zamtube zamexcite Makefile

zamautosat: force_look
	$(MAKE) -C zamautosat

zamcomp: force_look
	$(MAKE) -C zamcomp

zamcompx2: force_look
	$(MAKE) -C zamcompx2

zamcompexp: force_look
	$(MAKE) -C zamcompexp

zameq2: force_look
	$(MAKE) -C zameq2

zamulticomp: force_look
	$(MAKE) -C zamulticomp

zamvalve: force_look
	$(MAKE) -C zamvalve

zamvalve-tanh: force_look
	$(MAKE) -C zamvalve-tanh

zamtube: force_look
	$(MAKE) -C zamtube

zamdither: force_look
	$(MAKE) -C zamdither

zamexcite: force_look
	$(MAKE) -C zamexcite

force_look:
	true

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

clean: force_look
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

