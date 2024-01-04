---
layout: post
title:  "Low-level: x86 Instruction Encoding"
date:   2024-01-04 13:00:00 +1300
categories: code low-level
published: true
---

We will check out the binary encoding of the x86 instructions we authored in assembly, just for fun.

So far, we:
- [Compiled a few lines of assembly][int-asm-1], using GAS (GNU Assembler) in Linux.
- [Switched to Intel syntax][int-asm-2] from AT&T with GAS, and talked about the differences.

x86 is a [CISC ISA family][wikipedia-x86-overview] that has seen many extensions since it was first introduced in late 70s. And these extensions were mostly done in a backwards compatible way. **The encoding is neither trivial, nor useful for a programmer.**

We will just walk through this small example, trying to figure out the encoding, and observe some of the complexities. This exercise is more like a geek puzzle, unless you are planning to write a disassembler.

Now, let's look at the result of assembling these instructions. [^1]

We can `objdump` the 32 bit object file, or the executable file. Both will contain the same pieces of machine code, just in different offsets.

``` shell
objdump -D --disassembler-options=intel _prog_g32.o
```

The relevant part of the output:
```
_prog_g32.o:     file format elf32-i386


Disassembly of section .text:

00000000 <_start>:
   0:	b8 01 00 00 00       	mov    eax,0x1
   5:	bb 00 00 00 00       	mov    ebx,0x0
   a:	cd 80                	int    0x80

...
```
OK, but what are we looking at?

```
b8 01 00 00 00          mov    eax,0x1
______________          ___    _______
              \            \          \
       machine code      assembly  instruction
       for instruction   mnemonic    operands
```

What do we know so far:
- The encoded instruction is 5 bytes long.
- We see a `B8` at the beginning.

## Finding the Instruction

We can quickly check this online [x86 Instruction Reference][x86-insn-ref-index] to see if we will find the instruction.

Here is the link to [opcode `B8`][x86-insn-ref-coder-b8]. It says:
```
Primary opcode : B8+r
Mnemonic       : MOV
Operand 1      : r16/32
Operand 2      : imm16/32
Description    : Move
```

This generally seems to match what we expect.

We know our destination is `eax` which implies a 32 bit (DWORD) operation on an `r32` (register). And the second operand (`0x1`) is an `imm32` (as in immediate).

But we actually got lucky. Instructions often have prefixes. So just searching for bytes won't work without a good general understanding of the encoding.

This reference also has the alphabetical mnemonic list, where you can see all instructions that correspond to given mnemonic. [Here is `M`][x86-insn-ref-coder-abc-m].

Another convenient reference is these [unofficial x86 and amd64 instruction reference][felix-index] pages. The list here is based on the mnemonics.
Here is the page for [MOV][felix-mov], including all instructions that correspond to a MOV.

There are a few things we haven't answered yet:
- Where is the value of immediate stored?
- Where do we store the fact that the register is specifically `EAX`?
- What's `+r` in `B8+r`?

## Decoding the Instruction

I find the OSDev page on [X86-64 Instruction Encoding][osdev-x86-insn-enc] to be a pretty concise reference. But if you skim that page, you will see that there are various encodings, with a bunch of optional prefixes etc.[^2]

So let's first focus on the immediate. That sounds straightforward.

### Encoding of the Immediate

According to the [index page][x86-insn-ref-index]:
- `imm` : Immediate data. The operand value is encoded in subsequent bytes of the instruction.

So we at least know that the value `0x1` should be in those 5 bytes. And it is supposed to be 32 bits, so that's probably the last 4 bytes.

```
b8 01 00 00 00
   ___________
              \
       immediate: 0x1
```

Note that in this binary dump, byte at the smallest address is printed first (leftmost).

Since [x86 is little-endian][wikipedia-endianness], the least significant byte of the immediate (`0x01`) is expected to be at the smaller address.

At home, I usually use [rizin (FOSS Reverse Engineering Framework forked from radare2)][rizin] for low level stuff. `rz-asm` allows me to quickly test variations of an instruction:

``` shell
rz-asm -a x86 -b 32 'mov eax, 0x00000001'
```
outputs, as expected:
```
b801000000
```

If we try to assembly with a richer immediate:
``` shell
rz-asm -a x86 -b 32 'mov eax, 0xDDCCBBAA'
```
outputs:
```
b8aabbccdd
```

Yep, that's more obvious.

Cool, so we know that the last 4 bytes are for the immediate. We have a single byte left to focus on.

### Encoding of the Opcode

All remaining information must be part of the opcode. We had that `B8+r` syntax.

The [index page][x86-insn-ref-index] of instruction reference also states: [^3]
- `+r` in primary opcode: A register code, from 0 through 7, added to the value.

And here is an [online table of the register codes][osdev-registers]. The codes are defined in `X.Reg` form e.g `0.010`. For 32-bits, ignore the `X` part. We can see that `EAX` corresponds to `0` (`0b000`). So this is why the encoding was `B8`, because the value that was added to encode the register happened to be `0`.

## Remaining Instructions

We can further confirm the above finding, because the next instruction happens to be a similar move operation:
```
bb 00 00 00 00       	mov    ebx,0x0
```

Again, 32 bit operands. So we can guess that `BB` is the opcode, and `00 00 00 00` is the immediate. `BB` is `B8+3` and we can confirm from the table that `EBX` is `3`.

But wait, how did it know that the operands are 32 bit (DWORD) size? Why is it `EAX`, and not `AX` (16-bit), which shares the same register code? Same instruction is documented to handle both `r16/32`.

### Operand Sizes

Because that's the **default operand size** in 32 bit code.[^4]

Encoding a 16-bit operand [requires a `0x66` prefix][encoding-real-x86-insn-16-bit] in this scheme.

We can confirm this by trying to assemble the instruction with an `AX` operand:
``` shell
rz-asm -a x86 -b 32 'mov ax, 0x01'
```
outputs:
```
66b80100
```
As expected. Also, since we reduced the operand size, the immediate is now also encoded as: `01 00` instead of `01 00 00 00`.

### The Interrupt

Lastly, we have an `interrupt` instruction:
```
cd 80                	int    0x80
```

This one seems easy. `80` is clearly an 8 bits (BYTE) encoded immediate. And there is no general purpose register being passed.

Here is the link to the [opcode `CD`][x86-insn-ref-coder-cd]. Surprisingly, there is a second operand: EFLAGS!

Wikipedia [FLAGS register][wikipedia-flags-reg] page states that the state of FLAGS register may modify interrupt behaviour. So that makes sense.

There is only a single flags register, so there is no information about it to explicitly encode.

## Octal System

Clearly, even the encoding of opcodes are non-trivial. But it turns out the Hex radix we use to look at opcodes is making it even more difficult.

Mark Hopkins has an old post (from 1995?) called The 80x86 is an Octal Machine [that's copied here][x86-octal].

The post states:
> The only proper way to understand 80x86 coding is to realize that ALL 80x86 OPCODES ARE CODED IN OCTAL.
> A byte has 3 octal digits, ranging from 000 to 377.  In fact, each octal group (000-077, 100-177, etc.)
> tends to encode a specific variety of operation.  All of these are features inherited from the 8080/8085/Z80.

And if we look at B8 and BB in octal encoding, we see:
``` shell
printf "0o%o\n" 0xB8 0xB9 0xBA 0xBB 0xBC 0xBD 0xBE 0xBF
```
```
0o270
0o271
0o272
0o273
0o274
0o275
0o276
0o277
```

So,the opcode is basically octal `27r`, where `r` is the "register code".

## Conclusion

As we can see, it is not easy, but there is no magic.

I provided links to some common resources, and tried to describe in which context they are relevant. But the ultimate references of this topic are Intel's [Software Developer's Manual][intel-sdm] and AMD's [AMD64 Architecture Programmer's Manual][amd-apm] volumes.

In the next post, we will check out the x86-64 binary, but that one will be shorter.

Thanks for reading!

[^1]: I am not very experienced in assembly. If you find errors, please report in the blog's [Issues page][report].
[^2]: In fact, a lot of the encoding complexity comes from 64-bit instruction set. When looking at an 32-bit executable, [this document on x86 encoding][encoding-real-x86-insn] is an easier reference.
[^3]: The reference has a [geek mode][x86-insn-ref-geek-b8], which is a bit more cryptic, but also more precise.
[^4]: Default operand size is 32 bits for x86. And when code targeting x86 is run in x86-64, it runs in ["compatibility mode", which uses x86 encoding][wikipedia-x86-64-opmodes]. So the operand size is, again, 32 bits. In fact, even in 64-bit mode, default operand size is still 32 bits, while the address size is 64 bits.

[report]: https://github.com/kenanb/kenanb-blog/issues
[encoding-real-x86-insn]: http://www.c-jump.com/CIS77/CPU/x86/lecture.html
[wikipedia-x86-64-opmodes]: https://en.wikipedia.org/wiki/X86-64#OPMODES
[int-asm-1]: {% post_url 2024-01-02-minimal-x86-64-binary %}
[int-asm-2]: {% post_url 2024-01-03-syntax-choices-for-x86 %}
[wikipedia-x86-overview]: https://en.wikipedia.org/wiki/X86#Overview
[osdev-x86-insn-enc]: https://wiki.osdev.org/X86-64_Instruction_Encoding
[x86-insn-ref-index]: http://ref.x86asm.net/index.html
[x86-insn-ref-coder-b8]: http://ref.x86asm.net/coder32.html#xB8
[x86-insn-ref-coder-cd]: http://ref.x86asm.net/coder32.html#xCD
[x86-insn-ref-coder-abc-m]: http://ref.x86asm.net/coder32-abc.html#M
[x86-insn-ref-geek-b8]: http://ref.x86asm.net/geek32.html#xB8
[felix-index]: https://www.felixcloutier.com/x86/
[felix-mov]: https://www.felixcloutier.com/x86/mov
[encoding-real-x86-insn-16-bit]: http://www.c-jump.com/CIS77/CPU/x86/lecture.html#X77_0230_operands_16
[wikipedia-endianness]: https://en.wikipedia.org/wiki/Endianness
[rizin]: https://rizin.re/
[osdev-registers]: https://wiki.osdev.org/X86-64_Instruction_Encoding#Registers
[wikipedia-flags-reg]: https://en.wikipedia.org/wiki/FLAGS_register
[x86-octal]: https://gist.github.com/seanjensengrey/f971c20d05d4d0efc0781f2f3c0353da
[bpetkov-x86-insn-encoding]: https://www.youtube.com/watch?v=CUAXCeRjw3c
[intel-sdm]: https://www.intel.com/content/www/us/en/developer/articles/technical/intel-sdm.html
[amd-apm]: https://www.amd.com/en/search/documentation/hub.html#q=AMD64%20Architecture&sortCriteria=%40amd_release_date%20descending&f-amd_product_type=Processors&f-amd_document_type=Programmer%20References
