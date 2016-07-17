// Copyright 2016 ELIFE. All rights reserved.
// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.
#pragma once

#include <inform/export.h>
#include <stdint.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C"
{
#endif

#define INFORM_ENCODING_ERROR(n) (inform_encoding_error + n)
static uint64_t const inform_encoding_error = 0x8000000000000000;

/**
 * Encode a sequence of unsigned integers into a single integer.
 *
 * Note that this because the encodings are 64-bits in length,
 * for a base @f b @f the maximum number of base-@f b @f digits
 * that can be encoded is @f \frac{64}{log_2 b} @f.
 *
 * However, due to limitations in C's error handling we've opted
 * to admit up to 63-bits of the integer for the encoding. In the
 * event that an error occurs the returned value is
 * `inform_encoding_error` or larger.
 *
 * Several possible errors and their associated error codes are:
 *   - if `state == NULL` return `INFORM_ENCODING_ERROR(0)`
 *   - if `base < 2` return `INFORM_ENCODING_ERROR(1)`
 *   - if `n >= 64 / log2(base)` return `INFORM_ENCODING_ERROR(2)`
 *   - if `state[i] >= base` return `INFORM_ENCODING_ERROR(3 + i)`
 *
 * @param[in] state the sequence of digits
 * @param[in] n     the number of digits
 * @param[in] base  the base or number of distict digits
 * @return the integer encoding
 *
 * @see inform_decode
 */
EXPORT uint64_t inform_encode(uint64_t const *state, size_t n, int base);
/**
 * Decode an integer into a sequence of values.
 *
 * Note that this function is (roughly) the inverse of inform_encode.
 *
 * In the event that a decoding error occurs, a `NULL` value is
 * returned. A few situtations in which this may occur are
 *   - `encoding >= inform_encoding_error`
 *   - `base < 2`
 *   - `n >= 64 / log2(base)`
 *   - allocation of the memory for the decoded array fails
 *
 * @param[in] encoding the integral value to decode
 * @param[in] n        the number of digits to decode to
 * @param[in] base     the base of the decoding
 * @return a dynamic array containing the decoded digits
 *
 * @see inform_encode
 */
EXPORT uint64_t* inform_decode(uint64_t encoding, size_t n, int base);

#ifdef __cplusplus
}
#endif

