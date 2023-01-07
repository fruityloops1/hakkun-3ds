#pragma once

#include "hk/Types.h"
#include <stddef.h>

namespace hk::sys {

// Information used by the ExheaderCli tool to apply hooks/patches
struct PatchHeader {
    uintptr_t codeSize;
    void (*initFunc)(); // Hook function that will run in crt0
};

volatile const extern PatchHeader sPatchHeader;

} // namespace hk::sys
