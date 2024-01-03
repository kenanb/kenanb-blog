---
layout: post
title:  "Assembly: Compiling a Minimal x64 Binary"
date:   2024-01-02 21:00:00 +1300
categories: code assembly
published: true
---

I was reading [Programming from the Ground Up][pgu-book] by Jonathan Bartlett some time ago. ( It is good. )

It is based on x86, Linux, with GNU toolchain.

The first example is a program that just exits with a specific status code, and it looks like this:

```
        .section .data

        .section .text
        .global _start
_start:
        movl $1, %eax
        movl $0, %ebx
        int $0x80
```

"Exiting" involves making a system call. So the instructions (following `_start`) are about setting up the inputs to the call, and executing it.

Let's look at the instructions line-by-line:

`%eax` : Holds the system call number for x86.
``` asm
movl $1, %eax  # System call number for sys_exit: 1
```

`%ebx` : Holds the return status ( first syscall param for x86 calling conventions ).
``` asm
movl $0, %ebx  # Exit status number: 0
```

Interrupt to notify kernel to make the system call.
``` asm
int $0x80      # Deprecated ( and probably linux specific ).
```

Now, this seems to still work when we build an x86_64 executable.

However, [there is a better way][syscall] in x86_64 world for making system calls: `syscall` instruction.

(Other [fast entry instructions][sysenter] exist for x86 and x86_64, but I will omit them.)

Let's modify to get that version:

```
        .section .data

        .section .text
        .global _start
_start:
        mov $60, %rax
        mov $0, %rdi
        syscall
```

As you can see, every line is different. Here is the explanation:

`%rax` : Holds the system call number for syscall in x86_64.
```
mov $60, %rax # System call number for sys_exit in x86_64 is different: 60
```

`%rdi` : Holds the return status ( first syscall param for x86_64 calling conventions ).
```
mov $0, %rdi  # Exit status number: 0
```

Portable x86_64 insn for fast system call:
```
syscall
```

(And nowadays there is an [even more optimized way][vdso] to make system calls. I might talk about it in a later post.)

Here is the makefile to build both versions:
``` makefile
# Shell replacement to print the rule being run.
OLD_SHELL := $(SHELL)
SHELL := $(warning Building $@$(if $<, (from $<))$(if $?, ($? newer)))$(OLD_SHELL)

DEPS   =
OBJ    = prog
LD     = ld

.PRECIOUS:  %.s _%.o
_%_g32.o    : %_g32.s $(DEPS)               ; @echo "RULE> $@ : $^" ; as -o $@ $< --32
_%_g64.o    : %_g64.s $(DEPS)               ; @echo "RULE> $@ : $^" ; as -o $@ $< --64
prog_g32    : $(patsubst %,_%,$(OBJ)_g32.o) ; @echo "RULE> $@ : $^" ; $(LD) $^ -o $@ -m elf_i386
prog_g64    : $(patsubst %,_%,$(OBJ)_g64.o) ; @echo "RULE> $@ : $^" ; $(LD) $^ -o $@ -m elf_x86_64 -pie -static -no-dynamic-linker
```

Below will make the i386 binary:
```
make prog_g32
```

And to make the x86_64 binary:
```
make prog_g64
```

After running the program, you can `echo $?` to print the status number returned from the previous command. Both should return "0".


[pgu-book]: https://savannah.nongnu.org/projects/pgubook/
[x86-call-table]: https://chromium.googlesource.com/chromiumos/docs/+/HEAD/constants/syscalls.md#x86-32_bit
[x64-call-table]: https://chromium.googlesource.com/chromiumos/docs/+/HEAD/constants/syscalls.md#x86_64-64_bit
[syscall]: https://en.wikibooks.org/wiki/X86_Assembly/Interfacing_with_Linux#Via_dedicated_system_call_invocation_instruction
[sysenter]: https://wiki.osdev.org/SYSENTER
[vdso]: https://en.wikipedia.org/wiki/VDSO
