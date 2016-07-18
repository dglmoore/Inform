// Copyright 2016 ELIFE. All rights reserved.
// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.
#include <inform/error.h>

bool inform_succeeded(inform_error const *err)
{
    return err == NULL || *err == INFORM_SUCCESS;
}

bool inform_failed(inform_error const *err)
{
    return !inform_succeeded(err);
}