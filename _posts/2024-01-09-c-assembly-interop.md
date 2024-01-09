---
layout: post
title:  "Interop Between C/C++ and Assembly: BEGIN"
date:   2024-01-09 22:00:00 +1300
categories: code low-level
published: true
---

Collecting resources to start exploring interop between C, C++ and x86 Assembly.

[ *Check out all posts in "low-level" series [here][series-low-level].* ]

[series-low-level]: {% link series.md %}#low-level

I originally studied Architecture (as in buildings).

[Niklaus Wirth][wikipedia-niklaus-wirth] was among the many computer science pioneers that I started reading many years after I got into programming. I was reading [Algorithms and Data Structures][wikipedia-algorithms-data-structures] recently, and I was really enjoying it. By now I am, as many others, a fan of his work.

Wirth has died last week.

In [my last post][int-asm-6], I shared some useful resources on the topic of "instruction encoding", and wrapped that topic, for now. I needed a new topic, and I needed a word to mark the first post.

So I thought [`BEGIN`][pascal-language-constructs] is a suitable word.

And regarding the topic: I wonder if I should focus on C/C++ and Assembly interop next.[^1]

Of course, one aspect is inline assembly. You can read up on it in language references:
- [C reference for Inline Assembly][c-ref-asm]
- [C++ reference for `asm` declaration][cpp-ref-asm]

I used this kind of interop a few times, but my knowledge is limited to just a few patterns.

Actually, I used `asm` declaration the most for the rather famous functions below:

``` c++
static void
 escape( void * p )
{
    asm volatile( "" : : "g"( p ) : "memory" );
}

static void
 clobber( )
{
    asm volatile( "" : : : "memory" );
}
```

These functions are verbatim copied from Chandler Carruth's [CppCon 2015 talk "Tuning C++: Benchmarks, and CPUs, and Compilers! Oh My!"][cppcon-2015-carruth-benchmark]. (**I highly recommend watching this!**)

They are basically used to prevent compiler optimizations from messing with the benchmarking attempt. Chandler describes it in detail in the talk. Here are some other explanations I found on StackOverflow regarding this, and another related pattern: [ Link 1 ][so-answer-0], [ Link 2 ][so-answer-1], [ Link 3 ][so-answer-2].

OK, back to resources. The online documentation on the topic of inline assembly seems pretty good.

Here is some documents I started reading:
- Félix Cloutier's [GCC's assembler syntax][felix-gcc-asm] page.
- GCC docs on [How to Use Inline Assembly Language in C Code][gcc-inline-asm].
- OSDev [Inline Assembly][osdev-inline-assembly] page.

Other than that, I found [these really cool online lecture notes][fullcoll-cs241-asm-to-c] that describe calling C from assembly, and vice versa. That is, non-inline, link level interop. In fact, I got the impression that [the course notes][fullcoll-cs241] are generally really good.

That's it for today! I am still trying to teach myself how to write a useful/interesting post every day while also making progress in my other work. I might prioritize some other topics in the following days, but I will return to this.

Thanks for reading!

[^1]: If you find technical errors, please report in the blog's [Issues page][report].

[report]: https://github.com/kenanb/kenanb-blog/issues


[int-asm-6]: {% post_url 2024-01-07-x86-insn-encoding-resources %}

[wikipedia-niklaus-wirth]: https://en.wikipedia.org/wiki/Niklaus_Wirth
[wikipedia-algorithms-data-structures]: https://en.wikipedia.org/wiki/Algorithms_%2B_Data_Structures_%3D_Programs
[pascal-language-constructs]: https://en.wikipedia.org/wiki/Pascal_(programming_language)#Language_constructs
[cppcon-2015-carruth-benchmark]: https://youtu.be/nXaxk27zwlk?feature=shared&t=2473
[c-ref-asm]: https://en.cppreference.com/w/c/language/asm
[cpp-ref-asm]: https://en.cppreference.com/w/cpp/language/asm
[gcc-inline-asm]: https://gcc.gnu.org/onlinedocs/gcc/extensions-to-the-c-language-family/how-to-use-inline-assembly-language-in-c-code.html
[felix-gcc-asm]: https://www.felixcloutier.com/documents/gcc-asm.html
[osdev-inline-assembly]: https://wiki.osdev.org/Inline_Assembly
[fullcoll-cs241-asm-to-c]: https://staffwww.fullcoll.edu/aclifton/cs241/lecture-asm-to-c-interop.html
[fullcoll-cs241]: https://staffwww.fullcoll.edu/aclifton/courses/cs241/
[so-answer-0]: https://stackoverflow.com/a/52208443
[so-answer-1]: https://stackoverflow.com/a/44566496
[so-answer-2]: https://stackoverflow.com/a/40149692
