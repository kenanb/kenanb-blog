---
layout: post
title:  "ECL Object Lifetime and Ownership"
date:   2024-02-02 22:00:00 +1300
categories: code lisp
published: true
---

I am trying to build a mental model of object lifetimes in ECL.

[ *Check out all posts in "lisp" series [here][series-lisp].* ]

[series-lisp]: {% link series.md %}#lisp

Another short post today.

I have been exploring [Embeddable Common-Lisp][wikipedia-ecl] for a few weeks now. Here is my [previous ECL post][int-lisp-3]. I learned quite a lot since. I am now fairly familiar with:
- the general C API,
- ECL specific Lisp API,
- some key sections of the C codebase,
- the primary means of communicating between the C/C++ world and the Lisp world.

I also mentioned that I was trying to have a better grasp of the [memory management][ecl-overview-memory-management] aspect, which I started.

There is not much to say for objects that live purely in Lisp world.[^1] Because it will just behave like what you would expect a garbage collected environment to behave.

However, the situation is different for *foreign objects*. Working with data across runtimes with different data models usually involves multiple layers of wrapper/proxy objects. So that already obfuscates things. On top of that, if the main memory management strategy of those runtimes are not similar, the topic of memory management and ownership becames a pretty complicated one to reason about.

I briefly talked about the [`cl_object` type][int-lisp-3-cl-object] in my previous ECL related post. This value either encodes an *immediate*, or a pointer to a *box* object. The box can be set to contain "foreign data", in which case it will point to a foreign data block. Here are a few subtleties that requires accounting for in the mental model, if you aim to transfer data across:
- There is a question of *foreign data lifetime and ownership*.
- There is a question of *wrapper object lifetime and ownership*.
- De-facto ownership expectations for the Lisp->C case is not quite the same as C->Lisp case. So there is a bit of asymmetry as well.
- Since the Lisp world utilizes garbage collection, there is the question of how references to allocations are tracked, and which memory regions are scanned for that.

At this point, there are gaps in my understanding, so I will defer elaborating further to another post. But it is an interesting topic, so I am hoping to return later with some real-world examples.

Thanks for reading! If you find technical errors, please report in the blog's [Issues page][report].

[report]: https://github.com/kenanb/kenanb-blog/issues

[^1]: That is, unless your usecase involves resource utilization concerns.

[wikipedia-ecl]: https://en.wikipedia.org/wiki/Embeddable_Common_Lisp

[int-lisp-3]: {% post_url 2024-01-23-ecl-exploration-continued %}
[int-lisp-3-cl-object]: {% post_url 2024-01-23-ecl-exploration-continued %}#cl-object

[ecl-overview-memory-management]: https://ecl.common-lisp.dev/static/files/manual/current-manual/Memory-Management.html#Memory-Management
