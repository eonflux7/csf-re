#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(unsigned int, __fastcall, FindSpawnByName, 0x0075CE80, (void* self, void* edx, char* String1));

static void* GetSpawnManager()
{
    void* root = reinterpret_cast<void*>(GetValue8B0554());
    return reinterpret_cast<void*>(CSF_AT(uint32_t, root, 0x1FD8));
}

static int __fastcall SetTdmSpawnHandles(void* self, void* /*edx*/)
{
    void* manager = GetSpawnManager();
    CSF_AT(uint32_t, self, 0x13E4) = FindSpawnByName(
        manager, nullptr, const_cast<char*>(CSF_STR(0x00834454, "TDMSpawnAliados")));
    unsigned int result = FindSpawnByName(
        manager, nullptr, const_cast<char*>(CSF_STR(0x00834440, "TDMSpawnAlemanes")));
    CSF_AT(uint32_t, self, 0x13E8) = result;
    return static_cast<int>(result);
}

static int __fastcall SetPmSpawnHandles(void* self, void* /*edx*/)
{
    void* manager = GetSpawnManager();
    CSF_AT(uint32_t, self, 0x13F8) = FindSpawnByName(
        manager, nullptr, const_cast<char*>(CSF_STR(0x00844664, "PMInicialesAliados")));
    CSF_AT(uint32_t, self, 0x13FC) = FindSpawnByName(
        manager, nullptr, const_cast<char*>(CSF_STR(0x00844650, "PMInicialesAlemanes")));
    CSF_AT(uint32_t, self, 0x13E4) = FindSpawnByName(
        manager, nullptr, const_cast<char*>(CSF_STR(0x00844640, "PMSpawnAliados")));
    unsigned int result = FindSpawnByName(
        manager, nullptr, const_cast<char*>(CSF_STR(0x00844630, "PMSpawnAlemanes")));
    CSF_AT(uint32_t, self, 0x13E8) = result;
    return static_cast<int>(result);
}

static int __fastcall SetSabSpawnHandles(void* self, void* /*edx*/)
{
    void* manager = GetSpawnManager();
    CSF_AT(uint32_t, self, 0x13E4) = FindSpawnByName(
        manager, nullptr, const_cast<char*>(CSF_STR(0x00834478, "SABSpawnAliados")));
    CSF_AT(uint32_t, self, 0x13E8) = FindSpawnByName(
        manager, nullptr, const_cast<char*>(CSF_STR(0x00834464, "SABSpawnAlemanes")));
    unsigned int result = FindSpawnByName(
        manager, nullptr, const_cast<char*>(CSF_STR(0x00844678, "SABBombasSabotage")));
    CSF_AT(uint32_t, self, 0x13F0) = result;
    return static_cast<int>(result);
}

static int __fastcall SetupSpawnPointsForGameMode(void* self, void* /*edx*/)
{
    int mode = CSF_AT(int, reinterpret_cast<void*>(GetGlobalState_50D190()), 0xCC);
    switch (mode)
    {
    case 0:
    {
        void* manager = GetSpawnManager();
        unsigned int result = FindSpawnByName(
            manager, nullptr, const_cast<char*>(CSF_STR(0x00834438, "DMSpawn")));
        CSF_AT(uint32_t, self, 0x13EC) = result;
        return static_cast<int>(result);
    }
    case 1:
        return SetTdmSpawnHandles(self, nullptr);
    case 2:
        return SetPmSpawnHandles(self, nullptr);
    case 3:
        return SetSabSpawnHandles(self, nullptr);
    default:
        return mode;
    }
}

HOOK(0x00552920, SetupSpawnPointsForGameMode, "auto");
