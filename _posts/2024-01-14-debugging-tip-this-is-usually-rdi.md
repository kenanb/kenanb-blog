---
layout: post
title:  "Debugging Tip: this is usually %rdi"
date:   2024-01-14 15:00:00 +1300
categories: code low-level
published: true
---

Assembly-level debugging tip for a C++-heavy project.

[ *Check out all posts in "low-level" series [here][series-low-level].* ]

[series-low-level]: {% link series.md %}#low-level

Here is the key takeaway of this post:

**In x86-64 Linux distributions, [the `this` pointer][cppreference-this] will _usually_ be passed via `rdi` register when calling a non-static member function.**

Nowadays, most debugging is thankfully done at source level. However, I think some familiarity of lower-level patterns is good. The role of `rdi` register is a good start, because it is easy to remember, and it shows up a lot in generated instructions.

I will start with this occasionally-useful bit of information, and use it as an excuse to discuss some key concepts in future posts. Some of these concepts will be needed to [understand interop][int-asm-7] as well.

Let's confirm the statement using the silly example program below:

``` c++
#include <cstdio>
#include <ctime>
#include <cmath>

#define RAND_MOD 32

struct Foo
{
    Foo( const char offset ) : m_charOffset( offset )
    {
    }

    /* Generate a random char, offsetting
       the value to get a printable character. */
    char
     RandChar( )
    {
        srand( clock( ) );
        char randChar = rand( ) % RAND_MOD;
        return randChar + this->m_charOffset;
    }

    char m_charOffset;
};

int
 main( )
{
    Foo foo{ 0x40 };

    printf( "Generated char is: %c\n", foo.RandChar( ) );

    return 0;
}
```

File is called `this_is_rdi.cc`. It satisfies a few requirements for the topic: a class, a member function, data member access, etc.

We can compile with the usual:
``` shell
g++ this_is_rdi.cc -o this_is_rdi -O0
```

We don't optimize at all so that the assembly maps nicely to source.

When executed, the program, `this_is_rdi`, will print out a random character:
```
Generated char is: S
```

Here is the function block graph `rizin` generated for `main`:

```
 .-----------------------------------------------.
 |  0x5563e81b9179                               |
 | int main(int argc, char **argv, char **envp); |
 | ; var int64_t var_11h @ stack - 0x11          |
 | ; var int64_t var_10h @ stack - 0x10          |
 | push  rbp                                     |
 | mov   rbp, rsp                                |
 | sub   rsp, 0x10                               |
 | mov   rax, qword fs:[0x28]                    |
 | mov   qword [var_10h], rax ; local.set 1      |
 | xor   eax, eax                                |
 | lea   rax, [var_11h] ; local.get 1            |
 | mov   esi, 0x40                               |
 | mov   rdi, rax                                |
 | call  sym.Foo::Foo_char                       |
 | lea   rax, [var_11h] ; local.get 1  <- address of foo
 | mov   rdi, rax                      <- copy addr to RDI
 | call  sym.Foo::RandChar             <- call Foo::RandChar
 | movsx eax, al                                 |
 | mov   esi, eax                                |
 | lea   rax, str.Generated_char_is:__c          |
 | mov   rdi, rax                                |
 | mov   eax, 0                                  |
 | call  sym.imp.printf                          |
 | mov   eax, 0                                  |
 | mov   rdx, qword [var_10h] ; local.get 1      |
 | sub   rdx, qword fs:[0x28]                    |
 | je    0x5563e81b91df                          |
 '-----------------------------------------------'
       t f
       | |
    .--' |
    |    '------------------.
    |                       |
.------------------.    .--------------------------------.
|  0x5563e81b91df  |    |  0x5563e81b91da                |
| leave            |    | call  sym.imp.__stack_chk_fail |
| ret              |    '--------------------------------'
'------------------'
```

While disassembling, `rizin` generates convenient symbol names for data addresses, which is easier to read.

I annotated the important bits.

[Write the address][felix-lea] of stack location reserved for `foo`, to `rax`:
``` nasm
lea   rax, [var_11h] ; local.get 1
```

Copy `foo` address stored in `rax` to `rdi`:
``` nasm
mov   rdi, rax
```

Call `Foo::RandChar` member function for `foo`:
``` nasm
call  sym.Foo::RandChar
```

Note that there is some obvious redundancy here due to optimization being disabled.

And here is the beginning of `Foo::RandChar`:
```
.------------------------------------------.
|  0x5563e81b91fc                          |
| sym.Foo::RandChar(int64_t arg1);         |
| ; arg int64_t arg1 @ rdi                 |
| ; var int64_t var_20h @ stack - 0x20     |
| ; var int64_t var_9h @ stack - 0x9       |
| push  rbp                                |
| mov   rbp, rsp                           |
| sub   rsp, 0x20                          |
| mov   qword [var_20h], rdi ; local.set 1 |
| call  sym.imp.clock                      |
| mov   edi, eax                           |
|                 ...                      |
```

Thanks to `rizin`'s annotation of the function, we can clearly see that the function got one implicit argument, and that was passed via `rdi`.

This behaviour is defined as a combination of ABI standards:

## Itanium C++ ABI

[Itanium C++ ABI][itanium-abi] is followed by most C++ compilers (except MSVC). About `this` pointer, [it says below][itanium-abi-this]:

> Non-static member functions, including constructors and destructors, take an implicit this parameter of pointer type. It is passed as if it were the first parameter in the function prototype, except as modified for non-trivial return values.

## System V AMD64 ABI

[System V AMD64 ABI][wikipedia-cc-amd64-abi] defines the calling conventions followed by Linux x86-64.

The passing of parameters for function calls is part of the [calling convention][wikipedia-cc].

System V AMD64 ABI says first 6 integer or pointer arguments are carried in: `RDI`, `RSI`, `RDX`, `RCX`, `R8`, `R9`, in that order. 

So:
- the first integer or pointer parameter is carried in `rdi`.
- `this` pointer is usually the implicit first parameter of a non-static member function.

Therefore, we observe this behaviour.

In a later post, we will have a closer look at concepts like calling conventions.

We will also step through the generated assembly for this example code, and point out a few other common patterns.

That's it for today. Thanks for reading. If you find technical errors, please report in the blog's [Issues page][report].

[report]: https://github.com/kenanb/kenanb-blog/issues

[int-asm-7]: {% post_url 2024-01-09-c-assembly-interop %}

[cppreference-this]: https://en.cppreference.com/w/cpp/language/this
[felix-lea]: https://www.felixcloutier.com/x86/lea
[itanium-abi]: https://itanium-cxx-abi.github.io/cxx-abi/abi.html
[itanium-abi-this]: https://itanium-cxx-abi.github.io/cxx-abi/abi.html#this-parameters
[wikipedia-cc-amd64-abi]: https://en.wikipedia.org/wiki/X86_calling_conventions#System_V_AMD64_ABI
[wikipedia-cc]: https://en.wikipedia.org/wiki/Calling_convention
