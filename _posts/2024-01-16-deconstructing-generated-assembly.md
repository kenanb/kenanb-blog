---
layout: post
title:  "Deconstructing Generated Assembly"
date:   2024-01-16 22:00:00 +1300
categories: code low-level
published: true
---

Deconstructing the assembly resulting from example C++ code we compiled last time.

[ *Check out all posts in "low-level" series [here][series-low-level].* ]

[series-low-level]: {% link series.md %}#low-level

Recently, I shared a tip about [`this` pointer][int-prev]. It included the disassembly of a small program we compiled. Today, I will elaborate on various parts of that disassembly. To check out the C++ code that generated this, refer  to the previous post.

## Reference

First of all, a reminder of the reference document.

As we mentioned before, System V ABI is used on Linux x86-64. The ABI consists of a "generic ABI" document, and various "processor supplements".

The "System V ABI - AMD64 Architecture Processor Supplement" is the document I often refer to.

Calling conventions are documented in the processor supplement. I am not certain where the "official" up-to-date source is, but I get the latest psABI (processor supplement ABI) PDF from [this gitlab repo][gitlab-x86-psABIs-x86-64-ABI].[^1]

## Disassembly

Here is what `main` looked like:

```
 .-----------------------------------------------.
 |  0x5563e81b9179                               |
 | int main(int argc, char **argv, char **envp); |
 | ; var int64_t var_11h @ stack - 0x11          |
 | ; var int64_t var_10h @ stack - 0x10          |
 | push  rbp                                     | 00
 | mov   rbp, rsp                                | 01
 | sub   rsp, 0x10                               | 02
 | mov   rax, qword fs:[0x28]                    | 03
 | mov   qword [var_10h], rax ; local.set 1      | 04
 | xor   eax, eax                                | 05
 | lea   rax, [var_11h] ; local.get 1            | 06
 | mov   esi, 0x40                               | 07
 | mov   rdi, rax                                | 08
 | call  sym.Foo::Foo_char                       | 09
 | lea   rax, [var_11h] ; local.get 1            | 10
 | mov   rdi, rax                                | 11
 | call  sym.Foo::RandChar                       | 12
 | movsx eax, al                                 | 13
 | mov   esi, eax                                | 14
 | lea   rax, str.Generated_char_is:__c          | 15
 | mov   rdi, rax                                | 16
 | mov   eax, 0                                  | 17
 | call  sym.imp.printf                          | 18
 | mov   eax, 0                                  | 19
 | mov   rdx, qword [var_10h] ; local.get 1      | 20
 | sub   rdx, qword fs:[0x28]                    | 21
 | je    0x5563e81b91df                          | 22
 '-----------------------------------------------'
       t f
       | |
    .--' |
    |    '------------------.
    |                       |
.------------------.    .--------------------------------.
|  0x5563e81b91df  |    |  0x5563e81b91da                |
| leave            | 23 | call  sym.imp.__stack_chk_fail | 25
| ret              | 24 '--------------------------------'
'------------------'
```

Before we proceed: `rizin`'s output replaces memory references with variable names.

In the actual disassembly, here are the important differences:
- `var_10h` corresponds to `rbp-0x8`
- `var_11h` corresponds to `rbp-0x9`
- `str.Generated_char_is:__c` is `[rip+0xe4b]` (RIP-relative)

Above is easier to read, so I am keeping the variable names.

Let's deconstruct this.

## Prolog and Epilog

First section is what we call the "prolog".

```
 .-----------------------------------------------.
 |  0x5563e81b9179                               |
 | int main(int argc, char **argv, char **envp); |
 | ; var int64_t var_11h @ stack - 0x11          |
 | ; var int64_t var_10h @ stack - 0x10          |
 ---                                    PROLOG ---
 | push  rbp                                     | 00
 | mov   rbp, rsp                                | 01
                       ...
```

This is a very generic pattern. The existing frame pointer value is pushed to stack, then the current stack pointer is copied to `rbp`. So then `rbp` points to the "base" of the "stack frame" of the function, within the context of that function.

There is an [`enter` instruction][felix-x86-enter] that applies the same changes, and does a bit more, but [apparently it is really slow, compared to the usual prolog][so-question-enter-leave]. So gcc generates a `push` and `mov`.

And there is a corresponding "epilog" in the end (in the `true` branch target of the jump instruction).
```
                       ...
 '-----------------------------------------------'
       t f
       |
    .--'
    |
    |
.------------------.
|  0x5563e81b91df  |
---       EPILOG ---
| leave            | 23 
        ...
```

Epilog is normally the reverse of prolog:
``` nasm
mov rsp, rbp
pop rbp
```

But there is a corresponding [`leave` instruction][felix-x86-leave], and this one seems to be fast enough that gcc choose to generate a `leave` as epilog.

## Frame Pointer Details

Check out "Function Calling Sequence" (section 3.2) in the processor supplement.

In "Parameter Passing" (section 3.2.3) describes `rbp` as:
> callee-saved register; optionally used as frame pointer

Note that it states "optional". If we were to enable optimization level one in gcc (`-O1`), it would [enable `-omit-frame-pointer` optimization][gcc-optimize-options], which would allow the compiler to skip juggling the frame pointer when compiler decides it is not needed.

Note that in the case of `main`, the previous value of "frame pointer" is a bit of a special case:

> %rbp : The content of this register is unspecified at process initialization time, but the user
code should mark the deepest stack frame by setting the frame pointer to zero.

## Reserving Space on Stack

Then we have this instruction in the line I marked as `02`:
``` nasm
sub   rsp, 0x10
```

Stack grows down. So by subtracting from the stack pointer, we are reserving the space we need for the local variables of the function `main`.

Near the end of the function, when the epilogue copies the `rbp` to `rsp`, it will essentially mean discarding the space we reserved and used within the function.

## A Visual Description

I am aware that it is not easy to follow what's really happening if you are reading this stuff for the first time. So I looked for pages that give a more visual description of state changes during prolog and epilog. I found these two:
- [This page is part of a series][holbertonschool-vm-stack] about virtual memory. It does a good job of communicating the changes to the stack and relevant registers, and still keeping it simple.
- On a quick look, [this online book chapter][diveintosystems-functions] seems to be a comprehensive tracing of the state changes during the execution of a function, that includes this prolog/epilog.

## Stack Guard

Next, we have these sections surrounding rest of the code.
```
                       ...
 | mov   rax, qword fs:[0x28]                    | 03
 | mov   qword [var_10h], rax ; local.set 1      | 04
                       ...

                       ...
 | mov   rdx, qword [var_10h] ; local.get 1      | 20
 | sub   rdx, qword fs:[0x28]                    | 21
 | je    0x5563e81b91df                          | 22
 '-----------------------------------------------'
       t f
         |
         |
         '----------.
                    |
                .--------------------------------.
                |  0x5563e81b91da                |
                | call  sym.imp.__stack_chk_fail | 25
                '--------------------------------'
```

This logic is called stack protector, or stack guard.

You can read about this [in this SO thread][so-stack-guard], and in the [description of related GCC option][gcc-optimize-stack-protector].

Basically, a special value ("stack canary" value stored at `fs:[0x28]`) is copied to stack first. Near the end of the function, we confirm that it wasn't overwritten. If it was overwritten, we call the "stack check failure" routine. Otherwise, we proceed to epilog and return normally.

Note that the SO discussion explicitly refers to `XOR`. In our example, we see a `SUB` instruction instead. But it should achieve the same result in this case.

## Zeroing a Register

Here is a surprising instruction in the line I marked as `05`:

``` nasm
xor   eax, eax
```

This is effectively going to set `eax` to 0. This interesting way of zeroing a register is chosen, because [it is known to be the best way to do so][so-zeroing].

## Return and the Rest

Here is the parts we haven't discussed yet.

```
                       ...
 | lea   rax, [var_11h] ; local.get 1            | 06
 | mov   esi, 0x40                               | 07
 | mov   rdi, rax                                | 08
 | call  sym.Foo::Foo_char                       | 09
 | lea   rax, [var_11h] ; local.get 1            | 10
 | mov   rdi, rax                                | 11
 | call  sym.Foo::RandChar                       | 12
 | movsx eax, al                                 | 13
 | mov   esi, eax                                | 14
 | lea   rax, str.Generated_char_is:__c          | 15
 | mov   rdi, rax                                | 16
 | mov   eax, 0                                  | 17
 | call  sym.imp.printf                          | 18
 | mov   eax, 0                                  | 19
                       ...

                       ...
 '-----------------------------------------------'
       t f
       |
    .--'
    |
    |
.------------------.
|  0x5563e81b91df  |
        ...
| ret              | 24
'------------------'
```

This involves:
- Setting values on some registers to be used by calls to other functions.
- Making calls and utilizing return values.
- Setting the value to be returned from `main`.
- [Returning from procedure][felix-x86-ret].

The rules of how these are achived is described in "Parameter Passing" section (3.2.3) of Processor Supplement, in detail. But ["System V AMD64 ABI" section][wikipedia-cc-amd64-abi] of "x86 calling conventions" Wikipedia page has a summary that includes the key aspects to remember.

We already detailed one example call [in previous post][int-prev]. It is all the same rules repeated for calls to various functions. So I will skip those parts.

I will just mention that, in the line I marked `19`, we do:
``` nasm
mov   eax, 0
```
This is because `rax` carries the return value in this case.

That's all for today. The source is [shared in blog's repository][source]. If you find technical errors, please report in the blog's [Issues page][report].

Thanks for reading!

[^1]: There is also [an OSDev page][osdev-abi] that has links to related documents. This [SO answer][so-abi-documents] also has links to relevant docs.

[report]: https://github.com/kenanb/kenanb-blog/issues
[int-prev]: {% post_url 2024-01-14-debugging-tip-this-is-usually-rdi %}
[source]: https://github.com/kenanb/kenanb-blog/tree/main/example/2024-01-14-debugging-tip-this-is-usually-rdi

[gcc-optimize-options]: https://gcc.gnu.org/onlinedocs/gcc/Optimize-Options.html#index-O
[gcc-optimize-stack-protector]: https://gcc.gnu.org/onlinedocs/gcc/Instrumentation-Options.html#index-fstack-protector
[gitlab-x86-psABIs-x86-64-ABI]: https://gitlab.com/x86-psABIs/x86-64-ABI
[osdev-abi]: https://wiki.osdev.org/System_V_ABI#Documents
[so-abi-documents]: https://stackoverflow.com/a/40348010
[so-question-enter-leave]: https://stackoverflow.com/q/5959890
[felix-x86-enter]: https://www.felixcloutier.com/x86/enter
[felix-x86-leave]: https://www.felixcloutier.com/x86/leave
[holbertonschool-vm-stack]: https://blog.holbertonschool.com/hack-virtual-memory-stack-registers-assembly-code/
[diveintosystems-functions]: https://diveintosystems.org/book/C7-x86_64/functions.html
[so-stack-guard]: https://stackoverflow.com/a/10325915
[so-zeroing]: https://stackoverflow.com/a/33668295
[wikipedia-cc-amd64-abi]: https://en.wikipedia.org/wiki/X86_calling_conventions#System_V_AMD64_ABI
[felix-x86-ret]: https://www.felixcloutier.com/x86/ret
