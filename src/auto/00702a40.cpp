#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* , int additionalCapacity));
CSF_FUNC(int, __cdecl, CoreMemory_AllocUntagged, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, GlobalInt_ResolveAccessor, 0x006B13A0, (void* self, void* edx));
CSF_FUNC(int, __stdcall, GlobalInt_FindNameId, 0x006C1770, (uint8_t* a1));
CSF_FUNC(int, __cdecl, GetGlobalValue_6C5760, 0x006C5760, ());
CSF_FUNC(int, __fastcall, GfxSkin_ApplySceneRegionToObject, 0x006C58D0, (C_0085e7b8** self, void* , int a2));
CSF_FUNC(int, __fastcall, C_00861988_FindEmitterIndex, 0x00702990, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, C_008636fc_Construct, 0x0070B760, (C_008636fc* self, void* , int a2, int a3));
CSF_FUNC(char, __fastcall, Water_UpdateEntry, 0x0070B960, (void* self, void* , int arg, int key));

static char __fastcall C_00861988_UpdateNamedEmitterEntry(
    C_00861988* self, void* /*edx*/, int emitterKey, int entryArgument)
{
    const int index = C_00861988_FindEmitterIndex(self, nullptr, emitterKey);
    int emitterAddress;

    if (index == -1) {
        auto* memory = reinterpret_cast<C_008636fc*>(
            CoreMemory_AllocUntagged(0x24));

        emitterAddress = memory
            ? C_008636fc_Construct(
                  memory, nullptr, reinterpret_cast<int>(self), emitterKey)
            : 0;

        auto* emitters = reinterpret_cast<S_0041e5f0*>(
            reinterpret_cast<uint8_t*>(self) + 0x10);

        if (self->field_0018 >= self->field_0014) {
            // Preserve the packet's address binding for this annotation.
            (void)CSF_VTBL(0x0041E5F0);
            S_0041e5f0_GrowArrayCapacity(
                emitters, nullptr, emitters->field_000c);
        }

        const uint32_t count = emitters->field_0008;
        int* slot = reinterpret_cast<int*>(emitters->field_0000) + count;
        emitters->field_0008 = count + 1;
        *slot = emitterAddress;

        const int scene = GetGlobalValue_6C5760();
        GfxSkin_ApplySceneRegionToObject(
            reinterpret_cast<C_0085e7b8**>(scene), nullptr, emitterAddress);
    } else {
        auto* emitters = reinterpret_cast<S_0041e5f0*>(
            reinterpret_cast<uint8_t*>(self) + 0x10);
        emitterAddress = reinterpret_cast<int*>(emitters->field_0000)[index];
    }

    auto* argument = reinterpret_cast<uint8_t*>(entryArgument);
    uint8_t* name;
    if (CSF_AT(uint32_t, argument, 0x18) < 0x10u)
        name = argument + 4;
    else
        name = CSF_AT(uint8_t*, argument, 0x04);

    int nameId = 0;
    if (std::strlen(reinterpret_cast<const char*>(name)) != 0) {
        const int global = GetGlobalInt();
        void* accessor = reinterpret_cast<void*>(
            GlobalInt_ResolveAccessor(reinterpret_cast<void*>(global), nullptr));

        ((int (__fastcall*)(void*, void*))
            CSF_AT(void**, accessor, 0)[0x10 / 4])(accessor, nullptr);

        nameId = GlobalInt_FindNameId(name);
    }

    return Water_UpdateEntry(
        reinterpret_cast<void*>(emitterAddress), nullptr, entryArgument, nameId);
}

HOOK(0x00702A40, C_00861988_UpdateNamedEmitterEntry, "auto");
