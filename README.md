zam-plugins
===========

Collection of LV2/LADSPA/VST/JACK audio plugins for high quality processing

Note:

This is a new repo that contains the full history of the old repo.
Submodules are now required (>= 3.6).
Uses Distrho Plugin Framework.
Please use versions >= 3.6 of this package.

Build Dependencies:
===================

	pkg-config libx11-dev libgl-dev liblo-dev libjack-dev ladspa-sdk


Bleeding edge installation:
===========================

	git submodule init
	git submodule update
	make
	sudo make install


Package Maintainers:
====================

	git submodule init
	git submodule update
	git checkout 3.6
	make dist
