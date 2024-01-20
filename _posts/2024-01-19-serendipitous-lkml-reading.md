---
layout: post
title:  "Serendipitous LKML Reading"
date:   2024-01-19 22:00:00 +1300
categories: code linux
published: true
---

I read about a bunch of interesting Linux related technologies.

Today, I was looking for something interesting to write about, without spending too much time on. Because I wanted to reserve my time for a dev project. Obviously, I failed, as it is pretty late here, and I just started writing the post.

So, what happened.

I have this general notes file, which is, you guessed it, an emacs [org-mode][org-mode] file.

In it, I have a "Learn" section. This is just a list of technologies/concepts that I found interesting or useful to explore in the future. Here are a few example entries:
- [eBPF (extended Berkeley Packet Filter)][wikipedia-ebpf]
- [Cassowary][wikipedia-cassowary] algorithm
- UMCG

UMCG? What is that? I had no description in the file. Quick googling didn't produce any results that sounded relevant to my domain either.

## UMCG

It took a while to find out that it stands for: [User-Managed Concurrency Groups][lwn-umcg].

According to [this LKML post][lkml-p0][^1] from 2021, UMCG is a **proposed** Linux Kernel feature, as part of the work to open-source the "Google Fibers" userspace scheduling framework.

I don't quite know how learning more about this would be useful to me, but it sounded like a cool concept.

A quick grep on the recent kernel release (6.7) sources didn't produce any results. My understanding is, this concept haven't been merged yet. It seems like there have been a few iterations on the implementation, and the last UMCG related discussion I found is from last year.

## CRIU

Then I ended up reading a bunch of other discussions on LKML.

In one of the posts, I encountered another four letter acronym that's more familiar: CRIU

This is the second time in 2 days I am encountering CRIU. CRIU is "[a project to implement checkpoint/restore functionality for Linux][criu-org-main]". It stands for "Checkpoint/Restore In Userspace".

I read about it yesterday, [while trying to figure out what other features LXC provides][int-containers-2]. LXC has an `lxc-checkpoint` command that (according to the [man page][wikipedia-man-page], since I couldn't test it) allows checkpointing and restoring a container. It uses `CRIU` internally.

CRIU looks like a pretty ambitious and interesting project. I am not sure how successful or stable it is, but it is already integrated in a bunch of software. The wiki pages I found to be insightful are:
- [Usage Scenarios][criu-org-usage-scenarios]
- [What cannot be checkpointed][criu-org-what-cannot-be-checkpointed]
- [What can change after C/R][criu-what-can-change]

I just started following the tutorials to have a better understanding of this technology, and the limitations.

And that's it for today. Thanks for reading! If you find technical errors, please report in the blog's [Issues page][report].

[^1]: LKML: [Linux kernel mailing list][wikipedia-lkml]

[report]: https://github.com/kenanb/kenanb-blog/issues

[int-containers-2]: {% post_url 2024-01-18-lxc-for-development %}

[org-mode]: https://orgmode.org/
[wikipedia-ebpf]: https://en.wikipedia.org/wiki/EBPF
[wikipedia-cassowary]: https://en.wikipedia.org/wiki/Cassowary_(software)
[lwn-umcg]: https://lwn.net/Articles/879398/
[wikipedia-lkml]: https://en.wikipedia.org/wiki/Linux_kernel_mailing_list
[lkml-p0]: https://lwn.net/ml/linux-kernel/20210520183614.1227046-1-posk@google.com/
[criu-org-main]: https://criu.org/Main_Page
[criu-org-usage-scenarios]: https://criu.org/Usage_scenarios
[criu-org-what-cannot-be-checkpointed]: https://criu.org/What_cannot_be_checkpointed
[criu-what-can-change]: https://criu.org/What_can_change_after_C/R
[wikipedia-man-page]: https://en.wikipedia.org/wiki/Man_page
