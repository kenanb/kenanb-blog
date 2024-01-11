---
layout: post
title:  "On X-Macros and the Preprocessor"
date:   2024-01-10 22:00:00 +1300
categories: code c-cpp
published: true
---

Some notes on preprocessor usage and the classic X-macro with examples.

[ *Check out all posts in "c-cpp" series [here][series-c-cpp].* ]

[series-c-cpp]: {% link series.md %}#c-cpp

Yesterday, I started the topic of [interop between C/C++ and Assembly][int-prev], but I wanted to jump back to something more familiar today. The preprocessor.

[X macros][wikipedia-x-macro] is a classic, and very useful C/C++ preprocessor macro idiom.

It is useful when you need to declare/define program constructs based on the same list of entities, multiple times. Wikipedia says:

> They provide reliable maintenance of parallel lists whose corresponding items must be declared or executed in the same order.

This is a great description of the key aspects. But what's an X-macro, in practice?

In case you haven't read it up from the wikipedia link already, one variant is this:
- You define a macro (let's call that `PROCESS_ELEMENTS`) that applies another macro (let's call that `X`), consecutively, one-by-one, to various values.
- Then you repeat below as many times as you need:
  - You define (and redefine) the macro `X`.
  - You call `PROCESS_ELEMENTS` (again, usually in a different context than the previous time).

So an almost pseudocode version looks something like:
``` c
#define PROCESS_ELEMENTS X(a) X(b) X(c)

/* Some context. */
#define X(v) bar(v);
PROCESS_ELEMENTS
#undef X

/* Some other context. */
#define X(v) baz(v);
PROCESS_ELEMENTS
#undef X
```

[Alternatively][wikibooks-x-macros], instead of defining a `PROCESS_ELEMENTS` macro, you can put the list of `X` calls into another file, and then just `#include` that file instead of calling a `PROCESS_ELEMENTS` macro.[^1] This is the variant we will show later in this post.

## An Example

A few years ago, I was following [this Vulkan Tutorial][vulkan-tutorial], which heavily relies on C++ features. (Heh, I still need to finish that tutorial.)

I wanted to challenge myself to write it in C, and also find C-only solutions that will still keep the code concise.

There is a lot of error handling involved in the Vulkan setup, for which you want to have at least developer friendly reporting. I can't quite remember how the tutorial approached it. I can see that it has a mix of exceptions being thrown, along with some error code returns.[^2]

I am restricted to what C provides. I want to pass around an error code, and I probably want to generate a log as part of the handling. So, more concretely, you could say: I need an enumeratad type, and a message **associated with** every possible value of such type.[^3]

Associating a value (the enum value) with another (the message): That's an area we can utilize X-macros. I believe I first saw this usage in this GDC 2015 talk ["The Simplest AI Trick in the Book"][gdc-ai-trick] except it was based on a string array instead of a `switch`.

## A Tangent

I am sure we can imagine a few other ways you can associate the enum and the string. An X-macro is just one of the easiest methods to keep the underlying structures very minimalistic, while providing the necessary association.

I usually prefer these kind of solutions to use very simple abstractions, if that's not too inconvenient.

Not for performance, in this case. However, **the simpler the abstraction, the easier it is to debug the code**, at any level. I value that, a lot!

Something like error handling routines will show up in many functions, multiple times in the function body. So if it builds on a simple solution, it will be easy to work with values in debugger, print them, step through generated assembly.

Obviously, this is a choice I am making based on this being a solo project. Decisions for a codebase that's maintained by a team should be balanced based on the practices of the team. That significantly reduces the chances of "assembly debugging experience" being a major criterea. :)

This is also why it is good practice to do both solo and team projects. You gain different perspectives.

## Back to the Topic

I use the term "condition". I like the term from Lisp. No other reason.

So I ended up having an `error.codes` file. That looks like this:[^4]

``` c
CONDITION( GENERIC_FAILURE,
           "Something failed. No information." )
CONDITION( GLFW_INIT_FAILED,
           "GLFW error during initialization!" )
CONDITION( GLFW_WINDOW_FAILED,
           "GLFW error during window creation!" )
CONDITION( NO_VULKAN,
           "Vulkan not found!" )
CONDITION( UNSUPPORTED_EXTENSION,
           "Required extension not available." )
CONDITION( UNSUPPORTED_VLAYER,
           "Requested layer not available." )
CONDITION( VK_INSTANCE_FAILED,
           "Failed to create Vulkan instance!" )

/*              ... MORE ERRORS HERE ...                   */
```

Next, the actual source file. Let's call this one: `xmacro.c`

This would bring in `error.codes` file in various places:

``` c
#include <stdio.h>
#include <stdlib.h>

/* /////////////////////////////////////////////////////// */
/* UTILITY                                                 */

#define NSX( NS, X ) NS##X
#define NS( NS, X ) NSX( NS, X )

#define STRX( X ) #X
#define STR( X ) STRX( X )
```

Condition related definitions.
``` c
/* /////////////////////////////////////////////////////// */
/* CONDITION HANDLING                                      */

/* Namespacing the condition values. */
#define COND_NS FOO_CONDITION_

/* Used to refer to enums. */
#define cond_( X ) NS( COND_NS, X )

/* Get enum symbol as string. */
#define COND_STR( X ) STR( cond_( X ) )

/* Condition enum. */
typedef enum Condition
{
    cond_( SUCCESS ) = 0,

#define CONDITION( C, M ) cond_( C ),
#include "error.codes"
#undef CONDITION

    /* Non-error. Value to use in loop expressions. */
    cond_( END ),
} FooCondition_e;

/* Generate error message for given condition. */
const char *
 fooReport( FooCondition_e cond )
{
    switch ( cond )
    {

#define CONDITION( C, M )                                   \
    case cond_( C ):                                        \
        return M " ( Code: " COND_STR( C ) " )\n";
#include "error.codes"
#undef CONDITION

        default:
            return "SUCCESS.";
    }
}
```

And here is how this is intended to be used:
``` c
/* /////////////////////////////////////////////////////// */
/* MAIN                                                    */

/* Example function returning a condition. */
FooCondition_e
 fooTest( )
{
    return cond_( GLFW_WINDOW_FAILED );
}

int
 main( )
{
    FooCondition_e cond = fooTest( );

    /* Condition usage example. */
    if ( cond )
    {
        fprintf( stderr, "%s\n", fooReport( cond ) );

        /* Handle relevant conditions. */
        switch ( cond )
        {
            case cond_( GLFW_WINDOW_FAILED ):
            {
                fprintf( stderr, "Printing extra information here.\n" );
                break;
            }
            case cond_( UNSUPPORTED_EXTENSION ):
            {
                fprintf( stderr, "Doing other things here.\n" );
                break;
            }
            default:
                break;
        }

        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
```

I am testing this on Linux. Compiling this code using gcc generates `xmacro` executable:
``` shell
gcc -o xmacro xmacro.c
```

And when we run `./xmacro` we get the output below with a return value of `1`:

```
GLFW error during window creation! ( Code: FOO_CONDITION_GLFW_WINDOW_FAILED )

Printing extra information here.
```

Obviously, this is a lot of macros already, and the real code has many more. I intentionally pushed the macrology a bit in that experiment to see where it leads.

That resulted in an interesting set of simple macro-based abstractions to also pass around and handle the errors. Now, was it actually a good abstraction? I don't know. I didn't get a chance to test it properly. So I will reserve further details for a future post.

## A Macro Error

Now, imagine there is an error, and it looks like a macro related issue. I once read [VSCode can do C/C++ macroexpansion nowadays][vscode-expand] so maybe this is not a problem for most programmers. (I use Emacs.) But in case that's not solving it for you, GCC can help.

After all, you can ask GCC to only preprocess the source file, instead of compiling it, using the [`-E` option][gcc-options].

For debugging macros and preprocessing in general, I usually also add [`-P` and `-H` preprocessor options][gcc-preprocessor-options] as well:

> -P : Inhibit generation of linemarkers in the output from the preprocessor. ...

> -H : Print the name of each header file used, in addition to other normal activities. ...

Of course, preprocessing includes processing of all directives. Therefore all `#include`s will be included, usually generating an extremely large file. So I usually then filter the output (with `sed`) to trim the output till a certain pattern match. I won't explain how to do that, as I am not good with `sed`. [This][sed-tutorial] seems like a good document.

Anyway, let's say I made an error. I forgot the comma at the end of the `CONDITION` macro definition.

``` c
/* BROKEN CODE SNIPPET                      */
typedef enum Condition
{

#define CONDITION( T, C ) cond_( C )
/*                                  ^       */
/*                       MISSING COMMA HERE */
/* BROKEN CODE SNIPPET                      */
```
Now my code doesn't compile and it generates a very confusing error.

If I dump the preprocessor output:
``` shell
gcc -E -P xmacro.c -o xmacro.i
```

I can see the generated `xmacro.i` file contains this broken definition:
``` c
/* EXPANSION OF BROKEN CODE SNIPPET */
typedef enum Condition
{
    FOO_CONDITION_SUCCESS = 0,
    FOO_CONDITION_GENERIC_FAILURE
    FOO_CONDITION_GLFW_INIT_FAILED
    FOO_CONDITION_GLFW_WINDOW_FAILED
    FOO_CONDITION_NO_VULKAN
    FOO_CONDITION_UNSUPPORTED_EXTENSION
    FOO_CONDITION_UNSUPPORTED_VLAYER
    FOO_CONDITION_VK_INSTANCE_FAILED
    FOO_CONDITION_END,
} FooCondition_e;
/* EXPANSION OF BROKEN CODE SNIPPET */
```

## The End

I guess that's it. The example code is [shared in blog's repository][example]. If you find technical errors, please report in the blog's [Issues page][report].

I am not really a fan of C preprocessor. Coming from Lisp, this is just a very rudimentary "macro" functionality.

However, it is there, it is standard, and it is ubiquitous. And that makes it very useful.

Thanks for reading!

[report]: https://github.com/kenanb/kenanb-blog/issues


[^1]: Note that this is completely different than the conventional "header" file pattern. After all, an `#include` is an `#include`. It doesn't care if it is including a header or some other "source content", [as long it consists of complete tokens][gcc-include].
[^2]: I am not talking about the "debug messenger callback" part, which I seem to have kept verbatim.
[^3]: Note that this error type will contain most possible program setup errors, not only Vulkan related.
[^4]: I am simplifying the actual usage a bit, to keep it focused.

[int-prev]: {% post_url 2024-01-09-c-assembly-interop %}

[example]: https://github.com/kenanb/kenanb-blog/tree/main/example/2024-01-10-on-xmacros-and-preprocessor

[wikipedia-x-macro]: https://en.wikipedia.org/wiki/X_macro
[wikibooks-x-macros]: https://en.wikibooks.org/wiki/C_Programming/Preprocessor_directives_and_macros#X-Macros
[gcc-include]: https://gcc.gnu.org/onlinedocs/cpp/Include-Operation.html
[gcc-options]: https://gcc.gnu.org/onlinedocs/gcc/Overall-Options.html
[gcc-preprocessor-options]: https://gcc.gnu.org/onlinedocs/gcc/Preprocessor-Options.html
[vulkan-tutorial]: https://vulkan-tutorial.com/
[vscode-expand]: https://devblogs.microsoft.com/cppblog/visual-studio-code-c-july-2021-update-disassembly-view-macro-expansion-and-windows-arm64-debugging/
[sed-tutorial]: https://www.grymoire.com/Unix/Sed.html
[cl-condition-handling]: https://www.nhplace.com/kent/Papers/Condition-Handling-2001.html
[gdc-ai-trick]: https://youtu.be/iVBCBcEANBc?feature=shared&t=200
