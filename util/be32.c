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

#include <stdint.h>

void
be32enc (void *pp, uint32_t u)
{
  unsigned char *p = (unsigned char *)pp;

  p[0] = (u >> 24) & 0xff;
  p[1] = (u >> 16) & 0xff;
  p[2] = (u >>  8) & 0xff;
  p[3] = (u & 0xff);
}

uint32_t
be32dec (const void *pp)
{
  unsigned char const *p = (unsigned char const *)pp;

  /*
   * Store into unsigned temporaries before left shifting,
   * to avoid promotion to signed int and then left shifting
   * into the sign bit, which is undefined behaviour.
   */

  unsigned int p3 = p[3];
  unsigned int p2 = p[2];
  unsigned int p1 = p[1];
  unsigned int p0 = p[0];

  return (p0 << 24) | \
         (p1 << 16) | \
         (p2 << 8)  | \
         (p3);
}
