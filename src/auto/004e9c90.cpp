#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_4FBDE0, 0x004FBDE0, ());
CSF_FUNC(int, __fastcall, GetFirstElement, 0x004FC1D0, (void* self, void*));
CSF_FUNC(int, __cdecl, GetGlobalValue_8B0568, 0x004FDBB0, ());
CSF_FUNC(void*, __fastcall, GetReferencedObject, 0x006683F0, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(void*, __cdecl, CreateMissionSetupLoader, 0x006CDE90, ());

namespace {
struct MissionSetupNames
{
    const char* worldName;    // from global config string at +0x7C
    const char* missionName;  // string member at self+0x1F54
    const char* sceneName;    // string member at self+0x8D4
};
}

// Builds the three name strings (global config + two std::string members) and hands them,
// together with the referenced scene object, to a temporary loader object which is then destroyed.
static char __fastcall C_0085e7b8_RunMissionSetupLoader(C_0085e7b8* self, void* /*edx*/)
{
    void* registry = reinterpret_cast<void*>(GetGlobalValue_4FBDE0());
    void* firstElement = reinterpret_cast<void*>(GetFirstElement(registry, nullptr));
    void* provider = GetCachedObjectProvider(firstElement, nullptr);
    void* referencedObject = GetReferencedObject(provider, nullptr);

    MissionSetupNames names;
    names.worldName = nullptr;
    names.missionName = nullptr;
    names.sceneName = nullptr;

    int config = GetGlobalValue_8B0568();
    if (CSF_AT(uint32_t, config, 0x94) < 0x10)
        names.worldName = reinterpret_cast<const char*>(config) + 0x7C + 4;
    else
        names.worldName = CSF_AT(const char*, config, 0x80);

    if (CSF_AT(uint32_t, self, 0x1F6C) < 0x10)
        names.missionName = reinterpret_cast<const char*>(self) + 0x1F58;
    else
        names.missionName = CSF_AT(const char*, self, 0x1F58);

    if (CSF_AT(uint32_t, self, 0x8EC) < 0x10)
        names.sceneName = reinterpret_cast<const char*>(self) + 0x8D8;
    else
        names.sceneName = CSF_AT(const char*, self, 0x8D8);

    void* loader = CreateMissionSetupLoader();
    char result = ((char(__fastcall*)(void*, void*, void*, MissionSetupNames*))CSF_AT(void**, loader, 0)[1])(
        loader, nullptr, referencedObject, &names);
    ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, loader, 0)[0])(loader, nullptr, 1);
    return result;
}

HOOK(0x004E9C90, C_0085e7b8_RunMissionSetupLoader, "auto");
