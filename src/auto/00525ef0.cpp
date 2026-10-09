#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"

// Callee bindings (one name and signature per address).
CSF_FUNC(int, __cdecl, GetGlobalValue_521BB0, 0x00521BB0, ());
CSF_FUNC(int, __fastcall, ApplyGlobalValueRequest, 0x00523940, (void* self, void* edx, void* a2, float a3));

// Builds a default global-value request on the stack and submits it to the
// global value manager, returning whatever ApplyGlobalValueRequest returns.
static int __stdcall SubmitDefaultGlobalValueRequest(int a1)
{
    // Request layout (0x38 bytes, stack object in the original):
    //   0x00: 4            0x04: 4            0x08: a1           0x0C: -1
    //   0x10: 0            0x14: 0            0x18: 0            0x1C: 0
    //   0x20: 0            0x24: vtable       0x28: -1           0x2C: -1
    //   0x30: 0            0x34: 0 (byte)     0x35: 1 (byte)     0x36: 0 (byte)
    alignas(4) uint8_t request[0x38] = {};

    CSF_AT(int, request, 0x00) = 4;
    CSF_AT(int, request, 0x04) = 4;
    CSF_AT(int, request, 0x08) = a1;
    CSF_AT(int, request, 0x0C) = -1;
    CSF_AT(int, request, 0x10) = 0;
    CSF_AT(int, request, 0x14) = 0;
    CSF_AT(int, request, 0x18) = 0;
    CSF_AT(int, request, 0x1C) = 0;
    CSF_AT(int, request, 0x20) = 0;
    CSF_AT(void*, request, 0x24) = CSF_VTBL(0x0082D448);
    CSF_AT(int, request, 0x28) = -1;
    CSF_AT(int, request, 0x2C) = -1;
    CSF_AT(int, request, 0x30) = 0;
    CSF_AT(uint8_t, request, 0x34) = 0;
    CSF_AT(uint8_t, request, 0x35) = 1;
    CSF_AT(uint8_t, request, 0x36) = 0;

    void* globalValueManager = reinterpret_cast<void*>(GetGlobalValue_521BB0());
    return ApplyGlobalValueRequest(globalValueManager, nullptr, request, -1.0f);
}

HOOK(0x00525EF0, SubmitDefaultGlobalValueRequest, "auto");
