/* SPDX-License-Identifier: 0BSD */

/* Copyright (C) 2026 Mintsuki and contributors.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
 * OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */


/* Not a header of the library: what GCC 16 needs to find on the include path
 * of a translation unit it compiles with -fmodules, which it only looks for.
 *
 * Workaround: GCC 16, all targets, with -fmodules: it takes the #include of a
 * standard header, from a file outside of the directory of that header, to
 * name <bits/stdc++.h> where that has been built as a header unit, and stops
 * with an error where it finds no such header, as with -nostdinc. Where it
 * finds it and it has not been built, it includes the standard header as it
 * is. Pass -isystem with the directory above this one to GCC 16 along with
 * -fmodules (see README.md). */

#error "<bits/stdc++.h> is not a header of the freestanding C++ headers"
