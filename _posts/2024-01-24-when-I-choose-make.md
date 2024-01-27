---
layout: post
title:  "When I Choose Make"
date:   2024-01-24 21:00:00 +1300
categories: code makefile
published: true
---

Notes on Make build automation tool.

[ *Check out all posts in "makefile" series [here][series-makefile].* ]

[series-makefile]: {% link series.md %}#makefile

I really appreciate modern meta-build, packaging, and deployment systems. But there is also something I like about [*make*][wikipedia-make] as a lower level build automation utility on UNIX. I think it fills a certain role in the larger ecosystem pretty well.

I can think of a few cases where it is premature to go with a modern meta-build system.

Maybe the project is naturally very small or never meant to be deployed. Or it will eventually get larger, but maybe one of the cases below apply in early days:
- You don't want to think about your deployment strategy, or the right build system, just yet.
- It is too early to be abstracted away (via build configuration layers) from the single machine and OS you will be targeting for the next couple of months.
- The structure of your early prototype is too unconventional to benefit from the higher-level functionality a meta-build system would be designed to support well.
- Maybe you are not even sure what you are building yet, and you are worried that the build system will get in the way.

I think *make* fits rather well to this kind of non-committal stage of a codebase. The stage where you want a streamlined dev-build-test loop, but that's all you care about for now.

I will elaborate on why make is good for that, in [my next post][int-makefile-3].

[wikipedia-make]: https://en.wikipedia.org/wiki/Make_(software)
[int-makefile-3]: {% post_url 2024-01-25-using-make-the-good %}
