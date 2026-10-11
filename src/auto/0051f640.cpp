#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
static CSF_FUNC(int, __fastcall, SceneLoader_GetObjectRegistry, 0x00726C40, (void* self, void* edx));

static int __stdcall ParticleManager_LookUpObject(int objectId, int /*unused*/, std::uint32_t* output)
{
    *output = 0xFFFFFFFF;
    void* sceneLoader = reinterpret_cast<void*>(GetGlobalInt());
    void* registry = reinterpret_cast<void*>(
        SceneLoader_GetObjectRegistry(sceneLoader, nullptr));
    return reinterpret_cast<int (__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, registry, 0)[5])(registry, nullptr, objectId);
}

HOOK(0x0051F640, ParticleManager_LookUpObject, "auto");
