# Hey Emacs, this is a -*- makefile -*-
# By	 : Gary Lee
# E-mail : dreamer2006@163.com
# Addr	 : Ocean University of China
#----------------------------------------------------------------------------
# On command line:
# make all 			: Make software.
# make clean 		: Clean out built project files.
# make coff 		: Convert ELF to AVR COFF.
# make extcoff 		: Convert ELF to AVR Extended COFF.
# make program 		: Download the hex file to the device, using avrdude.
#                	  Please customize the avrdude settings below first!
# make debug 		: Start either simulavr or avarice as specified for debugging, 
#              		  with avr-gdb or avr-insight as the front end for debugging.
# make filename.s 	: Just compile filename.c into the assembler code only.
# make filename.i 	: Create a preprocessed source file for use in submitting
#                     bug reports to the GCC project.
# To rebuild project do "make clean" then "make all".
#----------------------------------------------------------------------------

# MCU name
MCU = atmega16

# Processor frequency.
F_CPU = 8000000

# Output format. (can be srec, ihex, binary)
FORMAT = ihex

# Target file name (without extension).
TARGET = uCOS2AVR

INCPATH  	= avr config os
TOPDIR 		= .
OUTPUTDIR 	= objs

export TOPDIR OUTPUTDIR MCU

OS_SRC = os/os_core.c   	\
         os/os_flag.c   	\
		 os/os_mbox.c   	\
		 os/os_mem.c    	\
		 os/os_mutex.c  	\
         os/os_q.c      	\
		 os/os_sem.c    	\
		 os/os_task.c   	\
		 os/os_time.c 

CPU_C  = avr/os_cpu_c.c    	\
         avr/avr_isr.c		\
		 avr/data_queue.c	\
		 avr/cmd_queue.c
		 
CPU_A  = avr/os_cpu_a.S    	\
         avr/avr_vect.S     
		 
APPS   = app/main.c  avr/uart.c  avr/res_control.c   

# List C source files here. (C dependencies are automatically generated.)
SRC = $(OS_SRC)     \
      $(CPU_C)      \
      $(APPS)       \

# List Assembler source files here.
#     Make them always end in a capital .S.  Files ending in a lowercase .s
#     will not be considered source files but generated files (assembler
#     output from the compiler), and will be deleted upon "make clean"!
#     Even though the DOS/Win* filesystem matches both .s and .S the same,
#     it will preserve the spelling of the filenames, and gcc itself does
#     care about how the name is spelled on its command-line.
ASRC = $(CPU_A)

# Optimization level, can be [0, 1, 2, 3, s]. 
#     0 = turn off optimization. s = optimize for size.
#     (Note: 3 is not always the best optimization level. See avr-libc FAQ.)
OPT = s

# Debugging format.
#     Native formats for AVR-GCC's -g are dwarf-2 [default] or stabs.
#     AVR Studio 4.10 requires dwarf-2.
#     AVR [Extended] COFF format requires stabs, plus an avr-objcopy run.
DEBUG = stabs

# List any extra directories to look for include files here.
#     Each directory must be seperated by a space.
#     Use forward slashes for directory separators.
#     For a directory that has spaces, enclose it in quotes.
EXTRAINCDIRS = $(INCPATH)

# Compiler flag to set the C Standard level.
#     c89   = "ANSI" C
#     gnu89 = c89 plus GCC extensions
#     c99   = ISO C99 standard (not yet fully implemented)
#     gnu99 = c99 plus GCC extensions
CSTANDARD = -std=gnu99

# Place -D or -U options here
CDEFS = -DF_CPU=$(F_CPU)UL

# Place -I options here
CINCS =

#---------------- Compiler Options ----------------
#  -g*:          generate debugging information
#  -O*:          optimization level
#  -f...:        tuning, see GCC manual and avr-libc documentation
#  -Wall...:     warning level
#  -Wa,...:      tell GCC to pass this to the assembler.
#    -adhlns...: create assembler listing
CFLAGS = -g$(DEBUG)
CFLAGS += $(CDEFS) $(CINCS)
CFLAGS += -O$(OPT)
CFLAGS += -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
CFLAGS += -Wall -Wstrict-prototypes
CFLAGS += -Wa,-adhlns=$(<:%.c=$(OUTPUTDIR)/%.lst)
CFLAGS += $(patsubst %,-I%,$(EXTRAINCDIRS))
CFLAGS += $(CSTANDARD)

export CFLAGS
#---------------- Assembler Options ----------------
#  -Wa,...:   tell GCC to pass this to the assembler.
#  -ahlms:    create listing
#  -gstabs:   have the assembler create line number information; note that
#             for use in COFF files, additional information about filenames
#             and function names needs to be present in the assembler source
#             files -- see avr-libc docs [FIXME: not yet described there]
ASFLAGS = -Wa,-adhlns=$(<:%.S=$(OUTPUTDIR)/%.lst),-gstabs 
export ASFLAGS
#---------------- Library Options ----------------
# Minimalistic printf version
PRINTF_LIB_MIN = -Wl,-u,vfprintf -lprintf_min

# Floating point printf version (requires MATH_LIB = -lm below)
PRINTF_LIB_FLOAT = -Wl,-u,vfprintf -lprintf_flt

# If this is left blank, then it will use the Standard printf version.
PRINTF_LIB = 
#PRINTF_LIB = $(PRINTF_LIB_MIN)
#PRINTF_LIB = $(PRINTF_LIB_FLOAT)

# Minimalistic scanf version
SCANF_LIB_MIN = -Wl,-u,vfscanf -lscanf_min

# Floating point + %[ scanf version (requires MATH_LIB = -lm below)
SCANF_LIB_FLOAT = -Wl,-u,vfscanf -lscanf_flt

# If this is left blank, then it will use the Standard scanf version.
SCANF_LIB = 
#SCANF_LIB = $(SCANF_LIB_MIN)
#SCANF_LIB = $(SCANF_LIB_FLOAT)

MATH_LIB = -lm

#---------------- Linker Options ----------------
#  -Wl,...:     tell GCC to pass this to linker.
#    -Map:      create map file
#    --cref:    add cross reference to  map file
LDFLAGS = -Wl,-Map=$(TARGET).map,--cref
LDFLAGS += $(EXTMEMOPTS)
LDFLAGS += $(PRINTF_LIB) $(SCANF_LIB) $(MATH_LIB)

#============================================================================

# Define programs and commands.
SHELL 		= sh
CC 			= avr-gcc
OBJCOPY 	= avr-objcopy
OBJDUMP 	= avr-objdump
SIZE 		= avr-size
NM 			= avr-nm
AVRDUDE 	= avrdude
REMOVE 		= rm -f
REMOVEDIR	= rm -fr
COPY 		= cp
WINSHELL 	= cmd

export CC

# Define Messages
# English
MSG_ERRORS_NONE 	= Errors: none
MSG_BEGIN 			= -------- 开始编译 --------
MSG_END   			= -------- 编译完成 --------
MSG_SIZE_BEFORE 	= Size before: 
MSG_SIZE_AFTER 		= Size after:
MSG_COFF 			= Converting to AVR COFF:
MSG_EXTENDED_COFF 	= Converting to AVR Extended COFF:
MSG_FLASH 			= Creating load file for Flash:
MSG_EEPROM 			= Creating load file for EEPROM:
MSG_EXTENDED_LISTING= Creating Extended Listing:
MSG_SYMBOL_TABLE 	= Creating Symbol Table:
MSG_LINKING 		= Linking:
MSG_COMPILING 		= Compiling:
MSG_ASSEMBLING 		= Assembling:
MSG_CLEANING 		= Cleaning project:

export MSG_COMPILING MSG_ASSEMBLING
# Define all object files.
OBJ = $(SRC:%.c=$(OUTPUTDIR)/%.o) $(ASRC:%.S=$(OUTPUTDIR)/%.o) 

# Define all listing files.
LST = $(SRC:%.c=$(OUTPUTDIR)/%.lst) $(ASRC:%.S=$(OUTPUTDIR)/%.lst) 

# Compiler flags to generate dependency files.
GENDEPFLAGS = -MD -MP -MF .dep/$(@F).d

# Combine all necessary flags and optional flags.
# Add target processor to flags.
ALL_CFLAGS  = -mmcu=$(MCU) -I. $(CFLAGS) $(GENDEPFLAGS)
ALL_ASFLAGS = -mmcu=$(MCU) -I. -x assembler-with-cpp $(ASFLAGS)

export ALL_CFLAGS ALL_ASFLAGS
# Default target.
all: begin gccversion sizebefore build sizeafter end

build: elf hex eep lss sym

elf: $(TARGET).elf
hex: $(TARGET).hex
eep: $(TARGET).eep
lss: $(TARGET).lss 
sym: $(TARGET).sym

# Eye candy.
# AVR Studio 3.x does not check make's exit code but relies on
# the following magic strings to be generated by the compile job.
begin:
	@echo
	@echo $(MSG_BEGIN)

end:
	@echo $(MSG_END)
	@echo

# Display size of file.
HEXSIZE = $(SIZE) --target=$(FORMAT) $(TARGET).hex
ELFSIZE = $(SIZE) -A $(TARGET).elf
ELFSIZE2 = $(SIZE) --mcu=$(MCU) --format=avr $(TARGET).elf
AVRMEM = avr-mem.sh $(TARGET).elf $(MCU)

sizebefore:
	@if test -f $(TARGET).elf; then echo; echo $(MSG_SIZE_BEFORE); $(ELFSIZE); \
	$(AVRMEM) 2>/dev/null; echo; fi

sizeafter:
	@if test -f $(TARGET).elf; then echo; echo $(MSG_SIZE_AFTER); $(ELFSIZE); $(ELFSIZE2)\
	$(AVRMEM) 2>/dev/null; echo; fi
	
# Display compiler version information.
gccversion : 
	@$(CC) --version

# Convert ELF to COFF for use in debugging / simulating in AVR Studio or VMLAB.
COFFCONVERT=$(OBJCOPY) --debugging \
		--change-section-address .data-0x800000 \
		--change-section-address .bss-0x800000 \
		--change-section-address .noinit-0x800000 \
		--change-section-address .eeprom-0x810000 

coff: $(TARGET).elf
	@echo
	@echo $(MSG_COFF) $(TARGET).cof
	$(COFFCONVERT) -O coff-avr $< $(TARGET).cof

extcoff: $(TARGET).elf
	@echo
	@echo $(MSG_EXTENDED_COFF) $(TARGET).cof
	$(COFFCONVERT) -O coff-ext-avr $< $(TARGET).cof

# Create final output files (.hex, .eep) from ELF output file.
%.hex: %.elf
	@echo
	@echo $(MSG_FLASH) $@
	$(OBJCOPY) -O $(FORMAT) -R .eeprom $< $@

%.eep: %.elf
	@echo
	@echo $(MSG_EEPROM) $@
	-$(OBJCOPY) -j .eeprom --set-section-flags=.eeprom="alloc,load" \
	--change-section-lma .eeprom=0 -O $(FORMAT) $< $@

# Create extended listing file from ELF output file.
%.lss: %.elf
	@echo
	@echo $(MSG_EXTENDED_LISTING) $@
	$(OBJDUMP) -h -S $< > $@

# Create a symbol table from ELF output file.
%.sym: %.elf
	@echo
	@echo $(MSG_SYMBOL_TABLE) $@
	$(NM) -n $< > $@

# Link: create ELF output file from object files.
.SECONDARY : $(TARGET).elf
.PRECIOUS : $(OBJ)
%.elf: $(OBJ)
	@echo
	@echo $(MSG_LINKING) $@
	$(CC) $(ALL_CFLAGS) $^ --output $@ $(LDFLAGS)

# Target: clean project.
clean: begin clean_list end

clean_list :
	@echo
	@echo $(MSG_CLEANING)
	$(REMOVE) $(TARGET).hex
	$(REMOVE) $(TARGET).eep
	$(REMOVE) $(TARGET).cof
	$(REMOVE) $(TARGET).elf
	$(REMOVE) $(TARGET).map
	$(REMOVE) $(TARGET).sym
	$(REMOVE) $(TARGET).lss
	$(REMOVE) $(SRC:%.c=$(OUTPUTDIR)/%.o)
	$(REMOVE) $(SRC:%.c=$(OUTPUTDIR)/%.lst)
	$(REMOVE) $(SRC:.c=.s)
	$(REMOVE) $(SRC:.c=.d)
	$(REMOVE) $(SRC:.c=.i)
	$(REMOVEDIR) .dep
	$(REMOVEDIR) ./objs

# Create object files directory
$(shell mkdir $(OUTPUTDIR) 		2>/dev/null)
$(shell mkdir $(OUTPUTDIR)/os 	2>/dev/null)
$(shell mkdir $(OUTPUTDIR)/app 	2>/dev/null)
$(shell mkdir $(OUTPUTDIR)/avr 	2>/dev/null)

# Include the dependency files.
-include $(shell mkdir .dep 2>/dev/null) $(wildcard .dep/*)

# Listing of phony targets.
.PHONY : all begin finish end sizebefore sizeafter gccversion \
build elf hex eep lss sym coff extcoff clean clean_list

include $(TOPDIR)/rules.mk
