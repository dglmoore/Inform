// Copyright 2016-2017 ELIFE. All rights reserved.
// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.
#pragma once

#include <inform/error.h>

#ifdef __cplusplus
extern "C"
{
#endif

EXPORT double inform_effective_info(double const *tpm, double const *inter,
    size_t n, inform_error *err);

#ifdef __cplusplus
}
#endif
