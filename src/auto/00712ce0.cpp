#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static int __fastcall C_0085ec18_SetConsoleControlState(C_0085ec18* self, void* /*edx*/, int a2)
{
    int result = 0;

    uint32_t previousControl = CSF_AT(uint32_t, self, 0x18);
    if (previousControl) {
        void* previous = reinterpret_cast<void*>(static_cast<uintptr_t>(previousControl));
        result = ((int(__fastcall*)(void*, void*, int))(CSF_AT(void**, previous, 0)[17]))(previous, nullptr, 0);
    }

    CSF_AT(uint32_t, self, 0x18) = static_cast<uint32_t>(a2);

    if (a2) {
        void* next = reinterpret_cast<void*>(static_cast<uintptr_t>(a2));
        return ((int(__fastcall*)(void*, void*, int))(CSF_AT(void**, next, 0)[17]))(next, nullptr, 1);
    }
    return result;
}

HOOK(0x712CE0, C_0085ec18_SetConsoleControlState, "auto");
