#
# linux/arch/c6x/Makefile
#
# This file is subject to the terms and conditions of the GNU General Public
# License.  See the file "COPYING" in the main directory of this archive
# for more details.
#

PROCESSOR	 = c6x
ABI		:= elf

DEFINES	        := -D__linux__ -DNO_MM -D__TMS320C6X__
ifdef CONFIG_TMS320C64XPLUS
DEFINES         += -D__TMS320C6XPLUS__
endif
ifdef CONFIG_TMS320C66X
DEFINES         += -D__TMS320C6XPLUS__ -D__TMS320C66X__
endif

LDSCRIPT = $(obj)/arch/c6x/kernel/vmlinux.lds

ifdef CONFIG_TI_C6X_COMPILER
include $(srctree)/arch/c6x/Makefile.lib

nk-flags-                 := 0
nk-flags-y                := 1
be-flags-                 := 0
be-flags-y                := 1

arch-$(CONFIG_TMS320C64X)     := -march=6400
arch-$(CONFIG_TMS320C64XPLUS) := -march=6400+
arch-$(CONFIG_TMS320C66X)     := -march=6600

CFLAGS_MODULE   += -mlarge-memory-model=3 --visibility=default
AFLAGS_MODULE   += -mlarge-memory-model=3
KBUILD_AFLAGS   = -P -Wa,-d__ASSEMBLY__  \
	-Wa,-I$(srctree)/arch/c6x/include/asm -Wa,-I$(obj)/arch/c6x/kernel \
	-Wa,-d__BIG_ENDIAN__=$(be-flags-$(CONFIG_CPU_BIG_ENDIAN))

AS		 = $(CROSS_COMPILE)gcc -c

HOSTCFLAGS_kallsyms.o += -DTI_ASM
export HOSTCFLAGS_kallsyms.o

# native works well enough but cross toolchain should have one
NM		= /usr/bin/nm
else
arch-$(CONFIG_TMS320C64X)     := -march=c64x
arch-$(CONFIG_TMS320C64XPLUS) := -march=c64x+ -msdata=none -mno-dsbt
arch-$(CONFIG_TMS320C66X)     := -march=c64x+ -msdata=none -mno-dsbt

CFLAGS_MODULE   += -mlong-calls
CFLAGS_KERNEL   += -mlong-calls
endif

ifdef CONFIG_TI_C6X_LINKER
LINKFLAGS	= -Wl,--unused_section_elimination=off

MODULESCRIPT := $(srctree)/arch/c6x/modules.cmd
export MODULESCRIPT
LDFLAGS_MODULE = $(MODULESCRIPT)

# Flags used for the final link
LDFLAGS_vmlinux	:= --final-script=arch/c6x/vmlinux.cmd \
		   -Wl,-m,arch/c6x/vmlinux.map -Wl,--reread_libs

# FIXME. Why isn't the elf linker getting this from --final-script ?
LDFLAGS_vmlinux	+= -Wl,arch/c6x/vmlinux.cmd

LD		= $(CROSS_COMPILE)gcc -nostdlib $(LINKFLAGS)
endif

KBUILD_CPPFLAGS += -I$(srctree)/arch/c6x/platforms/include -I$(obj)/arch/c6x/kernel

ifdef CONFIG_DEBUG_INFO
# KBUILD_CFLAGS	:= $(CPPFLAGS) $(DEFINES) -g
KBUILD_CFLAGS   += $(DEFINES) -g
KBUILD_AFLAGS   += -g
else
# KBUILD_CFLAGS := $(CPPFLAGS) $(DEFINES) -Os
KBUILD_CFLAGS   += $(DEFINES) -O2
endif

ifdef CONFIG_NO_FRAME_POINTER
KBUILD_CFLAGS	:= $(KBUILD_CFLAGS:-fomit-frame-pointer=)
endif

# Select CPU dependent flags.  Note that order of declaration is important;
# the options further down the list override previous items.

ifdef CONFIG_CPU_BIG_ENDIAN
KBUILD_CFLAGS   += -mbig-endian
KBUILD_AFLAGS	+= -mbig-endian
LINKFLAGS	+= -mbig-endian
KBUILD_LDFLAGS	+= -mbig-endian
ifndef CONFIG_TI_C6X_LINKER
LDFLAGS	+= -EB
endif
endif

KBUILD_CFLAGS	+= $(arch-y)
KBUILD_AFLAGS	+= $(arch-y)
KBUILD_IMAGE    += vmlinux

ifndef CONFIG_TI_C6X_LINKER
boot            := arch/c6x/boot
endif
head-y		:= arch/c6x/kernel/head.o
core-y		+= arch/c6x/kernel/ arch/c6x/mm/ arch/c6x/platforms/ arch/c6x/drivers/
libs-y		+= arch/c6x/lib/

ifdef CONFIG_TI_C6X_LINKER
TI_LDSCRIPT := $(obj)/arch/c6x/vmlinux.cmd
BUILTINSCRIPT := $(srctree)/arch/c6x/builtin.cmd
export BUILTINSCRIPT

.tmp_vmlinux1: $(TI_LDSCRIPT)

$(TI_LDSCRIPT): $(LDSCRIPT)
	cp $< $@

cmd_vmlinux__ = $(LD) $(LDFLAGS) $(LDFLAGS_vmlinux) -o $@ \
	$(vmlinux-init) \
	--start-group $(vmlinux-main) --end-group \
	$(LIBGCC) \
	$(obj)/arch/c6x/lib/lib.a $(obj)/lib/lib.a \
	$(filter-out $(vmlinux-lds) $(TI_LDSCRIPT) $(vmlinux-init) $(vmlinux-main) vmlinux.o FORCE ,$^)

cmd_vmlinux-modpost = $(LD) $(LDFLAGS) $(BUILTINSCRIPT) -r -o $@         \
	 $(vmlinux-init) --start-group $(vmlinux-main) --end-group       \
	 $(filter-out $(vmlinux-init) $(vmlinux-main) FORCE ,$^)
endif

#CLEAN_FILES =
MRPROPER_FILES = arch/c6x/kernel/c6x_config.inc arch/c6x/kernel/c6x_defs.inc
ifdef CONFIG_TI_C6X_LINKER
MRPROPER_FILES += arch/c6x/vmlinux.cmd arch/c6x/vmlinux.map
endif

ifndef CONFIG_TI_C6X_LINKER
OBJCOPYFLAGS:= -O binary -R .note -R .comment -R .GCC-command-line -R .note.gnu.build-id -S

CLEAN_FILES += $(boot)/zImage
CLEAN_FILES += $(boot)/zImage.bin
CLEAN_FILES += $(boot)/vmlinux.bin
CLEAN_FILES += $(boot)/compressed/vmlinux
CLEAN_FILES += $(boot)/compressed/vmlinux.bin
CLEAN_FILES += $(boot)/compressed/vmlinux.bin.gz

PHONY += vmlinux.bin zImage zImage.bin

all: vmlinux

# Convert bzImage to zImage
bzImage: zImage

BOOT_TARGETS = vmlinux.bin zImage zImage.bin

$(BOOT_TARGETS): vmlinux
	$(Q)$(MAKE) $(build)=$(boot) $(boot)/$@

compressed: zImage

archclean:
	$(Q)$(MAKE) $(clean)=$(boot)

define archhelp
	@echo '* vmlinux         - Create an uncompressed kernel image'
	@echo '  vmlinux.bin     - Create an uncompressed binary image (arch/$(ARCH)/boot/vmlinux.bin)'
	@echo '  zImage          - Compressed kernel image (arch/$(ARCH)/boot/zImage)'
	@echo '  zImage.bin      - Compressed binary image (arch/$(ARCH)/boot/zImage.bin)'
endef
endif
