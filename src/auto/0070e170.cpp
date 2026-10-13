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

CSF_FUNC(int, __cdecl, GetGlobalValue_70DDE0, 0x0070DDE0, ());
CSF_FUNC(int, __cdecl, CoreMemory_AllocUntagged, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));

// Configures the DirectSound effect buffer for this SOUNDMAX slot: computes the
// buffer size from a2, asks the global audio object for an effect (vtbl slot 37),
// applies the volume scale (vtbl slot 8), and reallocates the staging buffer.
static bool __fastcall S_0070e170_ConfigureBuffer(S_0070e170* self, void* /*edx*/, int a2, int a3)
{
    self->field_001c = static_cast<uint32_t>(a2) << 14;

    uint32_t effectParams[4];
    effectParams[1] = static_cast<uint32_t>(a2);
    effectParams[0] = static_cast<uint32_t>(a2) << 15;
    effectParams[2] = static_cast<uint32_t>(a3);
    effectParams[3] = 16;

    void* globalValue = reinterpret_cast<void*>(static_cast<uintptr_t>(GetGlobalValue_70DDE0()));
    uintptr_t effectAddr = reinterpret_cast<uintptr_t>(
        ((uint32_t(__fastcall*)(void*, void*, void*))CSF_AT(void**, globalValue, 0)[37])(
            globalValue, nullptr, effectParams));
    self->field_0018 = static_cast<uint32_t>(effectAddr);
    if (!effectAddr)
        return false;

    float volumeScale = static_cast<float>(self->field_0008) * static_cast<float>(self->field_0004);
    void* effectObj = reinterpret_cast<void*>(effectAddr);
    ((void(__fastcall*)(void*, void*, float))CSF_AT(void**, effectObj, 0)[8])(
        effectObj, nullptr, volumeScale);

    uint32_t oldBuffer = self->field_0014;
    self->field_0044 = 0;
    if (oldBuffer) {
        CoreMemory_Free(reinterpret_cast<void*>(static_cast<uintptr_t>(oldBuffer)));
        self->field_0014 = 0;
    }

    self->field_0014 = static_cast<uint32_t>(CoreMemory_AllocUntagged(self->field_001c));
    return true;
}

HOOK(0x0070E170, S_0070e170_ConfigureBuffer, "auto");
