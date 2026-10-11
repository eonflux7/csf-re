#include <cstdint>
#include "hook.h"
#include "globals.h"
#include "types/auto_classes.h"

static CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
static CSF_FUNC(int, __fastcall, SceneLoader_GetObjectRegistry, 0x00726C40, (void* self, void* edx));

static char __fastcall WeaponFirearms_ReleaseRegisteredSubstate(
    void* self, void* /*edx*/, char keepSubstate)
{
    if (keepSubstate)
        return keepSubstate;

    if (!CSF_AT(std::uint32_t, self, 0x38))
        return 0;

    void* sceneLoader = reinterpret_cast<void*>(GetGlobalInt());
    void* registry = reinterpret_cast<void*>(
        SceneLoader_GetObjectRegistry(sceneLoader, nullptr));

    int result = reinterpret_cast<int (__fastcall*)(void*, void*, std::uint32_t)>(
        CSF_AT(void**, registry, 0)[0x18 / 4])(
            registry, nullptr, CSF_AT(std::uint32_t, self, 0x38));

    CSF_AT(std::uint32_t, self, 0x38) = 0;
    return static_cast<char>(result);
}

HOOK(0x00476500, WeaponFirearms_ReleaseRegisteredSubstate, "auto");
