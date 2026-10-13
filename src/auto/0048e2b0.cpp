#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __cdecl, Vehicle_CreateObjectByType, 0x0041DDC0, (int type));

// Stores the vehicle object created for `vehicleType`, and if the owner's
// setup flag (+0x174) is set and creation succeeded, forwards the setup event
// to the new object through its vtable slot 8 (byte offset 0x20).
static int __fastcall C_00833908_SendSetupEvent(C_00833908* self, void* /*edx*/, int vehicleType, int setupArg)
{
    uint32_t* created = Vehicle_CreateObjectByType(vehicleType);
    int result = static_cast<int>(reinterpret_cast<uintptr_t>(created));

    CSF_AT(uint32_t, self, 0x4C) = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(created));

    bool shouldSend = (CSF_AT(uint8_t, self, 0x174) != 0) && (created != nullptr);
    CSF_AT(uint8_t, self, 0x174) = shouldSend ? 1 : 0;

    if (shouldSend) {
        result = ((int(__fastcall*)(void*, void*, void*, int))
                  (CSF_AT(void**, created, 0)[32 / 4]))(created, nullptr, self, setupArg);
        CSF_AT(uint8_t, self, 0x174) = static_cast<uint8_t>(result);
    }

    return result;
}

HOOK(0x0048E2B0, C_00833908_SendSetupEvent, "auto");
