#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(void*, __fastcall, MenuControls_ResolveObject, 0x006B1390, (void* self, void* edx));

// Copies six dwords from the source record into self+0x14..0x28, stores 1.0f at
// self+0x2C, then asks the resolved global object (vtable slot 25) whether the
// source's first dword is handled. The result is kept in self+0x10 and returned as a bool.
static bool __fastcall ParticleRender_CaptureStateAndQuery(void* self, void* /*edx*/, uint32_t* sourceValues)
{
    CSF_AT(uint32_t, self, 0x14) = sourceValues[1];
    CSF_AT(uint32_t, self, 0x18) = sourceValues[2];
    CSF_AT(uint32_t, self, 0x1C) = sourceValues[3];
    CSF_AT(uint32_t, self, 0x20) = sourceValues[4];
    CSF_AT(uint32_t, self, 0x24) = sourceValues[5];
    CSF_AT(uint32_t, self, 0x28) = sourceValues[6];
    CSF_AT(uint32_t, self, 0x2C) = 0x3F800000u; // 1.0f

    int globalInt = GetGlobalInt();
    void* resolved = MenuControls_ResolveObject(
        reinterpret_cast<void*>(static_cast<intptr_t>(globalInt)), nullptr);

    int queryResult = ((int(__fastcall*)(void*, void*, uint32_t))CSF_AT(void**, resolved, 0)[25])(
        resolved, nullptr, sourceValues[0]);

    CSF_AT(int, self, 0x10) = queryResult;
    return queryResult != 0;
}

HOOK(0x007051B0, ParticleRender_CaptureStateAndQuery, "auto");
