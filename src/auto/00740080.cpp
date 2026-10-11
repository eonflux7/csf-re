#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// 0x0064B250: __fastcall (int result), tested for nonzero by the caller.
CSF_FUNC(int, __fastcall, Sub_64B250, 0x0064B250, (void* self, void* edx));
// 0x00742180: __fastcall (char result), the refresh routine.
CSF_FUNC(char, __fastcall, sub_742180, 0x00742180, (void* self, void* edx));

// Original: char __thiscall sub_740080(CMenuMpStats* this)
// Calls the readiness check; if it passes, runs the refresh routine.
// Always sets the byte at this+0xA0 to 1 and returns the first call's result.
static char __fastcall CMenuMpStats_RefreshIfReadyAndSetFlag(void* self, void* /*edx*/) {
    char result = static_cast<char>(Sub_64B250(self, nullptr));
    if (result) {
        result = sub_742180(self, nullptr);
    }
    CSF_AT(uint8_t, self, 0xA0) = 1;
    return result;
}

HOOK(0x740080, CMenuMpStats_RefreshIfReadyAndSetFlag, "auto");
