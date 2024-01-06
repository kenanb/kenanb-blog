---
layout: post
title:  "Automatic Variables in Make Recipes"
date:   2022-07-10 21:00:00 +1200
categories: code makefile
published: true
---

Modern build systems are great for real-world projects, but `make` is my go-to solution for small builds on Linux.

Make defines many [automatic variables][doc-auto-vars] to be used in [recipes][doc-recipes].

They are easy to forget. So I wrote myself a makefile. It works as an interactive reminder.[^1]

It defines a few example rules, using a custom macro. When I run with "make", it builds 8 targets, but the build recipes do nothing but print information about the rule.

Below is the report of `test_2/baz` target:

```
makefile:16: Building test_2/baz (from foo) (foo y.bar z.bar newer)

RULE > test_2/baz : foo y.bar z.bar       << RULE
     ( %/baz : foo y.bar z.bar )          << PATTERN RULE
                                             _
AUTO > @  : test_2/baz                    <<  |
AUTO > @D : test_2                        <<  |
AUTO > @F : baz                           <<  |
AUTO > *  : test_2                        <<  |
AUTO > *D : .                             <<  |
AUTO > *F : test_2                        <<  | AUTOMATIC
AUTO > <  : foo                           <<  | VARIABLES
AUTO > <D : .                             <<  |
AUTO > <F : foo                           <<  |
AUTO > ^  : foo y.bar z.bar               <<  |
AUTO > ^D : . . .                         <<  |
AUTO > ^F : foo y.bar z.bar               << _|
       \/   ________________
        \                   \__ VALUE
         \__ VARIABLE
```
{: style="opacity: 0.60;" }

Here is the makefile:
{% highlight make %}
# Shell replacement to print the rule being run.
OLD_SHELL ::= $(SHELL)
SHELL = $(warning Building $@$(if $<, (from $<))\
	$(if $?, ($? newer)))$(OLD_SHELL)

# Print the rule, and the automatic variables.
AUTO_VARS = @ * < ^

GEN_AUTO = "\
	\nAUTO > $(1)  : $$($(1))\
	\nAUTO > $(1)D : $$($(1)D)\
	\nAUTO > $(1)F : $$($(1)F)"

GEN_RULE = $(1) : $(2) ; @echo -e "\
	\nRULE > $$@ : $$^\
	\n     ( "\
	$(1)":"$(2)")"$(foreach var,$(AUTO_VARS),\
	$(call GEN_AUTO,$(var)))"\n"

.PHONY: all
all: test_1/baz test_2/baz stem_1/a.stem_2.b

$(call GEN_RULE ,foo   , )
$(call GEN_RULE ,x.%   , )
$(call GEN_RULE ,%.bar , foo x.% )
$(call GEN_RULE ,%/baz , foo y.bar z.bar )
$(call GEN_RULE ,a.%.b , )
{% endhighlight %}

The shell replacement trick at the beginning is not mine.
I am not sure of its origin, but it exists in The GNU Make Book by John Graham-Cumming.
Every makefile I write starts with that snippet. Very useful.

Also, I use `make` with `--debug` option to find out how it matches patterns.

[^1]: If you find errors, please report in the blog's [Issues page][report].
[report]: https://github.com/kenanb/kenanb-blog/issues
[gnu-make]: https://www.gnu.org/software/make/
[doc-auto-vars]: https://www.gnu.org/software/make/manual/make.html#Automatic-Variables
[doc-recipes]: https://www.gnu.org/software/make/manual/make.html#Recipes
