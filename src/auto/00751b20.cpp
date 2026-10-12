#include <cstdint>
#include "globals.h"
#include "hook.h"

static CSF_FUNC(uint8_t*, __cdecl, SceneLoader_CreateLoadTimer, 0x0069CDC0, ());

static uint32_t* __fastcall NetSpawnMessages_InitializeTimedEntry(
    uint32_t* self, void* /*edx*/, int value)
{
    CSF_AT(uint32_t, self, 0x0) = static_cast<uint32_t>(value);
    CSF_AT(uint8_t*, self, 0x4) = SceneLoader_CreateLoadTimer();
    CSF_AT(uint32_t, self, 0x8) = 0;
    return self;
}

HOOK(0x00751B20, NetSpawnMessages_InitializeTimedEntry, "auto");
