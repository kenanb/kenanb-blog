---
layout: post
title:  "Generating Tag Files"
date:   2024-01-12 20:00:00 +1300
categories: code c-cpp
published: true
---

Bash snippet to generate tag files for C/C++.

[ *Check out all posts in "c-cpp" series [here][series-c-cpp].* ]

[series-c-cpp]: {% link series.md %}#c-cpp

Yesterday, I shared a code snippet to check out [type size and alignment][int-prev] in C++. I don't have enough time for a decent post today, so I will just continue with a useful snippet for C/C++ development.

This will be just a few lines of `bash` to easily generate a combined tag file using [ctags][ctags], for multiple related repositories.

Let's say I use [SDL][sdl]. There are a bunch of repositories that extend the feature set, like [SDL_image][sdl-image].

[sdl]: https://github.com/libsdl-org/SDL
[sdl-image]: https://github.com/libsdl-org/SDL_image/

So I want to have a single TAGS file that covers all relevant repositories.

Let's say I cloned all these repos as subdirectories under `$HOME/ext/sdl`. I use a `bash` snippet to:
- gather the set of all relevant source files under those locations,
- generate tags for each.

During the process, I am creating a file called `src_sdl.list`. It contains the list of source files for which tags will be generated. Then the tag file (called `TAGS`) will be generated. Note that `src_sdl.list` is useless after tag generation.

Here is the example code:

``` shell
TAGS_SOURCES_SDL="$HOME/ext/sdl/sdl/src
$HOME/ext/sdl/sdl/include
$HOME/ext/sdl/mxr/src
$HOME/ext/sdl/mxr/include
$HOME/ext/sdl/img
$HOME/ext/sdl/ttf"

cd $HOME/ext/sdl

for d in $TAGS_SOURCES_SDL
do find $d -type f -name '*.h' -o -name '*.hh' -o -name '*.hpp' >> src_sdl.list
done

for d in $TAGS_SOURCES_SDL
do find $d -type f -name '*.c' -o -name '*.cc' -o -name '*.cpp' >> src_sdl.list
done

ctags -e --c++-kinds=+p -L src_sdl.list
```

The `-e` option to `ctags` mean "create a tag file for use with Emacs". So if you use another editor, you probably don't need that.

This is super simple, but I often regenerate the tags after updating the cloned repos. So it is good to have a script to repeat the process.

## Are Tags Still Relevant?

It looks like LSP is all the rage nowadays. And it honestly sounds great.

However, I prefer to rely on fairly rudimentary dev tooling that I know I can absolutely use anywhere, and setup quickly, and tags fits this criteria.

I need cheap and reliable `jump-to-definition`. I don't care about intelligent code completion.

I suspect I am still missing out on some other nice features LSP provides, so I intend to try it sometime.

Anyway, long story short, I use tags. Tags are fine.

Thanks for reading! If you find technical errors, please report in the blog's [Issues page][report].

[report]: https://github.com/kenanb/kenanb-blog/issues

[int-prev]: {% post_url 2024-01-11-observing-type-size-and-alignment %}

[ctags]: https://ctags.io/
