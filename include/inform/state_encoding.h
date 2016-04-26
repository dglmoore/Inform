// Copyright 2016 ELIFE. All rights reserved.
// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.
#pragma once

#include <stdint.h>
#include <stdlib.h>

#define INFORM_ENCODING_ERROR(n) (inform_encoding_error + n)
static uint64_t const inform_encoding_error = 0x8000000000000000;

uint64_t inform_encode(int const *state, size_t n, int base);
int* inform_decode(uint64_t encoding, size_t n, int base);
