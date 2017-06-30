// Copyright 2016-2017 ELIFE. All rights reserved.
// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.
#include "util.h"
#include <inform/effective_info.h>
#include <math.h>
#include <unit.h>

UNIT(EffectiveInfoNullTPM)
{
    inform_error err = INFORM_SUCCESS;
    ASSERT_TRUE(isnan(inform_effective_info(NULL, NULL, 0, &err)));
    ASSERT_TRUE(inform_failed(&err));
    ASSERT_EQUAL(INFORM_ETPM, err);
}

UNIT(EffectiveInfoZeroSize)
{
    inform_error err = INFORM_SUCCESS;
    ASSERT_TRUE(isnan(inform_effective_info(NULL, NULL, 0, &err)));
    ASSERT_TRUE(inform_failed(&err));
    ASSERT_EQUAL(INFORM_ESIZE, err);
}

UNIT(EffectiveInfoZeroRowTPM)
{
    double tpm[4] = {0.0, 1.0, 0.0, 0.0};
    inform_error err = INFORM_SUCCESS;
    ASSERT_TRUE(isnan(inform_effective_info(tpm, NULL, 2, &err)));
    ASSERT_TRUE(inform_failed(&err));
    ASSERT_EQUAL(INFORM_ETPM, err);
}

UNIT(EffectiveInfoNegativeProbabilitiesTPM)
{
    double tpm[4] = {-0.5, 1.0, 0.5, 0.5};
    inform_error err = INFORM_SUCCESS;
    ASSERT_TRUE(isnan(inform_effective_info(tpm, NULL, 2, &err)));
    ASSERT_TRUE(inform_failed(&err));
    ASSERT_EQUAL(INFORM_ETPM, err);
}

UNIT(EffectiveInfoUnNormalizedRowTPM)
{
    inform_error err = INFORM_SUCCESS;
    {
        double tpm[4] = {0.5, 0.5, 0.5, 0.25};
        ASSERT_TRUE(isnan(inform_effective_info(tpm, NULL, 2, &err)));
        ASSERT_TRUE(inform_failed(&err));
        ASSERT_EQUAL(INFORM_ETPM, err);
    }
    {
        double tpm[4] = {0.5, 0.5, 0.5, 0.75};
        ASSERT_TRUE(isnan(inform_effective_info(tpm, NULL, 2, &err)));
        ASSERT_TRUE(inform_failed(&err));
        ASSERT_EQUAL(INFORM_ETPM, err);
    }
}

UNIT(EffectiveInfoZeroIntervention)
{
    double tpm[4] = {0.25, 0.75, 0.3, 0.7};
    double inter[2] = {0.0, 0.0};
    inform_error err = INFORM_SUCCESS;
    ASSERT_TRUE(isnan(inform_effective_info(tpm, inter, 2, &err)));
    ASSERT_TRUE(inform_failed(&err));
    ASSERT_EQUAL(INFORM_EDIST, err);
}

UNIT(EffectiveInfoNegativeProbabilitiesIntervention)
{
    double tpm[4] = {0.25, 0.75, 0.3, 0.7};
    double inter[2] = {0.0, -0.2};
    inform_error err = INFORM_SUCCESS;
    ASSERT_TRUE(isnan(inform_effective_info(tpm, inter, 2, &err)));
    ASSERT_TRUE(inform_failed(&err));
    ASSERT_EQUAL(INFORM_EDIST, err);
}

UNIT(EffectiveInfoUnNormalizedIntervention)
{
    inform_error err = INFORM_SUCCESS;
    double tpm[4] = {0.25, 0.75, 0.3, 0.7};
    {
        double inter[2] = {0.5, 0.25};
        ASSERT_TRUE(isnan(inform_effective_info(tpm, inter, 2, &err)));
        ASSERT_TRUE(inform_failed(&err));
        ASSERT_EQUAL(INFORM_EDIST, err);
    }
    {
        double inter[2] = {0.5, 0.75};
        ASSERT_TRUE(isnan(inform_effective_info(tpm, inter, 2, &err)));
        ASSERT_TRUE(inform_failed(&err));
        ASSERT_EQUAL(INFORM_EDIST, err);
    }
}

BEGIN_SUITE(EffectiveInformation)
    ADD_UNIT(EffectiveInfoNullTPM)
    ADD_UNIT(EffectiveInfoZeroSize)
    ADD_UNIT(EffectiveInfoZeroRowTPM)
    ADD_UNIT(EffectiveInfoNegativeProbabilitiesTPM)
    ADD_UNIT(EffectiveInfoUnNormalizedRowTPM)
    ADD_UNIT(EffectiveInfoZeroIntervention)
    ADD_UNIT(EffectiveInfoNegativeProbabilitiesIntervention)
    ADD_UNIT(EffectiveInfoUnNormalizedIntervention)
END_SUITE
