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

CPP_COMPILER ?= g++
TEST_COMPILER ?= g++
CPP_FLAGS :=
TEST_FLAGS :=

ifeq ($(dbg),1)
      CPP_FLAGS += -g -O0
	  TEST_FLAGS += -g -O0
      BUILD_TYPE := debug
else
      CPP_FLAGS += -O2
	  TEST_FLAGS += -O2
      BUILD_TYPE := release
endif

ALL_CPP_FLAGS := -Wall -fPIC
ALL_CPP_FLAGS += $(CPP_FLAGS)

ALL_LDFLAGS :=
ALL_LDFLAGS += $(ALL_CPP_FLAGS)

INCLUDES  := -I./include -I/usr/include -I/usr/local/include -I/usr/local/sypha/include
TEST_INCLUDES := -I./include -Itest/include
LIBRARIES := -L/usr/local/sypha/lib -l:libsyphac.a
TEST_LIBRARIES := -L/usr/local/sypha/lib -l:libsyphac.a -Lbin/$(TARGET_ARCH)/$(TARGET_OS)/$(BUILD_TYPE) -l:libsyphacpp.a.$(MAJOR_VERSION).$(MINOR_VERSION)
ALL_CPP_FLAGS += --std=c++11

# Target rules
all: clean-build

clean-build: clean build

build: libsyphacpp.a.$(MAJOR_VERSION).$(MINOR_VERSION) libsyphacpp.so.$(MAJOR_VERSION).$(MINOR_VERSION)

out/sypha_opt.o: src/sypha_opt.cpp
	mkdir -p out
	$(CPP_COMPILER) $(INCLUDES) $(ALL_CPP_FLAGS) -o $@ -c $<

libsyphacpp.a.$(MAJOR_VERSION).$(MINOR_VERSION): out/sypha_opt.o
	ar cr $@ $+
	mkdir -p bin/$(TARGET_ARCH)/$(TARGET_OS)/$(BUILD_TYPE)
	mv $@ bin/$(TARGET_ARCH)/$(TARGET_OS)/$(BUILD_TYPE)

libsyphacpp.so.$(MAJOR_VERSION).$(MINOR_VERSION): out/sypha_opt.o
	$(CPP_COMPILER) $(ALL_LDFLAGS) $(GENCODE_FLAGS) -shared -o $@ $+ $(LIBRARIES)
	mkdir -p bin/$(TARGET_ARCH)/$(TARGET_OS)/$(BUILD_TYPE)
	mv $@ bin/$(TARGET_ARCH)/$(TARGET_OS)/$(BUILD_TYPE)

clean:
	rm -fr out
	rm -rf bin/$(TARGET_ARCH)/$(TARGET_OS)/$(BUILD_TYPE)/libsyphacpp.a.$(MAJOR_VERSION).$(MINOR_VERSION)
	rm -rf bin/$(TARGET_ARCH)/$(TARGET_OS)/$(BUILD_TYPE)/libsyphacpp.so.$(MAJOR_VERSION).$(MINOR_VERSION)

install:
	mkdir -p $(INSTALL_DIR)/sypha
	mkdir -p $(INSTALL_DIR)/sypha/include
	cp -fr include/syphacpp $(INSTALL_DIR)/sypha/include
	mkdir -p $(INSTALL_DIR)/sypha/lib
	cp -f bin/$(TARGET_ARCH)/$(TARGET_OS)/$(BUILD_TYPE)/libsyphacpp.a.$(MAJOR_VERSION).$(MINOR_VERSION) $(INSTALL_DIR)/sypha/lib/libsyphacpp.a
	cp -f bin/$(TARGET_ARCH)/$(TARGET_OS)/$(BUILD_TYPE)/libsyphacpp.so.$(MAJOR_VERSION).$(MINOR_VERSION) $(INSTALL_DIR)/sypha/lib
	ln -fs $(INSTALL_DIR)/sypha/lib/libsyphacpp.so.$(MAJOR_VERSION).$(MINOR_VERSION) $(INSTALL_DIR)/sypha/lib/libsyphacpp.so.$(MAJOR_VERSION)
	ln -fs $(INSTALL_DIR)/sypha/lib/libsyphacpp.so.$(MAJOR_VERSION).$(MINOR_VERSION) $(INSTALL_DIR)/sypha/lib/libsyphacpp.so

uninstall:
	rm -fr $(INSTALL_DIR)/sypha/include/syphacpp
	rm -fr $(INSTALL_DIR)/sypha/lib/libsyphacpp.a
	rm -fr $(INSTALL_DIR)/sypha/lib/libsyphacpp.so*

# Test case sections

out/test_main.o: test/src/test_main.cpp
	mkdir -p out
	$(TEST_COMPILER) $(TEST_INCLUDES) -o $@ -c $<

out/test_env.o: test/src/test_env.cpp
	mkdir -p out
	$(TEST_COMPILER) $(TEST_INCLUDES) -o $@ -c $<

out/test_opt.o: test/src/test_opt.cpp
	mkdir -p out
	$(TEST_COMPILER) $(TEST_INCLUDES) -o $@ -c $<

test: out/test_main.o out/test_env.o out/test_opt.o
	$(TEST_COMPILER) -o libsyphac_$@ $+ $(TEST_LIBRARIES)
	mkdir -p bin/$(TARGET_ARCH)/$(TARGET_OS)/$(BUILD_TYPE)
	mv libsyphac_$@ bin/$(TARGET_ARCH)/$(TARGET_OS)/$(BUILD_TYPE)
	bin/$(TARGET_ARCH)/$(TARGET_OS)/$(BUILD_TYPE)/libsyphac_$@
