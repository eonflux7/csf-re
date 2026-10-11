#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CScene_RemoveObjectSlot, 0x00453390, (CScene* self, void* , int a2));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __stdcall, SceneLoader_RegisterObject, 0x004E8DF0, (C_00867260* object));
CSF_FUNC(C_00833908*, __fastcall, S_004e9090_ResolveObject, 0x004EEEF0, (S_004e9090* self, void* , int nameKey, uint32_t param, char altMode));
CSF_FUNC(int, __fastcall, SpawnPoints_AttachObjectReference, 0x0073F730, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, SpawnPoints_DetachObjectReference, 0x0073F750, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GetValueIfNotFlagged, 0x007408C0, (void* self, void*));

CSF_GLOBAL(std::int32_t, g_NextUniqueId, 0x008A669C);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static char __stdcall MpSpawnPoints_RefreshSpawnObject(int spawnAddress)
{
    auto* spawn = reinterpret_cast<std::uint8_t*>(spawnAddress);
    void* reference = spawn + 0x6B0;

    const bool active =
        reinterpret_cast<bool (__fastcall*)(void*, void*)>(
            CSF_AT(void**, reference, 0)[2])(reference, nullptr);
    if (active)
        return static_cast<char>(active);

    const char pending = CSF_AT(char, spawn, 0x718);
    CSF_AT(char, spawn, 0x718) = 0;
    if (!pending)
        return pending;

    const int previousReference = GetValueIfNotFlagged(reference, nullptr);
    if (previousReference && previousReference != 20)
    {
        auto* previousObject = reinterpret_cast<std::uint8_t*>(
            static_cast<std::uint32_t>(previousReference) - 20);

        CScene_RemoveObjectSlot(
            reinterpret_cast<CScene*>(g_pCurrentScene), nullptr,
            reinterpret_cast<int>(previousObject + 28));
        SpawnPoints_DetachObjectReference(previousObject + 20, nullptr);
    }

    const int nameKey = CSF_AT(int, spawn, 0x55C);
    auto* loader = reinterpret_cast<S_004e9090*>(GetValue8B0554());
    C_00833908* object =
        S_004e9090_ResolveObject(loader, nullptr, nameKey, 0xFFFFFFFF, 1);
    if (!object)
        return 0;

    SpawnPoints_AttachObjectReference(
        reinterpret_cast<std::uint8_t*>(object) + 0x14, nullptr,
        reinterpret_cast<int>(reference));

    const std::int32_t uniqueId = g_NextUniqueId;
    g_NextUniqueId = static_cast<std::int32_t>(
        static_cast<std::uint32_t>(uniqueId) + 1);
    object->field_0020 = uniqueId;
    object->field_0024 = -1;

    GetValue8B0554();
    SceneLoader_RegisterObject(reinterpret_cast<C_00867260*>(object));

    void* component = reinterpret_cast<void*>(object->vGetField016c());
    return reinterpret_cast<char (__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, component, 0)[15])(component, nullptr, 0);
}

HOOK(0x005531A0, MpSpawnPoints_RefreshSpawnObject, "auto");
