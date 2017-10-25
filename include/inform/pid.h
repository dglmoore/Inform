// Copyright 2016-2017 ELIFE. All rights reserved.
// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.
#pragma once

#include <inform/error.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct pid_source
{
    size_t *name;
    struct pid_source **above;
    struct pid_source **below;
    size_t size, n_above, n_below;
} pid_source;

void pid_source_free(pid_source *src);

pid_source **pid_sources(size_t n);

#ifdef __cplusplus
}
#endif
