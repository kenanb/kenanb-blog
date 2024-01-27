---
layout: post
title:  "Using Make: The Ugly"
date:   2024-01-27 14:00:00 +1300
categories: code makefile
published: true
---

Notes on Make build automation tool.

[ *Check out all posts in "makefile" series [here][series-makefile].* ]

[series-makefile]: {% link series.md %}#makefile

I mentioned some problematic aspects of working with [*make*][wikipedia-make] in [my previous post][int-makefile-4]. I will conclude this topic with the ugly parts.

Make [has functions][make-functions], but they are really just raw text transformations.

Make [seemingly allows you to setup recursive builds][make-recursion]. But for most projects you apply it to, the DAG it generates will be incorrect, because the dependency relationships of the inner graph will be opaque to the outer. You can check out a paper, and the corresponding HN discussion about this problem [here][hn-paper-rmch].

Choosing a flat build setup instead is often recommended, but that means you need to be more specific and verbose with the variable names.

The semantics of many features [turn out to be different][make-variable] than what most users would have expected.

It is difficult to avoid coupling between parts of the makefile, unless you choose to have very repetitive and verbose makefiles.

A higher level build tool is no silver bullet. Dependency issues are common regardless. However, by design, they provide consistent - and sometimes opinionated - frameworks of how the most common problems should be solved. And if the framework fits your needs, the bugs tend to be more contained, and they stand out.

So yeah. That's the end of this topic. Thanks!

[wikipedia-make]: https://en.wikipedia.org/wiki/Make_(software)
[int-makefile-4]: {% post_url 2024-01-26-using-make-the-bad %}

[hn-paper-rmch]: https://news.ycombinator.com/item?id=20014348

[make-recursion]: https://www.gnu.org/software/make/manual/html_node/Recursion.html
[make-variable]: https://www.gnu.org/software/make/manual/html_node/Recursive-Assignment.html
[make-functions]: https://www.gnu.org/software/make/manual/html_node/Functions.html
