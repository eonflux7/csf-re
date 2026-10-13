#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(bool, __fastcall, NetSession_IsDuplicate, 0x0064B5C0, (void* self, void* edx));
CSF_FUNC(char, __fastcall, sub_74B6E0, 0x0074B6E0, (void* self, void* edx, char a2));

// Stores 1.0f at +0x9C, then either forwards the value to sub_74B6E0 (when
// NetSession_IsDuplicate reports true) or, while the slot at +0x5C is still -1,
// latches a2 into it. Returns the forwarded result, a2 for the latch path, or 0.
static char __fastcall C_00858180_SetRequestedValue(C_00858180* self, void* /*edx*/, int a2)
{
    CSF_AT(float, self, 0x9C) = 1.0f;

    if (NetSession_IsDuplicate(self, nullptr))
        return sub_74B6E0(self, nullptr, static_cast<char>(a2));

    char result = 0;
    if (CSF_AT(int, self, 0x5C) == -1) {
        result = static_cast<char>(a2);
        if (a2 != CSF_AT(int, self, 0x5C))
            CSF_AT(int, self, 0x5C) = a2;
    }
    return result;
}

HOOK(0x73BD20, C_00858180_SetRequestedValue, "auto");
