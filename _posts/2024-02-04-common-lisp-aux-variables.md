---
layout: post
title:  "Common Lisp AUX Variables"
date:   2024-02-04 22:00:00 +1300
categories: code lisp
published: true
---

AUX lambda list keyword allows defining variables in the function lambda list.

[ *Check out all posts in "lisp" series [here][series-lisp].* ]

[series-lisp]: {% link series.md %}#lisp

I made a brief intro to lambda lists (parameter lists) of CL in [my previous post][int-lisp-lambda-lists].

The lambda list of Common Lisp has an interesting feature that I haven't encountered in other languages. It is called an [`&aux` variable][clhs-aux].

Basically, it allows you to define function variables in the parameter list. So it is not an actual parameter. You can't provide an argument that will be bound to the auxiliary variable while calling the function. Rather, it is analogous to a variable binding that would be defined within the function body, using a `let*` form.

For reference: [`let` and `let*` operators][clhs-let] allow creating new variable bindings with a scope. `let*` variant performs the bindings sequentially, so each binding can refer to previously defined variables.

Let's start with a simple function definition that doesn't have an auxiliary variable:

``` common-lisp
(defun fn (a b)
  (* a b))
```

We just have a function `fn` that accepts parameters `a` and `b`. It returns the result of multiplying `a` and `b`. Calling `(fn 2 3)` would return `6`.

Let's modify this to have an `&aux` variable called `c`, bind it to `a + b`, and introduce `c` into the multiplication:
``` common-lisp
(defun fn (a b &aux (c (+ a b)))
  (* a b c))
```

And here is the corresponding function using the usual `let*` form instead:

``` common-lisp
(defun fn (a b)
  (let* ((c (+ a b)))
    (* a b c)))
```

As expected, `(fn 2 3)` will return `30` for both of these.

So why is `&aux` useful?

`&aux` is not a very popular feature of the language. The `let*` form above is how you would usually see local variables like this get introduced. Most uses of `&aux` are convenience, based on personal preference. For example, you can use it to reduce one level of indentation.

There is an [old `comp.lang.lisp` discussion on `&aux`][comp-lang-lisp-aux], where people shared a few other interesting uses.

Examples of convenience uses:

- Some use it for cases where they will possibly promote a local variable to a formal parameter later. That use allows the parameter "promotion" to happen without modifying the function body.
- Some people use &aux variables to emphasize an important local variable, like the variable that's populated in function body, and eventually becomes the return value.
- Some use it for variables that are directly derived from function arguments, like the `(c (+ a b))` example above. That makes the relationship obvious.

Examples of more important usecases (these will only be clear to people who already know Common Lisp):
- Non-trivial initialization of structure slots, [which is also defined in the standard][clhs-boa].
- If a macro introduces lexical bindings, introducing it via a `let*` form will cause possible *declaration* forms in the macro call to be placed in the wrong position in macro expansion. An `&aux` variable will avoid that.

I like `&aux`. I occasionally use it for each reason stated above.

Thanks for reading! If you find technical errors, please report in the blog's [Issues page][report].

[report]: https://github.com/kenanb/kenanb-blog/issues

[int-lisp-lambda-lists]: {% post_url 2024-02-03-common-lisp-lambda-lists %}

[clhs-aux]: https://www.lispworks.com/documentation/lw50/CLHS/Body/03_dae.htm
[clhs-let]: https://www.lispworks.com/documentation/lw70/CLHS/Body/s_let_l.htm
[clhs-boa]: https://www.lispworks.com/documentation/lw50/CLHS/Body/03_df.htm

[comp-lang-lisp-aux]: https://groups.google.com/g/comp.lang.lisp/c/WGyjmda34LQ
