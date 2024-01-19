---
layout: post
title:  "Linux Containers: First Impressions"
date:   2024-01-13 13:15:00 +1300
categories: code containers
published: true
---

Early opinions on Linux Containers (LXC).

[ *This is not an LXC tutorial. I am too new to the tech to write a reliable tutorial.* ]

[ *Check out all posts in "containers" series [here][series-containers].* ]

[series-containers]: {% link series.md %}#containers

I am relatively ignorant of containers. I have evaluated introducing them into my daily development workflow, many times. Every time I read about it, it was mostly in terms of [Docker][wikipedia-docker]. And Docker always felt like a few layers of extra complexity for marginal benefit in my case.

I generally assume that the kind of development I do doesn't benefit a lot from containerization.

Every once in a while, a library or package makes me reconsider.

## Problem

**TLDR:** Manually built large libraries that partially depend on OS packages stop working if you keep updating the OS.

It usually goes like this:
- I am a user of library `A`, and I sometimes make minor modifications to it to test something.
- I spend a few hours getting library `A` to configure and build precisely the way I need it, for my home PC. That often involves carefully configuring and building a couple of its dependencies as well.
- I develop some hobby project against the library `A` for a few days.
- I get busy with work for a few months.
- Meanwhile, I keep the OS updated, but I don't touch the hobby project.
- At some point, I try to pick up the hobby project from where I left off, only to realize that the library `A` is now incompatible with my system packages, and it is not a simple act of rebuilding.

That gets even more complicated if I wanted to update library `A` with the upstream, as well.

Obviously, I won't stop updating my OS, just to keep library `A` working. So it becomes inconvenient.

I am a scene description expert. So the library `A` is usually [USD (OpenUSD)][usd] nowadays.

*Spoilers: I will start posting about OpenUSD soon.*

## LXC

In my most recent evaluation of containers for my dev workflow, [I read about LXC][wikipedia-lxc].

I was really impressed by it. My summary is:
- It is pretty generic.
- It is a thin layer using existing Linux concepts.
- The experience is very similar to normal Linux userspace experience.

Overall, it is the convenience of a VM without the performance cost of a VM.

I wouldn't know how the experience is compared to Docker. [This seems like a good comparison][lxc-vs-docker].

## LXD

[There is also LXD][wikipedia-lxc-lxd]. That's a container manager built on top of LXC.

It sounded cool, but I wanted to avoid the extra layer, because I was worried that it will prevent me from building a good mental model of what's happening under the hood.

So I went ahead and setup the container using LXC directly.

## How did it go?

**It was a challenge.** I think I spent a whole day, from zero to "Blender running inside container with GPU access, using the X server running on the host". But **it was a success**. I think the setup from scratch would take less than an hour next time I try. Spawning a new container is just a few minutes now.

**It is super promising.** LXC command line interface is super easy to use. And once you attach to the container, it is familiar ground. I still haven't got a chance to test it for actual development.

I think the biggest challenge so far was the lack of reliable information online:
- A lot of outdated and partial information.
- Generally more information on LXD etc. that I had to reinterpret.

Overall, I am really happy I spent the time to get an LXC container working. But it did make me wonder if LXD is a more viable starting point.

## Details

This was a few weeks ago so I can't remember all issues I encountered.

I believe I started with [this][ubuntu-lxc], but I got the impression that it is outdated. My host system is ArchLinux so [its wiki page][archlinux-lxc] ended up being much better guidance for me, in terms of initial setup. I also referred to information [here][linuxcontainers-lxc] for creating unprevilaged containers. I had to jump between a few online threads to get GPU support and "connection to host X" working.

I am not very happy with the various solutions available for the `connecting to host X" part, but I found one that works, and it is fine for my situation. (I do wonder if I should have instead tried to get the container run its own X or Wayland session.)

I get the impression that all this is relatively straightforward with LXD because it has templates (?) for that.

I am still in the "not quite sure about the implications of this thing that I am doing in config" phase of "understanding LXC containers".

I really enjoy that LXC supports things like cloning a container using *overlayfs*. But once I started utilizing it, I realized I need to internalize the concept a bit to understand how to use it effectively.

There are a few rough edges. When I run package update in container, I sometimes get seemingly harmless permission denied errors, probably due to packages trying to run hooks that are unapplicable to a container.

But it works as I expected.

Thanks for reading! If you find technical errors, please report in the blog's [Issues page][report].

[report]: https://github.com/kenanb/kenanb-blog/issues

[wikipedia-docker]: https://en.wikipedia.org/wiki/Docker_(software)
[wikipedia-lxc]: https://en.wikipedia.org/wiki/LXC
[wikipedia-lxc-lxd]: https://en.wikipedia.org/wiki/LXC#LXD
[usd]: https://openusd.org/release/index.html
[lxc-vs-docker]: https://earthly.dev/blog/lxc-vs-docker/
[ubuntu-lxc]: https://ubuntu.com/server/docs/containers-lxc
[archlinux-lxc]: https://wiki.archlinux.org/title/Linux_Containers
[linuxcontainers-lxc]: https://linuxcontainers.org/lxc/getting-started/
