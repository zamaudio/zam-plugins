#!/usr/bin/make -f
# Makefile for zam-plugins #
# ------------------------ #
# Created by falkTX
#

all: libs plugins gen

# --------------------------------------------------------------

libs:
	$(MAKE) -C libs/dgl

plugins: libs
	$(MAKE) -C plugins/ZamComp
	$(MAKE) -C plugins/ZamCompX2
	$(MAKE) -C plugins/ZaMultiComp
	$(MAKE) -C plugins/ZaMultiCompX2


gen: plugins libs/lv2_ttl_generator
	@./libs/generate-ttl.sh

libs/lv2_ttl_generator:
	$(MAKE) -C libs/lv2-ttl-generator

# --------------------------------------------------------------

install: all

# --------------------------------------------------------------

clean:
	$(MAKE) clean -C libs/dgl
	$(MAKE) clean -C plugins/ZamComp
	$(MAKE) clean -C plugins/ZamCompX2
	$(MAKE) clean -C plugins/ZaMultiComp
	$(MAKE) clean -C plugins/ZaMultiCompX2
	$(MAKE) clean -C libs/lv2-ttl-generator

# --------------------------------------------------------------

.PHONY: libs plugins
