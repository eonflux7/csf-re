#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(uint8_t*, __fastcall, MpGameModes_CopySpawnPointSettings, 0x00550B50, (uint8_t* self, void* /*edx*/, const uint8_t* source));
CSF_FUNC(char, __fastcall, MarkActiveAndReturnTrue, 0x00752DA0, (void* self, void* /*edx*/));

static int __fastcall MpSpawnPoints_ApplySelection(void* self, void* /*edx*/, int a2)
{
    void* spawnPointSettings = reinterpret_cast<uint8_t*>(self) + 0x64;
    MpGameModes_CopySpawnPointSettings(reinterpret_cast<uint8_t*>(spawnPointSettings), nullptr,
                                       reinterpret_cast<const uint8_t*>(a2));
    return MarkActiveAndReturnTrue(spawnPointSettings, nullptr);
}

HOOK(0x00747F00, MpSpawnPoints_ApplySelection, "auto");
