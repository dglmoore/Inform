// Copyright 2016 ELIFE. All rights reserved.
// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.
#include <unit.h>

IMPORT_SUITE(ActiveInformation);
IMPORT_SUITE(BlockEntropy);
IMPORT_SUITE(Canary);
IMPORT_SUITE(ConditionalEntropy);
IMPORT_SUITE(Distribution);
IMPORT_SUITE(Entropy);
IMPORT_SUITE(EntropyRate);
IMPORT_SUITE(MutualInfo);
IMPORT_SUITE(RelativeEntropy);
IMPORT_SUITE(TransferEntropy);
IMPORT_SUITE(Utilities);

BEGIN_REGISTRATION
    REGISTER(ActiveInformation)
    REGISTER(BlockEntropy)
    REGISTER(Canary)
    REGISTER(ConditionalEntropy)
    REGISTER(Distribution)
    REGISTER(Entropy)
    REGISTER(EntropyRate)
    REGISTER(MutualInfo)
    REGISTER(RelativeEntropy)
    REGISTER(TransferEntropy)
    REGISTER(Utilities)
END_REGISTRATION

UNIT_MAIN();
