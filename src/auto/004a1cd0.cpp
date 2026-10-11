#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(uint32_t, __fastcall, WaypointGraph_FindSpawnByName, 0x0075CE80, (void* self, void* , char* searchName));

static uint8_t* ToBytePtr(int address)
{
    return reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(address));
}

// Original 0x004A1C20: caches the team-deathmatch spawn points.
static int __fastcall HudAlertIcons_CacheTdmSpawns(S_004a1cd0* self, void* /*edx*/)
{
    uint8_t* root = ToBytePtr(GetValue8B0554());
    void* spawnManager = CSF_AT(void*, root, 0x1FD8);
    self->field_0014 = WaypointGraph_FindSpawnByName(spawnManager, nullptr, const_cast<char*>(CSF_STR(0x834454, "TDMSpawnAliados")));
    uint32_t axisSpawn = WaypointGraph_FindSpawnByName(spawnManager, nullptr, const_cast<char*>(CSF_STR(0x834440, "TDMSpawnAlemanes")));
    self->field_0018 = axisSpawn;
    return static_cast<int>(axisSpawn);
}

// Original 0x004A1C50: caches the SAB spawn points.
static int __fastcall HudAlertIcons_CacheSabSpawns(S_004a1cd0* self, void* /*edx*/)
{
    uint8_t* root = ToBytePtr(GetValue8B0554());
    void* spawnManager = CSF_AT(void*, root, 0x1FD8);
    self->field_0014 = WaypointGraph_FindSpawnByName(spawnManager, nullptr, const_cast<char*>(CSF_STR(0x834478, "SABSpawnAliados")));
    uint32_t axisSpawn = WaypointGraph_FindSpawnByName(spawnManager, nullptr, const_cast<char*>(CSF_STR(0x834464, "SABSpawnAlemanes")));
    self->field_0018 = axisSpawn;
    return static_cast<int>(axisSpawn);
}

static int __fastcall HudAlertIcons_CacheModeSpawns(S_004a1cd0* self, void* /*edx*/)
{
    int result = GetGlobalState_50D190();
    uint8_t* state = ToBytePtr(result);
    if (CSF_AT(uint8_t, state, 0xD0))
    {
        uint8_t* stateAgain = ToBytePtr(GetGlobalState_50D190());
        int modeIndex = CSF_AT(int, stateAgain, 0xCC);
        if (modeIndex == 0)
        {
            // Deathmatch
            uint8_t* root = ToBytePtr(GetValue8B0554());
            void* spawnManager = CSF_AT(void*, root, 0x1FD8);
            uint32_t dmSpawn = WaypointGraph_FindSpawnByName(spawnManager, nullptr, const_cast<char*>(CSF_STR(0x834438, "DMSpawn")));
            self->field_001c = dmSpawn;
            result = static_cast<int>(dmSpawn);
        }
        else if (modeIndex == 1)
        {
            return HudAlertIcons_CacheTdmSpawns(self, nullptr);
        }
        else if (modeIndex == 3)
        {
            return HudAlertIcons_CacheSabSpawns(self, nullptr);
        }
        else
        {
            result = modeIndex - 3;
        }
    }
    return result;
}

HOOK(0x004A1CD0, HudAlertIcons_CacheModeSpawns, "auto");
