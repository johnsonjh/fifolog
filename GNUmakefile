# SPDX-License-Identifier: BSD-2-Clause
#
# Copyright (c) 2005-2008 Poul-Henning Kamp
# Copyright (c) 2023 Jeffrey H. Johnson <trnsz@pobox.com>
#
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
# 1. Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
#
# 2. Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
#
# THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
# OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
# HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
# LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
# OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
# SUCH DAMAGE.

SHELL   := $(shell env PATH="$$(command -p getconf PATH)" command -v sh)
LIBSRC  := $(strip $(wildcard lib/*.c) $(wildcard util/*.c))
LTOFLAG ?= -flto=auto
CFLAGS  += -Ilib -Iutil -O3 $(LTOFLAG)
LDFLAGS += $(LTOFLAG)

ifeq ($(USE_MINIZ),1)
  CFLAGS += -Iminiz -DUSE_MINIZ=1
  LIBSRC += miniz/miniz.c
else
  LDFLAGS += -lz
endif

.PHONY: all
all:
	@$(MAKE) out/bin/fifolog_create out/bin/fifolog_reader out/bin/fifolog_writer

out/bin/fifolog_create: out/src/getdate.c fifolog_create/fifolog_create.c $(LIBSRC)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -o $@ out/src/getdate.c fifolog_create/fifolog_create.c $(LIBSRC) $(LDFLAGS)
	@ls -l $@ || true
	@printf '%s\n' "*** $@ built successfully." || true

out/bin/fifolog_reader: out/src/getdate.c fifolog_reader/fifolog_reader.c $(LIBSRC)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -o $@ out/src/getdate.c fifolog_reader/fifolog_reader.c $(LIBSRC) $(LDFLAGS)
	@ls -l $@ || true
	@printf '%s\n' "*** $@ built successfully." || true

out/bin/fifolog_writer: out/src/getdate.c fifolog_writer/fifolog_writer.c $(LIBSRC)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -o $@ out/src/getdate.c fifolog_writer/fifolog_writer.c $(LIBSRC) $(LDFLAGS)
	@ls -l $@ || true
	@printf '%s\n' "*** $@ built successfully." || true

out/src/getdate.c: lib/getdate.y
	@mkdir -p $(@D)
	@{ command -v byacc > /dev/null 2>&1 && "$$(command -v byacc)" $< -o $@; } || \
	 { command -v bison > /dev/null 2>&1 && "$$(command -v bison)" -Wnone $< -o $@; } || \
	 { command -v yacc  > /dev/null 2>&1 && "$$(command -v yacc)" $< -o $@; } || \
	 { printf '%s\n' "*** Error: yacc $< failed."; exit 1; }

.PHONY: test
test:
	@test -f out/bin/fifolog_create || \
	 { printf '%s\n' "*** Error: Missing fifolog_create."; exit 1; }
	@test -f out/bin/fifolog_reader || \
	 { printf '%s\n' "*** Error: Missing fifolog_reader."; exit 1; }
	@test -f out/bin/fifolog_writer || \
	 { printf '%s\n' "*** Error: Missing fifolog_writer."; exit 1; }
	@rm -rf ./out/test
	@mkdir -p out/test
	@printf '%s\n' "" || true
	out/bin/fifolog_create -s 10M out/test/test.log && test -f out/test/test.log
	ls -l out/test/test.log
	@printf '%s\n' "" || true
	printf '%s\n' "Verification 1" | out/bin/fifolog_writer out/test/test.log
	out/bin/fifolog_reader -t out/test/test.log 2>&1 | tee /dev/stderr | grep -q " Verification 1$$"
	@printf '%s\n' "" || true
	printf '%s\n' "Verification 2" | out/bin/fifolog_writer out/test/test.log
	out/bin/fifolog_reader -t out/test/test.log 2>&1 | tee /dev/stderr | tail -1 | grep -q " Verification 2$$"
	@printf '%s\n' "" || true
	out/bin/fifolog_reader -t out/test/test.log 2>&1 | tee /dev/stderr | tail -2 | head -1 | grep -q " Verification 1$$"
	@printf '%s\n' "" || true
	@printf '%s\n' "*** Tests completed successfully." || true

.PHONY: clean
clean:
	rm -rf ./out > /dev/null 2>&1 || true
