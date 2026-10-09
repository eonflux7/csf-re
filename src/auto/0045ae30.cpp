#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Computes a combined flag from two linked holders and writes it to the target's core flag byte.
// Each holder points (at +4) to a core object whose byte at +1 is a boolean flag.
// The target's core flag byte is set to 1 only when both pair entries have their flag set.
static int __stdcall UpdateCombinedFlag(void* target, int /*unused*/, void** pair) {
    void* firstHolder = pair[0];
    void* firstCore = CSF_AT(void*, firstHolder, 4);
    if (CSF_AT(uint8_t, firstCore, 1) != 0) {
        void* secondHolder = pair[1];
        void* secondCore = CSF_AT(void*, secondHolder, 4);
        if (CSF_AT(uint8_t, secondCore, 1) != 0) {
            void* targetCore = CSF_AT(void*, target, 4);
            CSF_AT(uint8_t, targetCore, 1) = 1;
            return 1;
        }
    }
    void* targetCore = CSF_AT(void*, target, 4);
    CSF_AT(uint8_t, targetCore, 1) = 0;
    return 0;
}

HOOK(0x45AE30, UpdateCombinedFlag, "auto");
