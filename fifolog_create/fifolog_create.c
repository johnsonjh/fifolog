/*
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (c) 2005-2008 Poul-Henning Kamp
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <sysexits.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#if defined(__FreeBSD__)
# include <libutil.h>
#endif

#include "libfifolog.h"

#define DEF_RECSIZE     512
#define DEF_RECCNT      (24 * 60 * 60)

static void
usage(void)
{
        fprintf(stderr, "Usage: fifolog_create [-l record-size] "
            "[-r record-count] [-s size] file\n");
        exit(EX_USAGE);
}

int expand_number(const char *buf, uint64_t *num);

int
main(int argc, char * const *argv)
{
        int ch;
        uint64_t size;
        uint64_t recsize;
        uint64_t reccnt;
        const char *s;

        recsize = 0;
        size = 0;
        reccnt = 0;
        while((ch = getopt(argc, argv, "l:r:s:")) != -1) {
                switch (ch) {
                case 'l':
                        if (expand_number(optarg, &recsize)) {
                                fprintf(stderr, "Error: Couldn't parse -l argument\n");
                                exit(1);
                        }
                        break;
                case 'r':
                        if (expand_number(optarg, &reccnt)) {
                                fprintf(stderr, "Error: Couldn't parse -r argument\n");
                                exit(1);
                        }
                        break;
                case 's':
                        if (expand_number(optarg, &size)) {
                                fprintf(stderr, "Error: Couldn't parse -s argument\n");
                                exit(1);
                        }
                        break;
                default:
                        usage();
                }
        }
        argc -= optind;
        argv += optind;
        if (argc != 1)
                usage();

        if (size != 0 && reccnt != 0 && recsize != 0) {         /* N N N */
                if (size !=  reccnt * recsize) {
                        fprintf(stderr, "Error: Inconsistent -l, -r and -s values\n");
                        exit(1);
                }
        } else if (size != 0 && reccnt != 0 && recsize == 0) {  /* N N Z */
                if (size % reccnt) {
                        fprintf(stderr,
                            "Error: Inconsistent -r and -s values (gives remainder)\n");
                        exit(1);
                }
                recsize = size / reccnt;
        } else if (size != 0 && reccnt == 0 && recsize != 0) {  /* N Z N */
                if (size % recsize) {
                    fprintf(stderr, "Error: -s arg not divisible by -l arg\n");
                    exit(1);
                }
        } else if (size != 0 && reccnt == 0 && recsize == 0) {  /* N Z Z */
                recsize = DEF_RECSIZE;
                if (size % recsize) {
                    fprintf(stderr, "Error: -s arg not divisible by %llu\n", (long long)recsize);
                    exit(1);
                }
        } else if (size == 0 && reccnt != 0 && recsize != 0) {  /* Z N N */
                size = reccnt * recsize;
        } else if (size == 0 && reccnt != 0 && recsize == 0) {  /* Z N Z */
                recsize = DEF_RECSIZE;
                size = reccnt * recsize;
        } else if (size == 0 && reccnt == 0 && recsize != 0) {  /* Z Z N */
                size = DEF_RECCNT * recsize;
        } else if (size == 0 && reccnt == 0 && recsize == 0) {  /* Z Z Z */
                recsize = DEF_RECSIZE;
                size = DEF_RECCNT * recsize;
        }

        s = fifolog_create(argv[0], size, recsize);
        if (s == NULL)
                return (0);
        fprintf(stderr, "Error: %s: %s\n", s, strerror(errno));
        exit(1);
}
