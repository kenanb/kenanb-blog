---
layout: post
title:  "Common Lisp Standard Method Combination"
date:   2024-02-07 14:00:00 +1300
categories: code lisp
published: true
---

Common Lisp Object System defines a flexible approach to method combination.

[ *Check out all posts in "lisp" series [here][series-lisp].* ]

[series-lisp]: {% link series.md %}#lisp

Common Lisp has a very powerful OOP system, called CLOS ([Common Lisp Object System][wikipedia-clos]). To be clear, I am specifically talking about expressive power, and flexibility. CLOS is not cheap in terms of performance. I assume it will be fast enough for many tasks, but I would not rely on CLOS as I can rely on OOP concepts of C++, if performance is very important.

One of the interesting aspects of the CLOS model is the emphasis on the functions. Methods are not owned by classes, but by "generic functions". Again, this is not the kind of generics statically typed languages like C++ has. It is just dynamic method dispatch. It is not a compile-time polymorphism solution, which I really wish existed in CL.[^1]

I think decoupling methods from the classes they operate on empowers the programmer to reason about the design in a more flexible manner. At least that's my memory of my CLOS experience. CLOS also privides a [*standard method combination*][clhs-method-combination] that has the concept of "auxiliary methods": `:before`, `:after`, and `:around`. So a call can automatically resolve to a well-ordered set of function calls. It is a good feature, if used wisely.

Wikipedia page of CLOS has [this nice diagram][wikipedia-smc-diagram] that visualizes the method application order defined by Standard Method Combination.

That's all for today.

Thanks for reading! If you find technical errors, please report in the blog's [Issues page][report].

[report]: https://github.com/kenanb/kenanb-blog/issues

[^1]: I suspect "sufficiently smart compilers" might be trying to optimize method dispatch when it is safe, but CLOS also allows runtime modification of classes etc. So I am not sure how possible that is. I am aware of an interesting concept called [sealable metaobjects][github-sealable-metaobjects], but I didn't get a chance to try it yet.

[wikipedia-clos]: https://en.wikipedia.org/wiki/Common_Lisp_Object_System
[clhs-method-combination]: https://www.lispworks.com/documentation/HyperSpec/Body/07_ffb.htm
[wikipedia-smc-diagram]: https://en.wikipedia.org/wiki/Common_Lisp_Object_System#/media/File:Method-combination.png
[github-sealable-metaobjects]: https://github.com/marcoheisig/sealable-metaobjects
