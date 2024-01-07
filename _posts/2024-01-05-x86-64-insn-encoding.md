---
layout: post
title:  "x86-64 Instruction Encoding"
date:   2024-01-05 16:00:00 +1300
categories: code low-level
published: true
---

We will check out the encoding of the instructions we authored in x86-64 assembly.

So far, we:
- [Compiled a few lines of assembly][int-asm-1], using GAS (GNU Assembler) in Linux.
- [Switched to Intel syntax][int-asm-2] from AT&T with GAS, and talked about the differences.
- [Figured out the encoding of the x86 instructions][int-asm-3] we authored in the 32 bit executable.

[ *Check out all posts in "low-level" series [here][series-low-level].* ]

[series-low-level]: {% link series.md %}#low-level

Here is the `objdump` of the instructions in the 64 bit object file:

``` shell
objdump -D --disassembler-options=intel _prog_g64.o
```

The relevant part of the output:
```
_prog_g64.o:     file format elf64-x86-64


Disassembly of section .text:

0000000000000000 <_start>:
   0:	48 c7 c0 3c 00 00 00 	mov    rax,0x3c
   7:	48 c7 c7 00 00 00 00 	mov    rdi,0x0
   e:	0f 05                	syscall

...
```

So these look completely different. But we can already see the same patterns emerge for the first 2 instructions.
```
   probably related
   to register code
    %rax=0, %rdi=7
       _/
48 c7 c0 3c 00 00 00 	mov    rax,0x3c
48 c7 c7 00 00 00 00 	mov    rdi,0x0
      |  ___________
      |             \
      ?       second operand (imm32)
```

Focusing on the first instruction:
- `48`: 1st byte is the [REX prefix][osdev-x86-64-insn-enc-rex]. This value has [`REX.W` flag set][x86-64-insn-ref-48], which ensures the instruction uses 64 bit operands.
- `C7`: 2nd byte seems to be the primary opcode of [the MOV variant that we need][x86-64-insn-ref-c7].
- `C0`: 3rd byte, in this particular instruction, is called the [`ModR/M` byte][osdev-x86-64-insn-enc-modrm]. It exists in many instructions, and it has roles like, storing register code for an operand, defining addressing mode etc.

If you check out the layout of ModR/M, it is yet another byte that makes more sense when written in octal. Let's rewrite that byte based on the information [here][osdev-x86-64-insn-enc-modrm]:
```
                _ opcode or register ?
               /
              /
ModR/M : 11 000 000
          \
           \_ register addressing
```

Regarding the question of `opcode or register`, let's checkout the instruction table of [MOV (`c7`)][x86-64-insn-ref-c7] again. The table has a column called `o` which is `0` for our entry. This is the "opcode extension" value. So those 3 bits are part of the opcode.

As you will remember from the previous post, we expect to see the register codes represented somewhere, each using 3 bits. In fact, it is slightly different in x86-64, because the number of general purpose registers are doubled. So we need 1 more bit to represent each register encoded in the instruction. It is the REX prefix that stores these extra bits.

For this instruction, the register is stored as a combination of least significant 3 bits of ModR/M (called `ModR/M.rm`) and least significant bit of REX (called `REX.B`). The register code for `rax` is `0`.
```
                _ C7 opcode extension
               /   _ register code: %rax
              /   /
ModR/M : 11 000 000
          \
           \_ register addressing
```

And in the case of second instruction, ModR/M byte (`C7`) looks like this:
```
                _ C7 opcode extension
               /   _ register code: %rdi
              /   /
ModR/M : 11 000 111
          \
           \_ register addressing

```

The last instruction is this:
```
0f 05                	syscall
```

There is not much to decode in this one. This is a [2-byte opcode][osdev-x86-64-insn-enc-opcode] listed [here][x86-64-insn-ref-0F05] as `syscall`.

My understanding is, what these (REX, MomR/M, SIB) byte slots represents is instruction-dependent. And there are is also a newer VEX encoding in x86-64 architecture. But this is all documented in the other resources, and I am not too familiar with VEX yet. So I won't try to detail instruction encoding any further.

[^1]: I am not very experienced in assembly. If you find errors, please report in the blog's [Issues page][report].

[report]: https://github.com/kenanb/kenanb-blog/issues

[int-asm-1]: {% post_url 2024-01-02-minimal-x86-64-binary %}
[int-asm-2]: {% post_url 2024-01-03-syntax-choices-for-x86 %}
[int-asm-3]: {% post_url 2024-01-04-x86-insn-encoding %}
[osdev-x86-64-insn-enc-rex]: https://wiki.osdev.org/X86-64_Instruction_Encoding#REX_prefix
[osdev-x86-64-insn-enc-modrm]: https://wiki.osdev.org/X86-64_Instruction_Encoding#ModR.2FM
[osdev-x86-64-insn-enc-opcode]: https://wiki.osdev.org/X86-64_Instruction_Encoding#Opcode_2
[x86-64-insn-ref-48]: http://ref.x86asm.net/coder64.html#x48
[x86-64-insn-ref-c7]: http://ref.x86asm.net/coder64.html#xC7
[x86-64-insn-ref-0F05]: http://ref.x86asm.net/coder64.html#x0F05
