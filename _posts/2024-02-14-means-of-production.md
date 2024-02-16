---
layout: post
title:  "Means of Production"
date:   2024-02-14 23:00:00 +1300
categories: code lisp
published: true
---

I have been working on a personal framework for my OpenUSD related experiments, mainly in Lisp.

[ *Check out all posts in "lisp" series [here][series-lisp].* ]

[series-lisp]: {% link series.md %}#lisp

I was trying to hit the first milestone in my latest project, called `mopr`, so I couldn't write a post for the last few days.

I just open-sourced the code of this project, but **it is heavily experimental, buggy, untested, unsafe, and completely undocumented. The experimental ideas I will describe in this post are similarly unsafe. So I really don't recommend trying any of this.[^1]** I am just sharing it as an intellectual activity.

[The repository is here][github-mopr]. It is currently under a [BSD-3-Clause license][mopr-license].

The acronym `mopr` is based on a reference to the economics term: "[means of production][wikipedia-means-of-production]". I found the name suitable, since I am using `mopr` for experimenting with visual production related ideas.

This is a monolithic personal framework for my own VFX related experiments. So its characteristics will evolve over time, and it will likely remain **heavily experimental, buggy, untested, and unsafe**.

## What Can It Do

Not a lot of interesting things just yet. It has the foundations that allow me to do [OpenUSD][openusd-webpage]-focused prototyping, mostly using [Common Lisp][wikipedia-common-lisp]:
- Very minimal and incomplete C and Lisp access to USD APIs like Layer, Stage, Prim.
- Support for generic data transfer from native Lisp arrays to USD's `VtValue` containers.
- Boilerplate setup to allow integrating the code into OpenUSD plugins. This kind of integration is currently specific to Embeddable Common Lisp.

But this effort already enabled one prototype, that I call USDS.

### USDS - Boring Parts

I do a lot of testing with USD content, and composition of content, usually to observe the outcome, confirm my intuition etc. I value being able to quickly prototype test data.

So the one actual feature I started implementing, is a declarative scene description language I call USDS, that allows me to populate a USD layer from native [S-Expression][wikipedia-s-expression] (s-exp) forms, without explicitly calling USD APIs. The information stored is similar to `usda` format, but the representation allows symbolic manipulation. So it is more like what you would have if USDA was based on a generic serialization format.

I have support for scene graph tree building, and most attribute types. No metadata, relationship, or timecoded attribute support yet. I also have an extension that allows more concise syntax, but it comes with its own limitations, so I will skip that for now.

[`data/test` module][github-mopr-data-test] has some examples. `02.lisp` looks like this:
``` common-lisp
((:tree
  ("hello"
   ("world")))

 (:prim ("hello")
        (:type Xform))

 (:prim ("hello" "world")
        (:type Sphere)))
```

When processed, this expression would produce usd that corresponds to `usda` below:
```
#usda 1.0

def Xform "hello"
{
    def Sphere "world"
    {
    }
}
```

Clearly, the `usda` form is more concise in this case. But I can directly manipulate/generate the s-exp form in Lisp.

One difference is, the USDS syntax separates the scene graph tree description (`:tree` forms) from the definition of prims (`:prim` forms), because I find it error-prone and tedious to write USD attributes inside 5 levels of nesting, when I write USDA by hand for testing.

I can also define "aliases" for prim paths. Here, `x` is an alias, that's later used to refer to the prim instead of by path "`/b/d/e`".
``` common-lisp
((:tree
  ("a" :spec :class)
  ("b"
   ("d"
    ("e" :spec :over :alias x)))
  ("c"))

 (:prim x
        (:type Sphere))

 (:prim ("a")))
```

This corresponds to a layer that consists of prims at: `</a>`, `</b>`, `</b/d>`, `</b/d/e>`, and `</c>`.

Also, multiple declarations targeting same entities is supported, which is useful when programmatically building parts of a prim or tree from independent functions.

### Cooler Stuff

OK, let's move on to slightly cooler stuff: I can register `callables` that I am able to trigger from within the USDS declarations.

Example: An expression like below will call the registered function `test-gen-cubes` with `30` as its only argument:
``` common-lisp
((:call :test-gen-cubes 30))
```

Obviously, **what I am about to say would be a very unsafe thing to do**, but if I were to then plug USDS into a file-format plugin, and opened the file that has the above form in usdview, I would see:
![USDS data displayed in usdview.](/assets/img/usdview-mopr-00.png){: width="740" }

**But again, this is a very bad idea.** I only added support for callables to trigger calls from within a live programming context, where it is clear that some arbitrary execution might take place.

Anyway, `callables` are expected to be just usual Lisp functions that return USDS forms themselves, which then get processed normally. (*But there is no mechanism in-place to sandbox the function being called, or prevent side effects.*) *Conceptually*, the return value acts as a list that is **spliced** into the location of the `:call` form.

Example callable `test-gen-cubes` (called with arg `N`) produces a grid of `N`x`N` cubes, each looking like below:
```
def Cube "Prim_0002"
{
    double size = 0.5
    double3 xformOp:rotateXYZ = (0, 2, 2)
    float3 xformOp:translate = (0, 1, 0)
    uniform token[] xformOpOrder = ["xformOp:translate", "xformOp:rotateXYZ"]
}
```

Here is an example that mixes hardcoded data with function calls at different levels: [09_call.lisp][09_call.lisp]

``` common-lisp
((:call :test-tree-gen)

 (:prim :x
        (:type Sphere)
        (:call :test-gen-xform-info
               #1A (10 0 100)
               #1A (33 55 4))
        (:prop "radius"
         :double #0A 5.0d0))

 (:call :test-gen-cubes 2))
```

This corresponds to the USDA file here: [09_call.usda][09_call.usda]

In this last example, we have 3 `call` expressions. `test-gen-xform-info` is a "prim-level" call, and it takes 2 arguments, both are "one-dimensional arrays" (`#1A`).[^4]

## Architecture

The way I currently integrate with USD roughly looks like this:
![USD integration over FFI.](/assets/img/mopr-ffi-diagram-00.png){: width="740" }

The current set of key modules in repo are:[^2]
- `wrap`: A very incomplete C API wrapper for some key OpenUSD modules, and minimal wrapper for miscellaneous classes.
- `lisp`: Common Lisp based implementation of various features building on the `wrap` module. This defines an [ASDF][asdf-webpage] system called `mopr`.

These 2 key modules currently build a generic API that aims to be Common Lisp implementation agnostic. So far, that's the case for the 2 Common Lisp implementations I am testing with: [SBCL][sbcl-webpage], and [ECL][ecl-webpage].

I am hoping to split this part to its own respository, once the OpenUSD API coverage is good enough (and tested enough) to be independently useful. For now, it only exposes the minimal API the rest of the framework needs.

`lisp` module mainly consists of submodules:
- `ffi`: Common Lisp foreign-function-interface for the API defined on `wrap`. This is using the ubiquitous [CFFI][cffi-webpage] system, and [cl-autowrap][github-cl-autowrap] library.
- `mopr`: An interface to interact with various USD concepts like the `VtValue` and `VtArray`, along with some `with-*` macros to implement RAII behaviour for foreign USD objects.
- `plug`: Placeholder to make the lisp system itself pluggable. It currently only registers a few hardcoded `callables`, so that the `call` expressions in the USDS declarations can trigger them.
- `usds`: This implements a `list` based declarative language to express scene description that can be used to populate an OpenUSD layer.

The remaining `lisp` submodules are not interesting.[^3]

On top of this, there are also some modules to allow testing this lisp-based interface in a practical fashion. Specifically, I currently use Embeddable Common Lisp based Linux-native builds of the Lisp module to integrate it into the OpenUSD runtime using USD's own plugin system:
- The `boot` module is built into a shared library that I use to initialize the `mopr` ECL modules. That becomes a dependency of the actual USD plugin library that will call into `mopr` lisp modules.
- `plug` module has an empty example USD "file format plugin" that integrates the `boot` module, and triggers some test code implemented in Lisp. So it doesn't really do anything useful.

`test` module implements some initial [`Catch2`][github-catch2] based testing. `data` module contains test files that I used to compare the `usds` forms to the reference USD ASCII format representation.

## Why Common Lisp

[I wrote about this a bit before][int-lisp-exploring-ecl]. Not for any critical reasons, but:
- It is very suitable for the kind of flexible prototyping goals I have.
- It is a mature language ecosystem with some very cool properties like "homoiconicity".
- Modern Lisp implementations find a good balance of dynamism and performance.

## Goals

Here are a few things I am planning to do with this.

### More Posts

Now that I have something that produces visuals, I am planning to write a series of blog posts with example procedural scenes first. (*Even though this is not the most scalable (or resource-friendly) kind of proceduralism for scene content generation, it is proceduralism nevertheless.*)

I am also planning to write up a bit about how I currently handle foreign objects etc.

I will add metadata etc. support soon. And hopefully, the C wrapper coverage will progressively increase. I am also planning to introduce an actual plugin interface to Lisp side.

### Threading

So far, I spent no time to investigate how integrating CL-based computations into a USD plugin would work if Lisp gets called from multiple threads. I need to look into that.

I really don't know if the OpenUSD-plugin - CommonLisp integration idea will hold well in practice.

If it does, I will likely move on to CL integration in other parts of USD, like in Hydra layer. Same fundamental data types are used on both layers, so I think it would be relatively straightforward to do similar interfacing in USD Hydra.

### Data Mapping and FFI Improvements

Binary encoding of primitives is usually different between the Lisp world and the C/C++ world. So I currently don't try to map or `memcpy` anything. I just iterate over the data blocks and let CFFI handle conversion. I didn't worry about performance so far anyway. But specific optimizations might be possible, if the data passing the language boundary ends up being the bottleneck.

Also, note that I am not too experienced with FFIs. So there might be issues with my approach to bridging C++ to Lisp.

### Other Implementations

Also, testing with other implementations: For now, I will continue focusing on ECL based integration. But I read that SBCL has early support for generating dynamic libraries. So I am planning to do some testing with that as well. Finally, once I am confident with the stability of various layers, I will return to investigating [Clasp][clasp-webpage] integration.

And since the USDS declarative syntax is basically standard s-expressions, I might test integration with a couple of [Scheme][wikipedia-scheme] implementations as well.

### Sandboxed Callables

I mentioned that it is sufficient for `callables` to be just pure functions that produce more USDS forms. So I would like to see if it would be possible to integrate with [WebAssembly][webassembly-website], and use it [as a sandbox][webassembly-website-security] for callables. Since WebAssembly has a text format that uses S-Expressions as well, there might be other synergy opportunities.

[webassembly-website]: https://webassembly.org/
[webassembly-website-security]: https://webassembly.org/docs/security/
[webassembly-text-format]: https://webassembly.github.io/spec/core/text/conventions.html

### Other Plans

I am hoping to make the `mopr` framework a testbed for a few other ideas I have been investigating:
- I previously started looking into [DWARF][dwarfstd-website] Debugging Standard. I would like to see how I can utilize DWARF for interesting runtime features, rather than just debugging. 
- SBCL has a really cool concept called *virtual operations* (VOPs). This is another thing I have been collecting notes on, and I want to see if I can make creative use of that.

I am hoping to soon write about VOPs and DWARF anyway.

That's all for today. I hope the post wasn't too boring.

Thanks for reading! If you find technical errors, please report in the blog's [Issues page][report].

[report]: https://github.com/kenanb/kenanb-blog/issues

[^1]: See [About][about] and [project license][mopr-license] disclaimers.
[^2]: There are the `base` and `core` modules, which, despite the names, contains nothing interesting. `base` consists of small headers that are needed by many modules. `core` is a placeholder library for future functionality I expect to be implementing in C.
[^3]: I also define a `mopr-user` lisp package to be used for the interactions with this API.
[^4]: For the 1D case, I could have just written `#( ... )` instead of `#1A( ... )`.

[about]: {% link about.md %}
[mopr-license]: https://github.com/kenanb/mopr/blob/main/LICENSE
[int-lisp-exploring-ecl]: {% post_url 2024-01-21-exploring-embeddable-common-lisp %}

[github-mopr]: https://github.com/kenanb/mopr
[wikipedia-means-of-production]: https://en.wikipedia.org/wiki/Means_of_production
[wikipedia-s-expression]: https://en.wikipedia.org/wiki/S-expression
[github-mopr-data-test]: https://github.com/kenanb/mopr/tree/main/src/data/test

[openusd-webpage]: https://openusd.org/release/index.html
[wikipedia-common-lisp]: https://en.wikipedia.org/wiki/Common_Lisp

[09_call.lisp]: https://github.com/kenanb/mopr/blob/c6bd7ff02fc83501ec5143278b27ba3b30070789/src/data/test/09_call.lisp
[09_call.usda]: https://github.com/kenanb/mopr/blob/c6bd7ff02fc83501ec5143278b27ba3b30070789/src/data/test/09_call.usda
[clasp-webpage]: https://clasp-developers.github.io/

[wikipedia-homoiconicity]: https://en.wikipedia.org/wiki/Homoiconicity
[sbcl-webpage]: https://www.sbcl.org/
[ecl-webpage]: https://ecl.common-lisp.dev/
[cffi-webpage]: https://cffi.common-lisp.dev/
[asdf-webpage]: https://asdf.common-lisp.dev/
[github-cl-autowrap]: https://github.com/rpav/cl-autowrap
[github-catch2]: https://github.com/catchorg/Catch2.git
[wikipedia-scheme]: https://en.wikipedia.org/wiki/Scheme_(programming_language)
[dwarfstd-website]: https://dwarfstd.org/
