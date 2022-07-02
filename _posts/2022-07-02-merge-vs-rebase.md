---
layout: post
title:  "Merge vs Rebase"
date:   2022-07-02 21:15:00 +1200
categories: code git
published: true
---

Most arguments are covered in [this tutorial][at-merge-rebase] and [this Stack Overflow post][so-merge-rebase] already. One I don't see emphasized:

Rebased history is possibly a lie. After rebase, commit messages can be incorrect.

Commits contain statements based on state of code at the time of original commit.

When developers rebase, they don't ensure every statement is still valid for each rebased commit.

Merge doesn't lie. When I look at a merge commit, I know developer merged the branch, resolved, tested once at most, and moved on.

So if you are developing with the intention of rebasing, make sure your commits don't contain statements that might not survive the rebase.

[at-merge-rebase]: https://www.atlassian.com/git/tutorials/merging-vs-rebasing
[so-merge-rebase]: https://stackoverflow.com/questions/804115/when-do-you-use-git-rebase-instead-of-git-merge
