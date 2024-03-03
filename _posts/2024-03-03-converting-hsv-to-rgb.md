---
layout: post
title:  "Converting HSV to RGB"
date:   2024-03-03 23:00:00 +1300
categories: code cg
published: true
---

Exploring HSV to RGB conversion for fun.

I will talk a little bit about the most straightforward HSV to RGB conversion algorithm, and I will try to iterate on it, for better or worse. I don't think the result of my iteration has any significance, because:
- Most cases where the performance of HSV to RGB conversion matters probably involve GPU shader-based implementations.
- There are already well known alternative HSV to RGB implementations that I assume are much more suitable for GPUs than the approach I am iterating on.

So this post is just documenting a programming puzzle I spent time on, for fun. I will mostly discuss using pseudo-code. But I might have a closer look at performance implications of various implementations in follow-up posts.

## Intro

Common algorithms for HSV to RGB conversion is documented in [HSL and HSV][wikipedia-hsv-to-rgb] Wikipedia page.

I spent some time exploring the first algorithm. It is simple, and it involves an interesting `switch` that makes me want to replace with a different approach.

There are two aspects to it:

- Calculating a set of values to be assigned to `R`, `G`, and `B` components.
- Distributing those values to each component.

Latter aspect shows up in the description as:
```
(C, X, 0) if 0 <= H' < 1
(X, C, 0) if 1 <= H' < 2
(0, C, X) if 2 <= H' < 3
(0, X, C) if 3 <= H' < 4
(X, 0, C) if 4 <= H' < 5
(C, 0, X) if 5 <= H' < 6
```

Here, `H'` corresponds to a particular factor of *hue*, depending on how we represent hue.

If `H` is represented in degrees in range `[0, 360)`, then `H'` is:

```
H' = H / 60
```

If we were to represent `H` in range `[0, 1]`, then:
```
H' = H * 6
```

`C` is *chroma*, where `V` is *value*, and `S` is *saturation*:
```
C = V x S
```

`X` doesn't directly map to a term I know of. It is the second largest component, and its formula is:

```
X = C x (1 - |H'mod2 - 1|)
```

Once the right component ordering is found, each of those values will then be summed with `V - C` to get the final `RGB` values.

### Example

Let's manually try this with the HSV values below, all values being in [0,1] range:
```
H=0.6
S=0.8
V=0.7
```

Here is what the calculation would look like:
```
H' = 3.6
C = 0.8 * 0.7 = 0.56
X = 0.56 * ( 1 - |1.6-1|) = 0.56 * 0.4 = 0.224
```
Since `H'` is `3.6`, base values of components is: `(0, X, C)`. Using this, we can calculate `RGB`:

```
R = 0.7 - 0.56 + 0     = 0.14
G = 0.7 - 0.56 + 0.224 = 0.364
B = 0.7 - 0.56 + 0.56  = 0.7
```

Since the ordering of the `X`, `C`, and `0` values for each case doesn't follow a very trivial pattern, the implementations I see usually have an actual `switch` statement. In fact, two variables are used instead of `X`, representing the two possible expansions of the absolute-value term.

Let's assume `F` to be the *fractional part* of `H'`:
```
X0 = C * F
X1 = C * (1 - F)
```

For this variant, distribution of values becomes:
```
(C , X0, 0 ) if 0 <= H' < 1
(X1, C , 0 ) if 1 <= H' < 2
(0 , C , X0) if 2 <= H' < 3
(0 , X1, C ) if 3 <= H' < 4
(X0, 0 , C ) if 4 <= H' < 5
(C , 0 , X1) if 5 <= H' < 6
```

Revising the previous example:

`H'` is `3.6`, base values of components is now: `(0, X1, C)`. Everything else being the same:
```
X1 = 0.56 * ( 1 - 0,6 ) = 0.224
```

In this form, the pattern of how these 4 possible values are ordered for different values of `H'` is even more obfuscated.

## Revising the Algorithm

I was trying to find a way to rewrite this in a way to have some kind of ordering pattern. Specifically, a pattern that's simple enough that I can implement without a `switch`. One approach I found is this:

Instead of assuming `C` as a constant, and selecting either `X0` or `X1`, we can rewrite the distribution of components so that, both `X0` and `X1` always gets assigned to a component, but in each case, either `X0` or `X1` equals `C`. Now, there is a clear cycling ordering of values: `X1`, `X0`, and `0`:

```
(X1, X0, 0 ) where X1 = C, if 0 <= H' < 1
(X1, X0, 0 ) where X0 = C, if 1 <= H' < 2
(0 , X1, X0) where X1 = C, if 2 <= H' < 3
(0 , X1, X0) where X0 = C, if 3 <= H' < 4
(X0, 0 , X1) where X1 = C, if 4 <= H' < 5
(X0, 0 , X1) where X0 = C, if 5 <= H' < 6
```

To achieve this, we can redefine `X0` and `X1` as below, where `H_ODD` is `true` if integer part of `H'` is *odd*:
```
H_ODD = H'mod2 >= 1
X0 = C * ( H_ODD ?     1 : F )
X1 = C * ( H_ODD ? (1-F) : 1 )
```

Adapting previous example again, the ordering is `(0, X1, X0)`, where:
```
H_ODD = true
X0 = 0.56 * 1 = 0.56
X1 = 0.56 * ( 1 - 0.6 ) = 0.224
```

Since we now have a clear pattern, we can assign each value to indices that are calculated based on arithmetic. Here is a C implementation of this approach:
``` c
void
 hsv_to_rgb( double const * const hsv, double * const rgb )
{
    const double h = ( hsv[ 0 ] - floor( hsv[ 0 ] ) ) * 6;
    const int h_int = ( int ) h;
    const double h_frac = h - h_int;
    const unsigned int h_shr1 = h_int >> 1;
    const int h_odd = h_int & 1;
    const double chroma = hsv[ 1 ] * hsv[ 2 ];
    const double svf = chroma * h_frac;
    rgb[ ( h_shr1 + 0 ) % 3 ] = hsv[ 2 ] - h_odd * svf;
    rgb[ ( h_shr1 + 1 ) % 3 ] = hsv[ 2 ] - !h_odd * ( chroma - svf );
    rgb[ ( h_shr1 + 2 ) % 3 ] = hsv[ 2 ] - chroma;
}
```

When I did some naive benchmarking, I did observe better performance with this approach compared to the `switch` based implementation. However, I am pretty sure other popular HSV to RGB approaches will be faster than this. (I also haven't really tested this implementation for edge cases.)

One alternative is actually [documented in the same Wikipedia page][wikipedia-hsv-to-rgb-alternative]. But I haven't spent much time to understand that algorithm yet. I might do that in a future post.

That's all for today. Thanks for reading! If you find technical errors, please report in the blog's [Issues page][report].

[report]: https://github.com/kenanb/kenanb-blog/issues
[wikipedia-hsv-to-rgb]: https://en.wikipedia.org/wiki/HSL_and_HSV#HSV_to_RGB
[wikipedia-hsv-to-rgb-alternative]: https://en.wikipedia.org/wiki/HSL_and_HSV#HSV_to_RGB_alternative
