#!/usr/bin/make -f
# Makefile for zita-convolver #
# -------------------------------- #
# Created by falkTX
#

# NOTE: Makefile.plugins.mk must have been included before this file!

$(BUILD_DIR)/zita-convolver.cpp.o: ../../lib/zita-convolver-4.0.0/zita-convolver.cpp
	-@mkdir -p $(BUILD_DIR)
	@echo "Compiling zita-convolver.cpp"
	@$(CXX) $< $(BUILD_CXX_FLAGS) -c -o $@

-include $(BUILD_DIR)/zita-convolver.cpp.d
