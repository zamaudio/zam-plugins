zam-plugins [![travis](https://travis-ci.org/zamaudio/zam-plugins.svg?branch=master)](https://travis-ci.org/zamaudio/zam-plugins)
===========

Collection of LV2/LADSPA/VST/JACK audio plugins for high quality processing

Note:

This is a new repo that contains the full history of the old repo.
Submodules are now required (>= 3.6).
Uses Distrho Plugin Framework.
Please use version 3.9 for a stable package, latest git is experimental.

Build Dependencies:
===================

	pkg-config libx11-dev libgl-dev liblo-dev libjack-dev ladspa-sdk


Bleeding edge installation:
===========================

	git submodule init
	git submodule update
	make
	sudo make install


Cross-compiling with docker:
============================

	sudo docker build . -t zam-plugins-build
	sudo docker run -v `pwd`:/tmp/build zam-plugins-build /bin/bash -c "cd /tmp/build && ..."

(See .travis.yml file for the required cross build commands)


Package Maintainers:
====================

	git checkout 3.9
	make dist
