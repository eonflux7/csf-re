#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(uint8_t*, __fastcall, MpGameModes_CopySpawnPointSettings, 0x00550B50, (uint8_t* self, void* , const uint8_t* source));

CSF_GLOBAL(uint8_t, g_MpSpawnPointSettingsBlock, 0x008B06A0);

// Calls the object's slot 1 virtual (+0x4) with no arguments, using the raw
// 32-bit eax result. When that result is 2, the function copies the spawn
// point settings block at this+0x64 into the global settings object.
static uint8_t* __fastcall C_00868010_ResolveSpawnPointSettings(C_00868010* self, void* /*edx*/, int a2)
{
    void* object = reinterpret_cast<void*>(static_cast<uintptr_t>(static_cast<uint32_t>(a2)));
    auto getStateCode = reinterpret_cast<int(__fastcall*)(void*, void*)>(CSF_AT(void**, object, 0)[1]);
    uint32_t stateCode = static_cast<uint32_t>(getStateCode(object, nullptr));

    uint32_t offsetValue = stateCode - 2u;
    uint8_t* result = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(offsetValue));
    if (offsetValue == 0)
    {
        uint8_t* settingsSource = reinterpret_cast<uint8_t*>(self) + 0x64;
        return MpGameModes_CopySpawnPointSettings(&g_MpSpawnPointSettingsBlock, nullptr, settingsSource);
    }
    return result;
}

HOOK(0x007484D0, C_00868010_ResolveSpawnPointSettings, "auto");
