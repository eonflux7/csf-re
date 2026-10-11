#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Virtual slot 0xAC / 4 = 43 on C_0083e7d8 (inherited from C_0083e160). No typed wrapper is declared for it.
static bool __fastcall C_0083e7d8_IsSlotAcResultNonPositive(C_0083e7d8* self, void* /*edx*/) {
    int result = ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, self, 0)[0xAC / 4])(self, nullptr, 20);
    return result <= 0;
}

HOOK(0x004E44B0, C_0083e7d8_IsSlotAcResultNonPositive, "auto");
