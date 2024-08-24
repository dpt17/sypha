# Makefile
#
# Copyright 2024 David Tuttle
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

MAJOR_VERSION=0
MINOR_VERSION=1

INSTALL_DIR ?= /usr/local

TARGET_ARCH ?= x86_64
TARGET_SZ ?= 64
TARGET_OS ?= linux

C_COMPILER ?= gcc
C_FLAGS :=

ifeq ($(dbg),1)
	  C_FLAGS += -g -O0
      BUILD_TYPE := debug
else
	  C_FLAGS += -O2
      BUILD_TYPE := release
endif

ALL_C_FLAGS := -Wall -fPIC
ALL_C_FLAGS += $(C_FLAGS)

ALL_LDFLAGS :=

INCLUDES  := -I./include
LIBRARIES :=

# Target rules
all: clean-build

clean-build: clean build

build: libsyphac.a.$(MAJOR_VERSION).$(MINOR_VERSION) libsyphac.so.$(MAJOR_VERSION).$(MINOR_VERSION)

out/sypha_opt.o: src/sypha_opt.c
	mkdir -p out
	$(C_COMPILER) $(INCLUDES) $(ALL_C_FLAGS) -o $@ -c $<

out/sypha_env.o: src/sypha_env.c
	mkdir -p out
	$(C_COMPILER) $(INCLUDES) $(ALL_C_FLAGS) -o $@ -c $<

out/sypha_list.o: src/sypha_list.c
	mkdir -p out
	$(C_COMPILER) $(INCLUDES) $(ALL_C_FLAGS) -o $@ -c $<

libsyphac.a.$(MAJOR_VERSION).$(MINOR_VERSION): out/sypha_opt.o out/sypha_env.o out/sypha_list.o
	ar cr $@ $+
	mkdir -p bin/$(TARGET_ARCH)/$(TARGET_OS)/$(BUILD_TYPE)
	mv $@ bin/$(TARGET_ARCH)/$(TARGET_OS)/$(BUILD_TYPE)

libsyphac.so.$(MAJOR_VERSION).$(MINOR_VERSION): out/sypha_opt.o out/sypha_env.o out/sypha_list.o
	$(C_COMPILER) $(ALL_LDFLAGS) $(GENCODE_FLAGS) -shared -o $@ $+ $(LIBRARIES)
	mkdir -p bin/$(TARGET_ARCH)/$(TARGET_OS)/$(BUILD_TYPE)
	mv $@ bin/$(TARGET_ARCH)/$(TARGET_OS)/$(BUILD_TYPE)

clean:
	rm -fr out
	rm -rf bin/$(TARGET_ARCH)/$(TARGET_OS)/$(BUILD_TYPE)/libsyphac.a.$(MAJOR_VERSION).$(MINOR_VERSION)
	rm -rf bin/$(TARGET_ARCH)/$(TARGET_OS)/$(BUILD_TYPE)/libsyphac.so.$(MAJOR_VERSION).$(MINOR_VERSION)

install:
	mkdir -p $(INSTALL_DIR)/sypha
	mkdir -p $(INSTALL_DIR)/sypha/include
	cp -fr include/syphac $(INSTALL_DIR)/sypha/include
	mkdir -p $(INSTALL_DIR)/sypha/lib
	cp -f bin/$(TARGET_ARCH)/$(TARGET_OS)/$(BUILD_TYPE)/libsyphac.a.$(MAJOR_VERSION).$(MINOR_VERSION) $(INSTALL_DIR)/sypha/lib/libsyphac.a
	cp -f bin/$(TARGET_ARCH)/$(TARGET_OS)/$(BUILD_TYPE)/libsyphac.so.$(MAJOR_VERSION).$(MINOR_VERSION) $(INSTALL_DIR)/sypha/lib
	ln -fs $(INSTALL_DIR)/sypha/lib/libsyphac.so.$(MAJOR_VERSION).$(MINOR_VERSION) $(INSTALL_DIR)/sypha/lib/libsyphac.so.$(MAJOR_VERSION)
	ln -fs $(INSTALL_DIR)/sypha/lib/libsyphac.so.$(MAJOR_VERSION).$(MINOR_VERSION) $(INSTALL_DIR)/sypha/lib/libsyphac.so

uninstall:
	rm -fr $(INSTALL_DIR)/sypha/include/syphac
	rm -fr $(INSTALL_DIR)/sypha/lib/libsyphac.a
	rm -fr $(INSTALL_DIR)/sypha/lib/libsyphac.so*
