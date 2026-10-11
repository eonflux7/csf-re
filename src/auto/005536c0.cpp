#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __stdcall, SceneLoader_RegisterObject, 0x004E8DF0, (C_00867260* object));
CSF_FUNC(C_00833908*, __fastcall, S_004e9090_ResolveObject, 0x004EEEF0, (S_004e9090* self, void* edx, int nameKey, uint32_t param, char altMode));
CSF_FUNC(int, __fastcall, sub_73F730, 0x0073F730, (void* self, void* edx, int a2));

CSF_GLOBAL(std::int32_t, g_NextUniqueId, 0x008A669C);

// Resolves the spawn-point object keyed by the owner's name key, assigns it a
// unique id, registers it with the scene loader and then runs its slot 0x6C hook.
static C_00833908* __stdcall MpSpawn_ResolveAndRegister(int a1)
{
    uint8_t* ownerBase = a1 ? reinterpret_cast<uint8_t*>(a1 - 1712) : nullptr;
    int nameKey = CSF_AT(int, ownerBase, 1372);

    S_004e9090* resolver = reinterpret_cast<S_004e9090*>(GetValue8B0554());
    C_00833908* result = S_004e9090_ResolveObject(resolver, nullptr, nameKey, 0xFFFFFFFF, 1);
    C_00833908* spawn = result;

    if (spawn) {
        sub_73F730(reinterpret_cast<uint8_t*>(spawn) + 0x14, nullptr, a1);

        std::int32_t uniqueId = g_NextUniqueId++;
        spawn->field_0020 = uniqueId;
        spawn->field_0024 = -1;

        GetValue8B0554();
        SceneLoader_RegisterObject(reinterpret_cast<C_00867260*>(spawn));

        using SlotFn = C_00833908* (__fastcall*)(void* self, void* edx, int arg);
        SlotFn slot = reinterpret_cast<SlotFn>(CSF_AT(void**, spawn, 0)[0x6C / 4]);
        return slot(spawn, nullptr, 0);
    }
    return result;
}

HOOK(0x005536C0, MpSpawn_ResolveAndRegister, "auto");
