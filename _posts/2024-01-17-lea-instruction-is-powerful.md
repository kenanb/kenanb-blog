---
layout: post
title:  "LEA Instruction is Powerful"
date:   2024-01-17 17:00:00 +1300
categories: code low-level
published: true
---

A quick note on the LEA instruction of x86 family.

[ *Check out all posts in "low-level" series [here][series-low-level].* ]

[series-low-level]: {% link series.md %}#low-level

In my [previous post on low-level code][int-prev], we deconstructed a few assembly patterns. There were a few `lea` instructions in the example assembly. I figured I can share a few links on why `lea` is interesting.

Here is one occurrance of it:
```
...
lea   rax, [var_11h]
mov   rdi, rax
call  sym.Foo::RandChar
...
```

For this discussion, let's get a more raw disassembly of that part.

Below command is for `binutils` version of `objdump` (`this_is_rdi` being the executable):
``` shell
objdump --disassemble=main -C -M intel this_is_rdi
```

This is the relevant part of the output:
``` nasm
...
lea    rax,[rbp-0x9]
mov    rdi,rax
call   11fc <Foo::RandChar()>
...
```

You can check out [the definition of `lea` here][felix-x86-lea]. Basically, it stores the "effective address" computed in a register, rather than accessing the data at the calculated address.

We [already discussed][int-prev] that `rbp` stores a pointer to the base of the stack frame. What this `lea` did is to get the address stored in `rbp`, subtract `0x9` from that, and write the result in rax. So `rax` now stores the memory address of some data on stack.[^1]

This kind of address calculation can be called the main function of `lea`. However, an address is a number, so `lea` can also be used for some generic, combined arithmetic operations. And that use does often show up in optimized generated code.

Take this example:
``` nasm
lea    r8d,[rcx+rcx*2]
```

Or same instruction, in `att` syntax:
```
leal	(%rcx,%rcx,2), %r8d
```

As it should be clear from the `intel` syntax, the instruction is used to "multiply the value stored in rcx by 3", and write the result to `r8d`. In this case, the value that was originally stored in `rcx` was not an address at all. You can find:
- a brief description of this use [in this StackOverflow answer][so-leal-brief].
- a detailed explanation [in this other answer][so-leal-detail].

Some more details on the subtleties around `lea` instruction is available in the "16.1 LEA instruction (all processors)" section of Agner Fog's manual called: "Optimizing subroutines in assembly language: An optimization guide for x86 platforms". The manual [can be found in this page][agner-optimize]. It states:

> The LEA instruction is useful for many purposes because it can do a shift operation, two
additions, and a move in just one instruction.

That's all for today. If you find technical errors, please report in the blog's [Issues page][report].

Thanks for reading!

[^1]: To remind what we already explained [in a previous post][int-this-is-rdi]: The `mov` will copy that address to `rdi`. The copy in `rdi` is "the `this` pointer" being passed as an implicit argument to the `call` to `Foo::RandChar`.

[report]: https://github.com/kenanb/kenanb-blog/issues
[int-prev]: {% post_url 2024-01-16-deconstructing-generated-assembly %}
[int-this-is-rdi]: {% post_url 2024-01-14-debugging-tip-this-is-usually-rdi %}
[felix-x86-lea]: https://www.felixcloutier.com/x86/lea
[so-leal-brief]: https://stackoverflow.com/a/11212925
[so-leal-detail]: https://stackoverflow.com/a/46597375
[agner-optimize]: https://www.agner.org/optimize/#manuals
