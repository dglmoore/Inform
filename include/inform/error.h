// Copyright 2016 ELIFE. All rights reserved.
// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.
#pragma once

#include <inttypes.h>
#include <stdbool.h>

/**
 * Determine whether or not a potential NaN tag is valid.
 *
 * NaN tags are bounded above by `0x0007ffffffffffff`. This function
 * returns true if the potential tag exceeds that value.
 *
 * @param[in] tag the desired tag
 * @return a true if the tag is below `0x0007ffffffffffff`
 */
bool inform_tag_is_valid(uint64_t tag);

/**
 * Construct a tagged NaN with a given `tag`
 *
 * At 64-bits, any value of the form `0x7ff8XXXXXXXXXXXX` is identified
 * as a NaN value. We can thus "tag" the NaN by embedding numeric value
 * in the unspecified region.
 *
 * The tag is expected to be between `0` and `0x0007ffffffffffff` inclusive.
 * If the tag is out of range, -1 is returned.
 *
 * @param[in] tag the tag to embed
 * @return the tagged NaN will be returned.
 * 
 * @see inform_nan_tag
 */
double inform_nan(uint64_t tag);

/**
 * Extract the tag from a NaN value
 *
 * This function is the ivnerse of inform_tagnan. If the provided argument
 * is non NaN, then 0x0008000000000000` is returned.
 *
 * @param[in] the NaN value
 * @return the NaN tag
 * 
 * @see inform_nan
 */
uint64_t inform_nan_tag(double x);
