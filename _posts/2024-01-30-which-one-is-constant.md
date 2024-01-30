---
layout: post
title:  "Which One is Constant?"
date:   2024-01-30 20:00:00 +1300
categories: code c-cpp
published: true
---

Discussing the constness implied by various const keyword placements.

[ *Check out all posts in "c-cpp" series [here][series-c-cpp].* ]

[series-c-cpp]: {% link series.md %}#c-cpp

It is known that [declaration grammar][cppreference-declarations] in C/C++ can get pretty cryptic. But for reasons unknown to me, it is made even worse by introducing inconsistencies to the rules.

The position of the `const` keyword is what I am thinking of. There is actually a pretty consistent way to declare things constant: It can be placed on the right of the thing that it qualifies as `const`.

```
int const x;
\_______/
```

More complex:

```
int const * const xp;
\_______/ \_____/
```

But C/C++ allows you to place that leftmost `const` on the other side.
```
const int * const xp;
\_______/ \_____/
```

And unfortunately, that's the popular (or even canonical, considering it being used in most texts) style. So I guess every newcomer starts by first learning the exception that doesn't scale to more elaborate declarations, only to then relearn how to read things later.

Here is a good SO discussion about the general rule: [What is the difference between const int*, const int * const, and int const *?][so-thread]. It links to a few other useful pages as well.

Thanks for reading! If you find technical errors, please report in the blog's [Issues page][report].

[report]: https://github.com/kenanb/kenanb-blog/issues

[cppreference-declarations]: https://en.cppreference.com/w/cpp/language/declarations
[so-thread]: https://stackoverflow.com/questions/1143262/what-is-the-difference-between-const-int-const-int-const-and-int-const
