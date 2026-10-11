#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, IsField1BCNonNegative, 0x00492B10, (void* self, void*));
CSF_FUNC(char, __fastcall, sub_48E5D0, 0x0048E5D0, (void* self, void* edx, int a2, int a3, int a4));
CSF_FUNC(int, __fastcall, sub_4DB440, 0x004DB440, (void* self, void* edx, int a2));

// Original: char __thiscall sub_420BB0(C_00834fa8*, int a2); ret 4.
static char __fastcall C_00834fa8_TryAdvanceWhenOwnerReady(C_00834fa8* self, void* /*edx*/, int /*a2*/)
{
    // The owner object pointer lives at +0x08 of this task; re-read it at each use.
    char result = static_cast<char>(
        IsField1BCNonNegative(reinterpret_cast<void*>(CSF_AT(uint32_t, CSF_AT(uint32_t, self, 0x08), 0x14)), nullptr));

    if (!result) {
        result = sub_48E5D0(
            reinterpret_cast<void*>(CSF_AT(uint32_t, CSF_AT(uint32_t, self, 0x08), 0x14)),
            nullptr,
            static_cast<int>(CSF_AT(uint32_t, self, 0x08)),
            2,
            1);
        if (result) {
            int stateArg = static_cast<int>(CSF_AT(uint32_t, self, 0x0C) + 0x34);
            return static_cast<char>(sub_4DB440(self, nullptr, stateArg));
        }
    }
    return result;
}

HOOK(0x420BB0, C_00834fa8_TryAdvanceWhenOwnerReady, "auto");
