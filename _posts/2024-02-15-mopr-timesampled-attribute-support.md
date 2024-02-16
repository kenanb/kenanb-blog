---
layout: post
title:  "Timesampled Attribute Support"
date:   2024-02-15 22:00:00 +1300
categories: code lisp mopr
published: true
---

Mopr now has initial support for timesampled attributes.

[ *Check out all posts in "lisp" series [here][series-lisp].* ]

[series-lisp]: {% link series.md %}#lisp

Short post today.

I wrote [about `mopr`][int-lisp-mopr] yesterday. I mentioned that `USDS` module supported generating attribute values for the `Default` timecode, but not for timesamples.

[int-lisp-mopr]: {% post_url 2024-02-14-means-of-production %}

I added the initial support for timesamples today.

I had to write a mini C wrapper for `UsdTimeCode`, since that's an object you need to pass as an argument while setting timesampled values on an attribute.

While at it, I also added support for authoring attributes of type `timecode`, which happens to be of cpp type `SdfTimeCode`. The underlying data is the same for both: a `double`. `UsdTimeCode` just encodes a couple sentinel values (`Default` and `EarliestTime`) as well.

Thanks for reading! If you find technical errors, please report in the blog's [Issues page][report].

[report]: https://github.com/kenanb/kenanb-blog/issues
