---
layout: post
title:  "Exploring Embeddable Common-Lisp"
date:   2024-01-21 14:00:00 +1300
categories: code lisp
published: true
---

Exploring the embedding and interop features of ECL.

[ *Check out all posts in "lisp" series [here][series-lisp].* ]

[series-lisp]: {% link series.md %}#lisp

In my [previous post][int-lisp-1], I mentioned that I really like Lisp but I only used it for hobby projects so far.

Professionally, I mainly use C, C++ and Python. There have been many times that I realized the solution I settled on is heavily influenced by a Lisp mindset. Mainstream languages pulled more and more concepts from it, so now it is even easier to think in Lisp and write in another language. I don't mind the label of the tool as long as it is a feasible option, and it empowers me to engineer good solutions.

Nowadays, I rarely think "this would benefit a lot from being implemented in a Lisp instead". It is even less rare that I consider it feasible to do so, even when there are good technical reasons. But there are cases where the impact of easy exploration (with good enough performance) is a higher priority than even the long term feasibility concerns. And it would be good to be able to throw Lisp at the problem when that's the case. Even if only for early prototyping.

There are many implementations of the Common Lisp standard. My primary choice has always been [SBCL][sbcl-main]. It is an extremely good native implementation.

Problems I want to throw Lisp at are sometimes existing tech written in C/C++. So I have been looking into interop options.

Common Lisp generally has strong FFI support for C interop. But I also wanted to explore the implementations that are written with embedding in mind. So I am having a closer look at [Embeddable Common-Lisp][ecl-main] today. [Here is the Wikipedia page][wikipedia-ecl] for ECL.

ECL has [a long history][ecl-history], and it has been actively maintained. I just found that one of the maintainers, Daniel Kochmański, has a nice European Lisp Symposium 2020 talk [uploaded on YouTube][els-2020-on-ecl].

As a starting point, I found [this nice embedding example][ecl-repo-example-embed] on ECL repository. It is pretty straightforward.

I don't yet have a good understanding of the strenghts and weaknesses of this implementation. Will write more as I learn.

Another very interesting CL implementation I started looking into is [Clasp][clasp-main], which originally [derived from ECL][clasp-credits]. Clasp focuses on interop with C++.

Thanks for reading! If you find technical errors, please report in the blog's [Issues page][report].

[report]: https://github.com/kenanb/kenanb-blog/issues

[int-lisp-1]: {% post_url 2024-01-20-my-road-to-common-lisp %}

[sbcl-main]: https://www.sbcl.org/
[ecl-main]: https://ecl.common-lisp.dev/main.html
[wikipedia-ecl]: https://en.wikipedia.org/wiki/Embeddable_Common_Lisp
[ecl-history]: https://ecl.common-lisp.dev/static/files/manual/current-manual/History.html
[ecl-repo-example-embed]: https://gitlab.com/embeddable-common-lisp/ecl/-/tree/develop/examples/embed
[els-2020-on-ecl]: https://www.youtube.com/watch?v=bPlVrQtkf8E
[clasp-main]: https://clasp-developers.github.io/
[clasp-credits]: https://clasp-developers.github.io/manual.html#orgd60f52d
