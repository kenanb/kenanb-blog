---
layout: post
title:  "Using Make: The Good"
date:   2024-01-25 21:00:00 +1300
categories: code makefile
published: true
---

Notes on Make build automation tool.

[ *Check out all posts in "makefile" series [here][series-makefile].* ]

[series-makefile]: {% link series.md %}#makefile

In [my previous post][int-makefile-2], I mentioned the specific cases where I find it suitable to choose *make* as a starting point, over a modern meta-build system. I will elaborate on the reasons today.

I mentioned that *make* fits really well to the non-committal stage of a codebase. The stage where you want a streamlined dev-build-test loop, but that's all you care about for now. But why is it good for that?

First of all, it is a smooth transition from using the compiler driver directly. No "project" or "executable" definition etc. You can move the compile command you were previously typing under a *makefile* rule, and refactor as you go.

Being fairly declarative, semantics of the `make` language allows you to concisely express relationships.

Make behaviour can be made very transparent, if you know the settings to make it so.

It naturally focuses you on the minimal set of low level actions that need to take place to get the output you need.

It is fast enough. It is standalone. It is flexible. It is ubiquitous.

You know perfectly well that directly working with `make` won't scale well as the project grows. **Or if you didn't, you should really know that.** But that is OK if you have no idea whether you will be iterating on that code long enough to care.

Over time, you end up collecting a few snippets that make it easier to remind yourself of the subtleties of *make*, and also to get better feedback from *make*.[^1] Then it becomes really easy to kickstart the prototyping phase of a new project without worrying about build and deployment decisions. 

[^1]: For example, I [shared a code snippet][int-makefile-1] to help remember the automated variable bindings a few years ago.

The next post will be [the bad parts][int-makefile-4].

[int-makefile-1]: {% post_url 2022-07-10-automatic-variables %}
[int-makefile-2]: {% post_url 2024-01-24-when-I-choose-make %}
[int-makefile-4]: {% post_url 2024-01-26-using-make-the-bad %}
