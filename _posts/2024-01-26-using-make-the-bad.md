---
layout: post
title:  "Using Make: The Bad"
date:   2024-01-26 21:00:00 +1300
categories: code makefile
published: true
---

Notes on Make build automation tool.

[ *Check out all posts in "makefile" series [here][series-makefile].* ]

[series-makefile]: {% link series.md %}#makefile

I mentioned the things I like about *make* [in my previous post][int-makefile-3]. I pointed out that it fills a certain role nicely, in the larger ecosystem. Now, the bad.

I don't intent to do a comprehensive analysis. People already prefer modern meta-build solutions over make due to reasons fairly obvious. I pointed out that it doesn't scale well. I will just elaborate on why that is.

Make is flexible, and it is full of useful features. There are many ways you can structure your build. And most of them work like a charm for a small project with flat structure.

But once the size and structure of your project increases, many of those possible approaches start leading to behaviour glitches. So makefiles gain intuition-breaking properties as they grow. Especially if you fight the system to avoid repetition, or try to increase the modularity of the setup.

There are *tricks* to achieve most things you need. However, there is no proper encapsulation. So the stuff you kitbash into your makefile will start to fall apart unless you really understand what they do.

If the makefile started pushing you to apply "creative" solutions you are not sure you understand well, it might be a good time to consider a higher-level build system instead.

I will conclude this topic mentioning the ugly parts in the next post.

Thanks for reading.

[int-makefile-3]: {% post_url 2024-01-25-using-make-the-good %}
