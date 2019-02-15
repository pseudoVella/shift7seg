#Makefile for building arduino libraries+examples without the IDE
#General Settings
AVRDUDE_PATH ?= $(HOME)/arduino/sdk/hardware/tools
AVR_PATH ?= $(HOME)/arduino/sdk/hardware/tools/avr/bin
ARDUINO_INCLUDE ?= $(HOME)/arduino/sdk/hardware/arduino
THISDIR := $(shell pwd)
OBJDIR ?= build
CORE ?= arduino
CORE_DIR ?= $(ARDUINO_INCLUDE)/cores/$(CORE)
VARIANT ?= standard
F_CPU ?= 16000000L
MCU ?= atmega328p
BAUD ?= 115200
PORT ?= /dev/ttyACM0
PROG ?= stk500
#tools
CC := $(AVR_PATH)/avr-gcc
CXX := $(AVR_PATH)/avr-g++
AS := $(AVR_PATH)/avr-as
AR := $(AVR_PATH)/avr-ar
OBJCOPY := $(AVR_PATH)/avr-objcopy
OBJDUMP := $(AVR_PATH)/avr-objdump
SIZE := $(AVR_PATH)/avr-size
STRIP := $(AVR_PATH)/avr-strip
#extra-tools
AVRD := $(AVRDUDE_PATH)/avrdude
MKDIR := mkdir -p
RM := rm -rf
#suffixes
gcc_suffix := .c .cc
g++_suffix := .cpp .ino
gas_suffix := .S
all_suffix := $(gcc_suffix) $(g++_suffix) $(gas_suffix)
#flags for building dep files
define DEPFLAGS =
-MMD -MF $(@:.o=.d) -MT $@
endef
#preprocessor flags
CPPFLAGS := -mmcu=$(MCU) -DF_CPU=$(F_CPU) \
-I$(CORE_DIR) -I$(ARDUINO_INCLUDE)/variants/$(VARIANT) -Isrc
#c compiler flags
CFLAGS := -g -Os -Wall -ffunction-sections -fdata-sections
#c++ compiler flags
CXXFLAGS := -g -Os -Wall -ffunction-sections -fdata-sections \
-fpermissive -fno-exceptions
#assembler flags
ASFLAGS := -g -Os -Wall -ffunction-sections -fdata-sections \
-xassembler-with-cpp
#linker flags
LDFLAGS := -mmcu=$(MCU) -Wl,--gc-sections -Os

#MACROS
define build_objs =
	@$(MKDIR) $(@D)
	$(if $(filter $(foreach s,$(gcc_suffix),%$(s).o),$(@F)),\
	$(CC) $(DEPFLAGS) -c $(CPPFLAGS) $(CFLAGS) $? -o $@,\
	$(if $(filter $(foreach s,$(g++_suffix),%$(s).o),$(@F)),\
	$(CXX) $(DEPFLAGS) -c -xc++ $(CPPFLAGS) $(CXXFLAGS) $? -o $@,\
	$(if $(filter $(foreach s,$(gas_suffix),%$(s).o),$(@F)),\
	$(CC) $(DEPFLAGS) -c $(CPPFLAGS) $(ASFLAGS) $? -o $@)))
endef
define find_srcs =
$(foreach x,$(all_suffix),$(wildcard $(1)/*$(x)))
endef
define gen_objs =
$(foreach x,$(1),$(patsubst $(x),$(OBJDIR)/$(2)$(if $(4),/$(basename $(notdir $(x))))/$(notdir $(x)).$(3),$(x)))
endef
define rm_files =
	@echo "$(1)"
	@$(foreach x,$(2),$(if $(wildcard $(x)),$(RM) $(x); echo "Removed file : $(x)";))
endef
define avrupl =
	$(if $(3),,@echo "Uploading $(2) to port $(PORT) at $(BAUD)baud")
	$(AVRD) -p$(MCU) -b$(BAUD) -P$(PORT) -c$(PROG) -v -v -U $(type):$(1):$(2):i
endef
#Things to be built
#local library files
LOCAL_SRCS := $(call find_srcs,src)
LOCAL_OBJS := $(call gen_objs,$(LOCAL_SRCS),local,o)
LIBNAME := lib$(notdir $(shell pwd)).a
LOCAL_LIB := $(OBJDIR)/local/$(LIBNAME)
#Arduino core library files
CORE_SRCS := $(call find_srcs,$(CORE_DIR))
CORE_OBJS := $(call gen_objs,$(CORE_SRCS),$(CORE),o)
#example executables
EXAMPLES := $(notdir $(wildcard examples/*))
EX_SRCS := $(call find_srcs,examples/*)
EX_OBJS := $(call gen_objs,$(EX_SRCS),examples,o,1)
EX_ELFS := $(call gen_objs,$(EXAMPLES),examples,elf,1)
EX_BINS := $(call gen_objs,$(EXAMPLES),examples,bin,1)
EX_EEPS := $(call gen_objs,$(EXAMPLES),examples,eep,1)
EX_HEXS := $(call gen_objs,$(EXAMPLES),examples,hex,1)
#dependency files
DEPS := $(EX_OBJS:%.o=%.d) $(CORE_OBJS:%.o=%.d) $(LOCAL_OBJS:%.o=%.d)

#rules for building targets
all: $(LOCAL_LIB) $(EX_ELFS) $(EX_HEXS) $(EX_EEPS)
	@echo "Compilation Successfull!"
	@echo "You can upload an example sketch by typing :"
	@echo "  --> make upload utarget=\"example\""
	@echo "Where \"example\" is one of the following :"
	@$(foreach e,$(EXAMPLES),echo "  --> $(e)")

$(LOCAL_LIB): $(CORE_OBJS) $(LOCAL_OBJS)
	@$(MKDIR) $(@D)
	$(AR) rcs $@ $?

$(OBJDIR)/$(CORE)/%.o: $(CORE_DIR)/%
	$(build_objs)
$(OBJDIR)/local/%.o: src/%
	$(build_objs)
$(OBJDIR)/examples/%.o: examples/%
	$(build_objs)

%.eep: %.elf
	@$(MKDIR) $(@D)
	$(OBJCOPY) -j.eeprom --set-section-flags=.eeprom='alloc,load' \
--no-change-warnings --change-section-lma .eeprom=0 -O ihex $? $@
%.bin: %.elf
	@$(MKDIR) $(@D)
	$(OBJCOPY) -O binary $? $@
%.hex: %.elf
	@$(MKDIR) $(@D)
	$(OBJCOPY) -j.text -j.data --strip-unneeded -O ihex $? $@

upload:
	$(call avrupl,w,$(OBJDIR)/examples/$(utarget))
verify_upl:
	@echo "Verifying upload...."
	$(call avrupl,v,$(OBJDIR)/examples/$(utarget),1)
read_mem:
	@echo "Dumping $(type) memory to file $(dtarget)"
	$(call avrupl,r,$(dtarget),1)

clean:
	$(call rm_files,Checking for dependency files to clean...,$(DEPS))
	$(call rm_files,Checking for Arduino Core object files to clean...,$(CORE_OBJS))
	$(call rm_files,Checking for local object files to clean...,$(LOCAL_OBJS))
	$(call rm_files,Checking for example object files to clean...,$(EX_OBJS))
distclean:
	$(call rm_files,Removing build directory and it's contents...,$(OBJDIR))

-include $(DEPS)
.PHONY: distclean clean all examples upload_hex upload_eep
.PRECIOUS: $(EX_OBJS) $(CORE_OBJS) $(DEPS) $(LOCAL_OBJS) \
$(LOCAL_LIB) $(EX_ELFS) $(EX_BINS) $(EX_EEPS) $(EX_HEXS)
.SECONDEXPANSION:
%.elf: $$(@D)/*.o | $(EX_OBJS) $(LOCAL_LIB)
	$(CXX) $(LDFLAGS) -o $@ $? -L$(THISDIR)/build/local/ -l$(LIBNAME:lib%.a=%)
