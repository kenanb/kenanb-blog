---
layout: post
title:  "Emacs Registers"
date:   2024-02-01 15:00:00 +1300
categories: code emacs
published: true
---

Emacs has a way of practically storing information to be retrieved and used later.

[ *Check out all posts in "emacs" series [here][series-emacs].* ]

[series-emacs]: {% link series.md %}#emacs

Today's post will be short, as I am a bit busy.

I thought I might just continue talking about [GNU Emacs][wikipedia-gnu-emacs] features. This one is called [*registers*][emacs-manual-registers]. It is a generic facility that allows users to store simple data, like strings, integers, and I think most importantly, buffer positions.

I haven't learned about emacs registers for years. I guess the name didn't sound very relevant. I still couldn't make a habit of using it. So this will be a very incomplete description of what I use them for, so far:

Using registers, you can jump around specific locations across buffers. You first assign the buffer position the cursor is at, to a key of your preference via a shortcut. You can then jump back to that location, using another shortcut suffixed by that key.

When you `jump-to-register`, the window tile that's currently in focus will start displaying the buffer associated with the register, and the cursor will move to the registered position in that buffer.

If you just have some feature to implement in a file or two, you don't need a facility to help you navigate buffers precisely. But if you are investigating a bug, or if you are trying to get familiar with a new codebase, the effort often involves jumping back and forth across many buffers. So register-based buffer navigation is great for that.

Thanks for reading! If you find technical errors, please report in the blog's [Issues page][report].

[report]: https://github.com/kenanb/kenanb-blog/issues

[wikipedia-gnu-emacs]: https://en.wikipedia.org/wiki/GNU_Emacs
[emacs-manual-registers]: https://www.gnu.org/software/emacs/manual/html_node/emacs/Registers.html

