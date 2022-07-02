---
layout: post
title:  "Why I Like Lisp Syntax - 1"
date:   2022-07-01 12:00:00 +1200
categories: code languages
published: true
---

Because it has space for expansion.

Simple languages can afford rich syntax. As long as they stay simple, syntax won't get in the way. C is a simple language. C syntax is rich, and it works for C.

Python was a simple language. It didn't resist growth, so it had to make breaking changes and remove syntax.

C++ starting with the C syntax was a big mistake. I see the same mistake being made in most modern compiled languages.

If you are designing a general purpose language with the intention to grow, just use prefix, or postfix. Whatever. But no infix binary operators. And a simple syntax for derived type specification. Also, maybe wait a few years before introducing syntactic sugar.

If you are designing a scripting language, or a DSL, make sure it stays as such.
