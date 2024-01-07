---
layout: post
title:  "Some x86 Instruction Encoding Resources"
date:   2024-01-07 20:00:00 +1300
categories: code low-level
published: true
---

Just sharing a few useful resources, relating to x86-64 instruction encoding.

[ *Check out all posts in "low-level" series [here][series-low-level].* ]

[series-low-level]: {% link series.md %}#low-level

As of [my previous post][int-asm-5], I wrapped the topic of x86-64 instruction encoding, for now.

We used a bunch of resources in the previous posts. I am listing them (and a few more) here for reference:
- [Wikipedia - X86][wikipedia-x86] : Good overview, and details on register extensions.
- [Wikipedia - X86-64 : Operating modes][wikipedia-x86-64-opmodes] : Operand size defaults of various modes listed.
- [StackOverflow X86 Tag Wiki][so-x86-tag-wiki] : Nice set of learning resources.
- [Encoding Real X86 Instructions][encoding-real-x86-insn] : A good resource on x86 (32 bit) encoding.
- [OSDev - X86-64 Instruction Encoding][osdev-x86-insn-enc] : Very good resource on x86-64 encoding.
- Online [X86 Opcode and Instruction Reference][x86-insn-ref-index] : Great resource, separate pages for 32 and 64 bit, and separate pages for coder (more brief info), geek (complete info), along with alphabetical tables (abc).
- Another [Unofficial X86 and AMD64 Instruction Reference][felix-index] : Great for looking up all info about a mnemonic.
- [Really good talk on X86-64 instruction encoding][bpetkov-x86-insn-encoding] by Borislav Petkov.
- Intel's [Software Developer's Manuals][intel-sdm] : The ultimate guides on the topic. Volume 2 is the Instruction Set Reference.
- AMD's [AMD64 Architecture Programmer's Manuals][amd-apm] : Volume 3 has Instruction Encoding and General-Purpose Instruction Reference.
- Agner Fog's [Software optimization resources][agner-optimize] : One of the most highly respected set of resources on optimization and x86 hardware details.
- [Standord CS107 - Guide to x86-64][stanford-cs107-guide-to-x86-64] : Nice and compact guide, that includes topics like addressing modes.
- [x86 Instruction Encoding Revealed: Bit Twiddling for Fun and Profit][codeproject-x86-instruction-encoding-revealed] : This one seems to go into details on ModR/M and SIB, which is cool.
- [X86_64 Instruction Encoding - Compiler Programming Ep4][dmitriy-x86-64-insn-encoding] : Looks like a nice video walkthrough of the encoding, referencing the relevant resources. (I only skimmed through it so far.)

Also, when I look for reliable and focused information on a specific x86 topic, I first see if I will get lucky finding a StackOverflow answer from Peter Cordes. [Here is an example.][so-addressing-modes]

I will keep adding to this list as I encounter new resources.

[int-asm-5]: {% post_url 2024-01-06-encoding-diagram-attempt %}

[wikipedia-x86]: https://en.wikipedia.org/wiki/X86
[wikipedia-x86-64-opmodes]: https://en.wikipedia.org/wiki/X86-64#OPMODES
[so-x86-tag-wiki]: https://stackoverflow.com/tags/x86/info
[encoding-real-x86-insn]: http://www.c-jump.com/CIS77/CPU/x86/lecture.html
[osdev-x86-insn-enc]: https://wiki.osdev.org/X86-64_Instruction_Encoding
[x86-insn-ref-index]: http://ref.x86asm.net/index.html
[felix-index]: https://www.felixcloutier.com/x86/
[bpetkov-x86-insn-encoding]: https://www.youtube.com/watch?v=CUAXCeRjw3c
[intel-sdm]: https://www.intel.com/content/www/us/en/developer/articles/technical/intel-sdm.html
[amd-apm]: https://www.amd.com/en/search/documentation/hub.html#q=AMD64%20Architecture&sortCriteria=%40amd_release_date%20descending&f-amd_product_type=Processors&f-amd_document_type=Programmer%20References
[agner-optimize]: https://www.agner.org/optimize/
[stanford-cs107-guide-to-x86-64]: https://web.stanford.edu/class/cs107/guide/x86-64.html
[codeproject-x86-instruction-encoding-revealed]: https://www.codeproject.com/Articles/662301/x86-Instruction-Encoding-Revealed-Bit-Twiddling-fo
[dmitriy-x86-64-insn-encoding]: https://www.youtube.com/watch?v=5FCgOFbKHYU
[so-addressing-modes]: https://stackoverflow.com/a/34058400
