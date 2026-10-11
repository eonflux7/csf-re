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

// Maps the gathering state stored at +0x0C of the argument object to a result code.
// Original: int __stdcall sub_6A7A00(int a1), ret 4.
static int __stdcall NetGathering_MapStateToResultCode(int a1) {
    switch (CSF_AT(int, reinterpret_cast<uint8_t*>(a1), 0x0C)) {
        case 1:
            return 3;
        case 2:
            return 1;
        case 3:
            return 2;
    }
    return 0;
}

HOOK(0x6A7A00, NetGathering_MapStateToResultCode, "auto");
