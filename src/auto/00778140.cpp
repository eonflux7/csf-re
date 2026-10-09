#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, ReleaseNativeHandle, 0x00775360, (int a1, int a2));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

static int __fastcall C_0082d410_ReleaseBuffersAndReset(C_0082d410* self, void* /*edx*/)
{
    // First block: release the native handle at +0x3C (with its companion at +0x40),
    // then free the buffer that was stored at +0x3C.
    if (CSF_AT(uint32_t, self, 0x3C) != 0) {
        ReleaseNativeHandle((int)CSF_AT(uint32_t, self, 0x3C),
                            (int)CSF_AT(uint32_t, self, 0x40));
        if (CSF_AT(uint32_t, self, 0x3C) != 0) {
            Mem_FreeDebug(reinterpret_cast<void*>(static_cast<uintptr_t>(CSF_AT(uint32_t, self, 0x3C))));
        }
    }
    CSF_AT(uint32_t, self, 0x3C) = 0;
    CSF_AT(uint32_t, self, 0x40) = 0;
    CSF_AT(uint32_t, self, 0x44) = 0;

    // Second block: the string buffer at +0x20 is freed only when its capacity (+0x34) is >= 16.
    // eax tracks the last value the original leaves in the return register.
    uint32_t eaxValue = CSF_AT(uint32_t, self, 0x34);
    if (eaxValue >= 0x10u) {
        eaxValue = CSF_AT(uint32_t, self, 0x20);
        if (eaxValue != 0) {
            eaxValue = static_cast<uint32_t>(
                Mem_FreeDebug(reinterpret_cast<void*>(static_cast<uintptr_t>(CSF_AT(uint32_t, self, 0x20)))));
        }
    }
    CSF_AT(uint32_t, self, 0x34) = 0xF;
    CSF_AT(uint32_t, self, 0x30) = 0;
    CSF_AT(uint8_t, self, 0x20) = 0;

    // Third block: the string buffer at +0x04 is freed only when its capacity (+0x18) is >= 16.
    if (CSF_AT(uint32_t, self, 0x18) >= 0x10u) {
        eaxValue = self->field_0004;
        if (eaxValue != 0) {
            eaxValue = static_cast<uint32_t>(
                Mem_FreeDebug(reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_0004))));
        }
    }
    CSF_AT(uint32_t, self, 0x14) = 0;
    CSF_AT(uint32_t, self, 0x18) = 0xF;
    CSF_AT(uint8_t, self, 0x04) = 0;

    return static_cast<int>(eaxValue);
}

HOOK(0x00778140, C_0082d410_ReleaseBuffersAndReset, "auto");
