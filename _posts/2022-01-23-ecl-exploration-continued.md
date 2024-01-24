---
layout: post
title:  "ECL Exploration: Continued"
date:   2024-01-23 23:00:00 +1300
categories: code lisp
published: true
---

Exploring the embedding and interop features of ECL. Continued.

[ *Check out all posts in "lisp" series [here][series-lisp].* ]

[series-lisp]: {% link series.md %}#lisp

In my [previous post][int-lisp-2], I started exploring Embeddable Common-Lisp. I was looking into [this example from the ECL repo][ecl-repo-example-embed] as a starting point. I will share some of my learning so far.

The initial integration into a C/C++ library is pretty straightforward:[^1]
- There are [various ways to build the lisp code][ecl-sys-bld], but I focused on the [`asdf`][wikipedia-asdf] based [method][ecl-sys-bld-asdf]. You just add a build rule that's a few lines long. It triggers `ecl` to build a (static or shared) library from an ASDF based system you point at.
- As part of the build rule, you also declare an "entry point" to generate. This entry point will be used to initialize the library later.
- On the C/C++ side, you boot ecl, and call the library init function you just created.
- You are now ready to interact with the system.
- While building your C/C++ code, you link against the library you created earlier.

The API that ecl exports is pretty rich. It has:
- C versions of [many][ecl-only-in-cl] CL functions, which are `cl_` prefixed.
- Functions for interop-related functionality, convenience etc. These are `ecl_` prefixed.

[The naming conventions page][ecl-naming-conventions] in docs have the details.

## CL Object

One interesting concept is the `cl_object`, which is [the box for CL values][ecl-overview-c-ref]. The definition:
``` C
typedef union cl_lispunion *cl_object;
```

Internals are detailed [here][ecl-object-representation]. The type `cl_lispunion` is essentially a tagged `union`. *But I am planning to start a series on methods used for boxing values in dynamic languages, anyway. So I will move on for now.*

The `cl_object` is the input and the return type of most C side APIs. Obviously, the requirement to explicitly generate `cl_object`s for values makes it a slightly verbose to use the APIs from C side, but that is expected.

I am currently trying to have a better grasp of the [memory management][ecl-overview-memory-management] aspect.

## Defun Preprocessor

While navigating the ECL codebase, I noticed that the `src/c` directory is full of files with `.d` extension. It took me a while to figure out that these source files are intended to be processed by the tool called ["defun preprocessor"][ecl-defun-preprocessor] (`dpp`), which generates the corresponding C sources.

So those files are mostly normal C, with some parts that use special syntax to be expanded by this preprocessor, to C. Here is [an example line from "src/c/file.d"][ecl-repo-d-file-example]:
``` c
  if (b == @':eof') b = ECL_NIL;
```

Above expands to a C form:
``` c
  if (b == ECL_SYM(":EOF",1256)) b = ECL_NIL;
```

And `ECL_SYM` macro just indexes an array called `cl_symbols` using the index (second argument) arg.

Another example: this [`defun` declaration][ecl-repo-d-read-example] expands to a function with the prototype below:
``` c
cl_object cl_read(cl_narg narg, ...)
```

Cool!

Thanks for reading! If you find technical errors, please report in the blog's [Issues page][report].

[^1]: At least for the method I focused on.

[report]: https://github.com/kenanb/kenanb-blog/issues

[int-lisp-2]: {% post_url 2024-01-21-exploring-embeddable-common-lisp %}

[ecl-repo-example-embed]: https://gitlab.com/embeddable-common-lisp/ecl/-/tree/develop/examples/embed
[ecl-repo-d-file-example]: https://gitlab.com/embeddable-common-lisp/ecl/-/blob/b9a7d06635b4defad4f8af93291d60e235488a74/src/c/file.d#L1196
[ecl-repo-d-read-example]: https://gitlab.com/embeddable-common-lisp/ecl/-/blob/b9a7d06635b4defad4f8af93291d60e235488a74/src/c/read.d#L1465

[wikipedia-asdf]: https://en.wikipedia.org/wiki/Another_System_Definition_Facility
[ecl-sys-bld]: https://ecl.common-lisp.dev/static/files/manual/current-manual/System-building.html
[ecl-sys-bld-asdf]: https://ecl.common-lisp.dev/static/files/manual/current-manual/System-building.html#Compiling-with-ASDF
[ecl-overview-c-ref]: https://ecl.common-lisp.dev/static/files/manual/current-manual/Overview.html#C-Reference-1
[ecl-overview-memory-management]: https://ecl.common-lisp.dev/static/files/manual/current-manual/Memory-Management.html#Memory-Management
[ecl-only-in-cl]: https://ecl.common-lisp.dev/static/files/manual/current-manual/Overview.html#Only-in-Common-Lisp
[ecl-naming-conventions]: https://ecl.common-lisp.dev/static/files/manual/current-manual/Overview.html#Naming-conventions
[ecl-object-representation]: https://ecl.common-lisp.dev/static/files/manual/current-manual/Manipulating-Lisp-objects.html#Objects-representation
[ecl-defun-preprocessor]: https://ecl.common-lisp.dev/static/files/manual/current-manual/Defun-preprocessor.html
