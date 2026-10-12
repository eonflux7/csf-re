#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* , int additionalCapacity));
CSF_FUNC(int, __fastcall, C_0085e7b8_GetSceneRegionAt, 0x004FC130, (C_0085e7b8* self, void* , int index));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(C_00860178*, __fastcall, C_00860178_Construct, 0x006B2820, (C_00860178* self, void*));
CSF_FUNC(int, __fastcall, C_00860894_Init, 0x006C45F0, (void* self, void* edx, int a2, int a3));
CSF_FUNC(int, __fastcall, C_008670f0_SetField0C, 0x006C4620, (C_008670f0* self, void* , int value));
CSF_FUNC(int, __cdecl, GetGlobalValue_6C5760, 0x006C5760, ());

static int __fastcall C_0083e124_RegisterState(
    C_0083e124* self, void* /*edx*/, void** state)
{
    const uint32_t count = CSF_AT(uint32_t, self, 0x84);
    for (uint32_t index = 0; index < count; )
    {
        C_00860178* entry = CSF_AT(C_00860178**, self, 0x7C)[index];
        const bool matches = CSF_AT(void**, entry, 0x04) == state;
        ++index;
        if (matches)
            return static_cast<int>(index);
    }

    C_00860178* entry =
        static_cast<C_00860178*>(Mem_Alloc(28));
    if (entry)
    {
        C_00860178_Construct(entry, nullptr);
        entry->vtbl = CSF_VTBL(0x00860134);
        C_00860894_Init(
            reinterpret_cast<uint8_t*>(entry) + 0x08, nullptr, -1, 0);
        CSF_AT(void**, entry, 0x04) = state;
    }

    auto* globalObject = reinterpret_cast<void*>(GetGlobalValue_6C5760());
    const int sceneRegion = C_0085e7b8_GetSceneRegionAt(
        CSF_AT(C_0085e7b8*, globalObject, 0x04), nullptr, 0);

    C_008670f0* regionHolder =
        reinterpret_cast<C_008670f0*(__fastcall*)(void*, void*)>(
            entry->vtbl[0x1C / 4])(entry, nullptr);
    C_008670f0_SetField0C(regionHolder, nullptr, sceneRegion);

    if (CSF_AT(uint32_t, self, 0x84) >=
        CSF_AT(uint32_t, self, 0x80))
    {
        // Retain the packet's additional vtable-address binding.
        (void)CSF_VTBL(0x0041E5F0);
        S_0041e5f0_GrowArrayCapacity(
            reinterpret_cast<S_0041e5f0*>(
                reinterpret_cast<uint8_t*>(self) + 0x7C),
            nullptr, CSF_AT(int, self, 0x88));
    }

    const uint32_t index = CSF_AT(uint32_t, self, 0x84);
    C_00860178** destination =
        CSF_AT(C_00860178**, self, 0x7C) + index;
    const uint32_t result = index + 1;
    CSF_AT(uint32_t, self, 0x84) = result;
    *destination = entry;
    return static_cast<int>(result);
}

HOOK(0x006B1E50, C_0083e124_RegisterState, "auto");
