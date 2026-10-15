# Orion Store Teste - experimental OpenOrbis / SDL2 PS4 build.
# Derived build procedure from publicly documented OpenOrbis sample conventions,
# not from copied application code. Requires a COMPLETE OpenOrbis SDK installation.
TITLE      := Orion Store Teste
VERSION    := 1.00
TITLE_ID   := ORNT00001
CONTENT_ID := IV0000-ORNT00001_00-ORIONSTORETESTE1
TOOLCHAIN  := $(OO_PS4_TOOLCHAIN)
SOURCE     := .
BUILD      := build
CXX        := clang++
LD         := ld.lld
OS_NAME    := $(shell uname -s)
ifeq ($(OS_NAME),Darwin)
TOOL_BIN   := macos
else
TOOL_BIN   := linux
endif
SDK_TOOLS  := $(TOOLCHAIN)/bin/$(TOOL_BIN)
FILES      := main.cpp ui_model.cpp
OBJECTS    := $(patsubst %.cpp,$(BUILD)/%.o,$(FILES))
ASSETS     := $(wildcard assets/*)
MODULES    := $(wildcard sce_module/*)
LIBS       := -lc -lkernel -lc++ -lSceUserService -lSceSystemService -lSceVideoOut -lSceAudioOut -lScePad -lSceSysmodule -lSDL2
CXXFLAGS   := --target=x86_64-pc-freebsd12-elf -std=c++11 -O2 -fPIC -funwind-tables -fno-exceptions -fno-rtti -c -isysroot $(TOOLCHAIN) -isystem $(TOOLCHAIN)/include -isystem $(TOOLCHAIN)/include/c++/v1
LDFLAGS    := -m elf_x86_64 -pie --script $(TOOLCHAIN)/link.x --eh-frame-hdr -L$(TOOLCHAIN)/lib $(LIBS) $(TOOLCHAIN)/lib/crt1.o

.PHONY: all clean check
all: check $(CONTENT_ID).pkg

check:
	@test -n "$(TOOLCHAIN)" || (echo "Missing OO_PS4_TOOLCHAIN"; exit 1)
	@test -f "$(TOOLCHAIN)/link.x" || (echo "Missing OpenOrbis SDK: link.x"; exit 1)
	@test -f "$(TOOLCHAIN)/lib/libSDL2.a" || (echo "Missing PS4 SDL2 static library"; exit 1)
	@test -x "$(SDK_TOOLS)/create-fself" || (echo "Missing create-fself tool"; exit 1)
	@test -x "$(SDK_TOOLS)/create-gp4" || (echo "Missing create-gp4 tool"; exit 1)
	@test -x "$(SDK_TOOLS)/PkgTool.Core" || (echo "Missing PkgTool.Core tool"; exit 1)
	@test -f sce_sys/about/right.sprx || (echo "Missing sce_sys/about/right.sprx: copy from legally obtained OpenOrbis sample toolchain"; exit 1)
	@test -f sce_sys/icon0.png || (echo "Missing original app icon"; exit 1)

$(BUILD)/%.o: %.cpp
	@mkdir -p $(BUILD)
	$(CXX) $(CXXFLAGS) -o $@ $<

eboot.bin: $(OBJECTS)
	$(LD) $(OBJECTS) -o $(BUILD)/store_teste.elf $(LDFLAGS)
	$(SDK_TOOLS)/create-fself -in=$(BUILD)/store_teste.elf -out=$(BUILD)/store_teste.oelf --eboot eboot.bin --paid 0x3800000000000011

sce_sys/param.sfo: Makefile
	$(SDK_TOOLS)/PkgTool.Core sfo_new $@
	$(SDK_TOOLS)/PkgTool.Core sfo_setentry $@ APP_TYPE --type Integer --maxsize 4 --value 1
	$(SDK_TOOLS)/PkgTool.Core sfo_setentry $@ APP_VER --type Utf8 --maxsize 8 --value '$(VERSION)'
	$(SDK_TOOLS)/PkgTool.Core sfo_setentry $@ ATTRIBUTE --type Integer --maxsize 4 --value 0
	$(SDK_TOOLS)/PkgTool.Core sfo_setentry $@ CATEGORY --type Utf8 --maxsize 4 --value 'gd'
	$(SDK_TOOLS)/PkgTool.Core sfo_setentry $@ CONTENT_ID --type Utf8 --maxsize 48 --value '$(CONTENT_ID)'
	$(SDK_TOOLS)/PkgTool.Core sfo_setentry $@ DOWNLOAD_DATA_SIZE --type Integer --maxsize 4 --value 0
	$(SDK_TOOLS)/PkgTool.Core sfo_setentry $@ SYSTEM_VER --type Integer --maxsize 4 --value 0
	$(SDK_TOOLS)/PkgTool.Core sfo_setentry $@ TITLE --type Utf8 --maxsize 128 --value '$(TITLE)'
	$(SDK_TOOLS)/PkgTool.Core sfo_setentry $@ TITLE_ID --type Utf8 --maxsize 12 --value '$(TITLE_ID)'
	$(SDK_TOOLS)/PkgTool.Core sfo_setentry $@ VERSION --type Utf8 --maxsize 8 --value '$(VERSION)'

pkg.gp4: eboot.bin sce_sys/param.sfo sce_sys/icon0.png sce_sys/about/right.sprx $(ASSETS) $(MODULES)
	$(SDK_TOOLS)/create-gp4 -out $@ --content-id=$(CONTENT_ID) --files "$^"

$(CONTENT_ID).pkg: pkg.gp4
	$(SDK_TOOLS)/PkgTool.Core pkg_build $< .

clean:
	rm -rf $(BUILD) eboot.bin pkg.gp4 sce_sys/param.sfo $(CONTENT_ID).pkg
