---
layout: post
title:  "Emacs Follow Mode"
date:   2024-01-31 20:00:00 +1300
categories: code emacs
published: true
---

I will be describing a neat emacs feature.

[ *Check out all posts in "emacs" series [here][series-emacs].* ]

[series-emacs]: {% link series.md %}#emacs

[GNU Emacs][wikipedia-gnu-emacs] is my preferred editor. I learnt to use it in early university years [while trying to learn Common Lisp][int-road-to-lisp]. Over the years, it essentially became a digital exoskeleton for me. GNU Emacs is [available here][gnu-org-emacs].

[Emacs][wikipedia-emacs] presents content like a tiling window manager.

I am not too familiar with VSCode. I assume majority of the benefits of a tiling editor is somehow made available in VSCode (and other popular editors), as well.

But I will start by describing it, just to have a brief record of emacs terminology that I can refer to later. A more formal description [is available in the manual][emacs-manual-basic-windows].

## Windows and Buffers

The user can horizontally or vertically split the main region of the emacs application window into smaller regions.

Emacs terminology is a bit different. Emacs calls each "application window" a "Frame". And each subregion we generate is called a "window".

Each open file is associated with a "buffer". And the user can freely assign buffers to these "windows".

```
              B1            B2             B3
         +----------+  +---------+  +---------+
Buffers: | dev/foo  |  | dev/bar |  | dev/baz |
         +----------+  +---------+  +---------+

                              +---------------+  _ FRAME
         +------+------+      |+------+------+| /
         |      |      |      ||      |      ||/
Windows: | W1   |  W3  | ---> || B1   |  B1  ||
         +------+      |      |+------+      ||
         | W2   |   \  |      || B3   |  \   ||
         +------+----\-+      ++------+---\--++
                      \_ WINDOW            \_ BUFFER ASSIGNED
                                              TO WINDOW
```

It also allows you to assign the same buffer to multiple windows.

The usual workflow involves actions like:
- creating new buffers (some of which represent things like a "shell", a "repl", or information generated as a result of a command),
- cycling between buffers, by changing the buffer a window is associated with,
- partially modifying the layout.

You can also create new frames, which will share buffers with other frames of the same emacs process.

Buffers have major and minor *modes* associated with them[^1], determining behaviour and providing functionality.

## Follow Mode

Now to the main topic: [`follow-mode`][emacs-manual-follow-mode]. Follow mode is a [minor-mode][emacs-manual-minor-mode].

I believe this feature is still missing in VSCode, as [this closed issue][github-vscode-issue] is the only information I found on a quick search.

I already mentioned that you can assign the same buffer to multiple windows. What `follow-mode` allows you to do is synchronizing the scrolling state between all windows associated with the buffer we enable `follow-mode` for, in a way that they show contiguous blocks of text.

So if I have a file that has very long function definitions, I can:
- split the emacs frame into 2 or 3 windows,
- assign the same buffer to each,
- and browse many lines at once.

```
+---------+---------+---------+
| 01 ...  |08       |15  .... |
| 02      |09  .... |16  .... |
| 03 .... |10  .... |17       |
| 04 .... |11   ... |18  ...  |
| 05  ... |12   ... |19       |
| 06  ... |13       |20  .... |
| 07      |14   ... |21  .... |
+---------+---------+---------+
```

Let's say we scrolled down a bit. All pages will scroll the same amount of lines.
```
+---------+---------+---------+
| 13      |20       |27       |
| 14  ... |21  .... |28  .... |
| 15 .... |22  .... |29       |
| 16 .... |23       |30  ...  |
| 17      |24   ... |31       |
| 18 ...  |25   ... |32  .... |
| 19      |26   ... |33  .... |
+---------+---------+---------+
```

I find this feature super handy, especially when I am looking at an unfamiliar codebase, and I feel I need to see larger sections of the code at once.

That's all for today! *I managed to write one blog post for every day this month. So that's cool.*

Thanks for reading! If you find technical errors, please report in the blog's [Issues page][report].

[report]: https://github.com/kenanb/kenanb-blog/issues

[^1]: Actually, minor modes can be global, as well, instead of being buffer-local.

[int-road-to-lisp]: {% post_url 2024-01-20-my-road-to-common-lisp %}

[wikipedia-gnu-emacs]: https://en.wikipedia.org/wiki/GNU_Emacs
[gnu-org-emacs]: https://www.gnu.org/software/emacs/
[wikipedia-emacs]: https://en.wikipedia.org/wiki/Emacs
[github-vscode-issue]: https://github.com/microsoft/vscode/issues/88118
[emacs-manual-basic-windows]: https://www.gnu.org/software/emacs/manual/html_node/elisp/Basic-Windows.html
[emacs-manual-follow-mode]: https://www.gnu.org/software/emacs/manual/html_node/emacs/Follow-Mode.html
[emacs-manual-minor-mode]: https://www.gnu.org/software/emacs/manual/html_node/emacs/Minor-Modes.html
