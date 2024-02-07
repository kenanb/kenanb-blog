---
layout: post
title:  "Common Lisp Lambda Lists"
date:   2024-02-03 22:00:00 +1300
categories: code lisp
published: true
---

Brief and informal introduction to parameter lists of CL.

[ *Check out all posts in "lisp" series [here][series-lisp].* ]

[series-lisp]: {% link series.md %}#lisp

Common Lisp parameter lists have some interesting features I haven't encountered in other languages. But before I get to  those, I will start by introducing the usual argument passing styles in CL.

Let's start with a simple function definition:

``` common-lisp
(defun mul (a b)
  (* a b))
```

In CL, parameter lists are officially called ["lambda list"][clhs-glo-lambda-list]. So `(a b)` is the lambda list here.

There are [various types of lambda lists][clhs-lambda-lists]. The one used by functions is called ["ordinary lambda list"][clhs-ordinary-ll]. Macro definitions use an "extended lambda list".

You have all the usual parameter styles that you would be familiar from languages like Python. You have *required*, *optional*, *keyword*, as well as a *rest* (catchall) style parameter. The way these are declared is slightly unconventional. The way they behave (especially the way combinations behave) is also different from other languages.

Each parameter declaration is called a "parameter specifier". Specifiers of the same style are grouped together. There are "lambda list keywords" that describes the kind of each group.

Here is an example with optional positional parameters (`...` is where function body would go):
```
                __ required parameters
               /               __ optional parameters
           ___/          _____/
(defun fn (a b &optional c d e) ... )
                   \
                    \_ A lambda list keyword.
```

This can be called like:
``` common-lisp
(fn 1 2)     ; c = nil, d = nil, e = nil
(fn 1 2 3 4) ; c = 3, d = 4, e = nil
```

So everything you declare before the first lambda-list keyword are required parameters.

Lambda list keywords start with ampersand character, like: `&optional`.

Another example, this time using a `&rest` parameter:
```
                __ required
               /          __ all other positional arguments
           ___/      ____/   gathered into a list
(defun fn (a b &rest args) ... )
```

This can be called like:
``` common-lisp
(fn 1 2)             ; args = nil
(fn 1 2 3)           ; args = '(3)
(fn 1 2 3 4 5 6 7 8) ; args = '(3 4 5 6 7 8)
```

And the last one, using keyword parameters:
```
                __ required
               /        __ keyword arguments
           ___/     ___/
(defun fn (a b &key c d) ... )
```

This can be called like:
``` common-lisp
(fn 1 2)      ; c = nil , d = nil
(fn 1 2 :d 4) ; c = nil , d = 4
```

You can combine different groups but only a subset of combinations build an intuitive protocol.

I will continue describing some interesting aspects in future posts, but you can read a great description of details from the [functions section][pcl-functions] of Practical Common Lisp.

Thanks for reading! If you find technical errors, please report in the blog's [Issues page][report].

[report]: https://github.com/kenanb/kenanb-blog/issues

[clhs-glo-lambda-list]: https://www.lispworks.com/documentation/lw50/CLHS/Body/26_glo_l.htm#lambda_list
[clhs-lambda-lists]: https://www.lispworks.com/documentation/lw50/CLHS/Body/03_d.htm
[clhs-ordinary-ll]: https://www.lispworks.com/documentation/lw50/CLHS/Body/03_da.htm
[pcl-functions]: https://gigamonkeys.com/book/functions
