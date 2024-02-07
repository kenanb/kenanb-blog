---
layout: post
title:  "Common Lisp Supplied-p Parameters"
date:   2024-02-05 22:00:00 +1300
categories: code lisp
published: true
---

In CL, you can check if the optional and keyword arguments are actually supplied.

[ *Check out all posts in "lisp" series [here][series-lisp].* ]

[series-lisp]: {% link series.md %}#lisp

I made a brief intro to lambda lists (parameter lists) of CL in [a previous post][int-lisp-lambda-lists].

I wanted to mention another interesting feature of the lambda list of Common Lisp. We already mentioned that CL supports [`&optional`][clhs-opt], and [`&key`][clhs-key] parameters. It also allows both of these parameter groups to have default values.

Example:

``` common-lisp
(defun fn (a b &optional (c 5))
  (* a b c))
```
The optional parameter `c` has a default value of `5`.

Let's try to call this:
``` common-lisp
(fn 3 4 7) ; = 84
(fn 3 4)   ; = 60
```

Now, the cool part. You can also check whether the caller actually supplied an optional parameter, or if the value defaulted.

In languages like python, people sometimes default to an intermediate value, like `None`, that's not a sensible value for that specific parameter, just to be able to distinguish between defaulted value vs a caller-supplied value that happens to match the default.

In CL, you don't need that. You can supply a variable name to be bound to *true* or *false*, depending on whether or not an argument was supplied for the parameter during call.

``` common-lisp
(defun fn (&optional (x 0 x-supplied-p))
  (format t "Value of x: ~a~%Caller supplied x: ~a"
          x
          (if x-supplied-p "YES" "NO")))
```

Here is the output of various calls:
```
CL-USER> (fn 5)
Value of x: 5
Caller supplied x: YES
NIL
CL-USER> (fn 0)
Value of x: 0
Caller supplied x: YES
NIL
CL-USER> (fn)
Value of x: 0
Caller supplied x: NO
NIL
```

Thanks for reading! If you find technical errors, please report in the blog's [Issues page][report].

[report]: https://github.com/kenanb/kenanb-blog/issues

[int-lisp-lambda-lists]: {% post_url 2024-02-03-common-lisp-lambda-lists %}

[clhs-opt]: https://www.lispworks.com/documentation/lw50/CLHS/Body/03_dab.htm
[clhs-key]: https://www.lispworks.com/documentation/lw50/CLHS/Body/03_dad.htm
