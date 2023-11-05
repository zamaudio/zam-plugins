zam-plugins ![build](https://github.com/zamaudio/zam-plugins/actions/workflows/build.yml/badge.svg)
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

	git submodule update --init --recursive
	make
	sudo make install


Package Maintainers:
====================

	git checkout 4.2
	make dist
