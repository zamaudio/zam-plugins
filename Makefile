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

gen: plugins libs/lv2_ttl_generator
	$(MAKE) -C libs/lv2-ttl-generator

libs/lv2_ttl_generator:
	$(MAKE) -C libs/lv2-ttl-generator

# 	./utils/lv2_ttl_generator bin/lv2/ZamComp.lv2/ZamComp.so
# 	mv *.ttl bin/lv2/ZamComp.lv2

# --------------------------------------------------------------

install: all

# --------------------------------------------------------------

clean:
	$(MAKE) clean -C libs/dgl
	$(MAKE) clean -C plugins/ZamComp
	$(MAKE) clean -C libs/lv2-ttl-generator

# --------------------------------------------------------------

.PHONY: libs plugins
