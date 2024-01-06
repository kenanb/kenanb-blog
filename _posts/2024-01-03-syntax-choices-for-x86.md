---
layout: post
title:  "Syntax Choices for x86 Assembly"
date:   2024-01-03 16:00:00 +1300
categories: code low-level
published: true
---

We will see how Intel syntax can be used instead of AT&T with GAS, and talk about the differences.

In previous post, [we compiled a few lines of assembly][int-asm-1], using GAS (GNU Assembler) in Linux.

[ *Check out all posts in "low-level" series [here][series-low-level].* ]

[series-low-level]: {% link series.md %}#low-level

GAS, by default, uses the AT&T syntax. There are two major syntax branches for x86: AT&T, and Intel.

Let's see how the same code looks in Intel syntax.[^1]

[Here is the Wikipedia entry][wikipedia-syntax] describing the main differences.

 I haven't written enough assembly to have a preference. I more often read assembly for debugging, rather than writing it. So I find it beneficial to roughly understand both syntaxes.

GAS supports both. [Here is an SO answer that describes how to use Intel syntax][so-gas-intel-syntax], both in assembly and C/C++ contexts. There is also a nice set of links in the "Related" section.

In order to reuse the makefile from the previous post, I will use the `.intel_syntax noprefix` directive for enabling Intel sytax in GAS.

x86 version of the code:
```
    .intel_syntax noprefix
    .section .data

    .section .text
    .global _start
_start:
    mov eax, 1
    mov ebx, 0
    int 0x80
```

So `movl $1, %eax` becomes `mov eax, 1`.

The changes we see in this one line:
- Destination (`eax`) before source (`1`).
- No `%` prefix (sigil) to register name, and no `$` prefix to the integer literal.
- No `l` (`long` or `dword`) suffix in the mnemonic. Parameter size is derived from the register name instead: `eax`. ( See [this][wikipedia-x86-32bit-reg] and [this][wikibooks-x86-gpr] for details on x86 register names.)

There are other syntax differences (effective addresses etc.) that don't show up in this minimal example.

And the x64 version of the code:
```
    .intel_syntax noprefix
    .section .data

    .section .text
    .global _start
_start:
    mov rax, 60
    mov rdi, 0
    syscall
```

[^1]: I am not very experienced in assembly. If you find errors, please report in the blog's [Issues page][report].
[report]: https://github.com/kenanb/kenanb-blog/issues
[int-asm-1]: {% post_url 2024-01-02-minimal-x86-64-binary %}
[wikipedia-syntax]: https://en.wikipedia.org/wiki/X86_assembly_language#Syntax
[so-gas-intel-syntax]: https://stackoverflow.com/a/9347957
[wikipedia-x86-32bit-reg]: https://en.wikipedia.org/wiki/X86#32-bit
[wikibooks-x86-gpr]: https://en.wikibooks.org/wiki/X86_Assembly/X86_Architecture#General-Purpose_Registers_(GPR)_-_16-bit_naming_conventions
