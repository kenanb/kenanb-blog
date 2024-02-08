---
layout: post
title:  "Chez Scheme and Gambit Scheme"
date:   2024-02-08 14:00:00 +1300
categories: code lisp
published: true
---

Looking into current state of various Scheme implementations.

[ *Check out all posts in "lisp" series [here][series-lisp].* ]

[series-lisp]: {% link series.md %}#lisp

This morning, I was updating my OS packages, and I realized [Chez Scheme][chez-webpage] had its v10.0 release! For those unfamiliar, here is the description of Chez from [its Wikipedia page][wikipedia-chez]:
> Chez Scheme is a programming language, a dialect and implementation of the language Scheme which is a type of Lisp. It uses an incremental native-code compiler to produce native binary files for the x86 (IA-32, x86-64), PowerPC, and SPARC processor architectures.

Links to Chez Scheme project page and source code are [available in its webpage][chez-webpage].

The release notes for v10.0.0 [is available here][chez-release-notes]. I have not used Chez a lot, but I have always been interested. The direction and amount of improvements really got me excited to give it a try with my current project, which I will write about soon.

There are a lot of nice improvements, from introduction of a *portable bytecode* to *wrapper procedures* and foreign interface improvements. Apparently, the compiler now generates code that locally unboxes floats. Great! But also, support for more architectures:

> AArch64 (64-bit Arm), RV64G (64-bit RISC-V), and LoongArch64 architectures are supported. Threads are supported on architectures with weak memory models while preserving the safety guarantee described in the documentation for variables and primitive datatypes.

I read in forums that recently, [Racket][wikipedia-racket] branch of Chez Scheme merged with the mainline Chez Scheme. Lisp dialects are not super mainstream nowadays, so this kind of consolidation is great to see.

Another scheme implementation I have always been interested is [Gambit Scheme][gambit-webpage]. Gambit has a compiler that compiles to `C`. I realize Gambit's foreign interface is written in a way to also support `new`/`delete` based resource management of `C++`. That got me curious regarding the `C++` interop experience in Gambit. Its [wikipedia page][wikipedia-gambit] also mentions that software written in Gambit-C can contain `C++` and integrate with corresponding libraries.

I hope to soon try both implementations with some minimal FFI code and share my findings.

That's all for today.

Thanks for reading! If you find technical errors, please report in the blog's [Issues page][report].

[report]: https://github.com/kenanb/kenanb-blog/issues

[^1]: I suspect "sufficiently smart compilers" might be trying to optimize method dispatch when it is safe, but CLOS also allows runtime modification of classes etc. So I am not sure how possible that is. I am aware of an interesting concept called [sealable metaobjects][github-sealable-metaobjects], but I didn't get a chance to try it yet.

[wikipedia-chez]: https://en.wikipedia.org/wiki/Chez_Scheme
[wikipedia-gambit]: https://en.wikipedia.org/wiki/Gambit_(Scheme_implementation)
[wikipedia-racket]: https://en.wikipedia.org/wiki/Racket_(programming_language)
[chez-release-notes]: https://cisco.github.io/ChezScheme/release_notes/v10.0/release_notes.html
[chez-webpage]: https://www.scheme.com/
[gambit-webpage]: https://gambitscheme.org/
