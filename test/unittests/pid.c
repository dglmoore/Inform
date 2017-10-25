// Copyright 2016-2017 ELIFE. All rights reserved.
// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.
#include <inform/pid.h>
#include <ginger/unit.h>

UNIT(PIDCanary)
{
    ASSERT_TRUE(0);
}

BEGIN_SUITE(PID)
    ADD_UNIT(PIDCanary)
END_SUITE
