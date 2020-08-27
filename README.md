zam-plugins [![travis](https://travis-ci.org/zamaudio/zam-plugins.svg?branch=master)](https://travis-ci.org/zamaudio/zam-plugins)
===========

Collection of LV2/LADSPA/VST/JACK audio plugins for high quality processing

Note:

This is a new repo that contains the full history of the old repo.
Submodules are now required (>= 3.6).
Uses Distrho Plugin Framework.


Build Dependencies:
===================

	pkg-config libx11-dev libgl-dev liblo-dev libjack-dev ladspa-sdk


Bleeding edge installation:
===========================

	git submodule update --init
	make
	sudo make install


Cross-compiling with docker:
============================

	# Make sure docker is installed and configured
	git submodule update --init
	./make-all


Package Maintainers:
====================

	git checkout 3.13
	make dist
