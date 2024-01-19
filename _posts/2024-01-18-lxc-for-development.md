---
layout: post
title:  "LXC for Development"
date:   2024-01-18 22:00:00 +1300
categories: code containers
published: true
---

I tried to use Linux Containers (LXC) as my development environment.

[ *This is not an LXC tutorial. I am too new to the tech to write a reliable tutorial.* ]

[ *Check out all posts in "containers" series [here][series-containers].* ]

[series-containers]: {% link series.md %}#containers

In my [previous post on LXC][int-containers-1], I shared my first impressions on the LXC technology. But I didn't really try to use it for dev work at the time.

Today, I made my first attempt to actually do development using an LXC container.

**It was great so far!** It provides me the library version isolation behaviour I was after, with the native performance I needed.

I was able to test below steps without observing any issues:
- Build USD ([OpenUSD][openusd]), building some of its dependencies, and using the distribution's package manager for others.
- Load [Kitchen Set][openusd-kitchen-set] scene in `usdview`, confirming the responsiveness of the runtime.
- Build an executable that links against USD libs, and run it.
- Modify the sources using my preferred editor, `emacs`, and rebuild.

There were a couple instances where a new emacs process I spawned crashed at startup. But that was the only instability I encountered during my hours of testing. Running emacs sessions were very stable.

Another aspect of the LXC container-based workflow I really enjoyed is that I feel like I am working on a really clean OS setup:
- My home directory in the container has much fewer files.
- Running `htop` shows just a few processes.

## tmux

I decided to introduce another tool to my container-based dev workflow: [`tmux`][wikipedia-tmux]. It is a "[terminal multiplexer][wikipedia-terminal-multiplexer]".

I evaluated `tmux` a few years ago. It was very cool, but I decided that its window-management related features overlap with other tools I use daily: tiling window managers, and emacs. And I didn't really need persistent terminal sessions at the time.

So how does it help me now?

It felt limiting to access the container through a terminal. Because I normally use a separate terminal for separate tasks. You can attach the container multiple times, but then it would be frustrating to constantly distinguish "container-attached terminals" from "host terminals".

So I figured I can:
- usually attach from a single terminal
- utilize `tmux` to make that single terminal behave like my desktop.

This indeed made the experience much more convenient for me. I still spawn separate emacs sessions with X support, and I use a different background color for the emacs running inside the container.

I will keep sharing my experience with development using LXC containers.

Thanks for reading! If you find technical errors, please report in the blog's [Issues page][report].

[report]: https://github.com/kenanb/kenanb-blog/issues

[int-containers-1]: {% post_url 2024-01-13-linux-containers-first-impressions %}
[openusd]: https://openusd.org/release/index.html
[openusd-kitchen-set]: https://openusd.org/release/dl_downloads.html
[wikipedia-tmux]: https://en.wikipedia.org/wiki/Tmux
[wikipedia-terminal-multiplexer]: https://en.wikipedia.org/wiki/Terminal_multiplexer
