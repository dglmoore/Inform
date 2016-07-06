// Copyright 2016 ELIFE. All rights reserved.
// Use of this source code is governed by a MIT
// license that can be found in the LICENSE file.
#include <unit.h>

IMPORT_SUITE(ActiveInformation);
IMPORT_SUITE(Canary);
IMPORT_SUITE(Distribution);
IMPORT_SUITE(Entropy);
IMPORT_SUITE(EntropyRate);
IMPORT_SUITE(Error);
IMPORT_SUITE(StateEncoding);
IMPORT_SUITE(TransferEntropy);

BEGIN_REGISTRATION
    REGISTER(ActiveInformation)
    REGISTER(Canary)
    REGISTER(Distribution)
    REGISTER(Entropy)
    REGISTER(EntropyRate)
    REGISTER(Error)
    REGISTER(StateEncoding)
    REGISTER(TransferEntropy)
END_REGISTRATION

UNIT_MAIN();
