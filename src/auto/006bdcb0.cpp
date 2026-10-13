#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RpWorldDestroy, 0x0058CBD0, (int a1));
CSF_FUNC(int, __cdecl, RpWorldForAllClumps, 0x0058CEF0, (int a1, int (__cdecl* a2)(uint32_t*, int), int a3));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(int, __fastcall, ResolveClumpManager, 0x00652020, (int managerId, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(char, __fastcall, GfxEnvironment_FinalizeResourceCleanup, 0x006BCD10, (void* self, void* edx));
CSF_FUNC(char, __stdcall, GfxEnvironment_ReleaseResourceEntry, 0x006BD2B0, (void** a1, char a2));
CSF_FUNC(int, __fastcall, Environment_TidyStringOrArray, 0x006BDA20, (void* self, void* , char flags));
CSF_FUNC(int, __fastcall, SceneLoader_ClearRenderResources, 0x006BDAE0, (void* self, void* edx));
CSF_FUNC(int, __stdcall, GfxLights_ForAllWorldLights, 0x006D1650, (int world));

static char __fastcall C_008605d8_FinalizeRenderResources(
    C_008605d8* self, void* /*edx*/)
{
    if (self->field_0091)
        SceneLoader_ClearRenderResources(self, nullptr);

    std::uint32_t recordIndex = 0;
    std::uint32_t recordOffset = 0;
    if (self->field_0084)
    {
        do
        {
            auto* records = reinterpret_cast<std::uint8_t*>(self->field_007c);
            RwMatrixDestroy(
                CSF_AT(std::uint32_t*, records + recordOffset, 0x24));
            ++recordIndex;
            recordOffset += 56;
        }
        while (recordIndex < self->field_0084);
    }

    void* records = reinterpret_cast<void*>(self->field_007c);
    self->field_0080 = 0;
    self->field_0084 = 0;
    if (records)
    {
        Environment_TidyStringOrArray(records, nullptr, 3);
        self->field_007c = 0;
    }
    self->field_008c = 0;

    auto* resourceEntry =
        reinterpret_cast<std::uint8_t*>(&self->field_002c);
    int remainingEntries = 3;
    do
    {
        GfxEnvironment_ReleaseResourceEntry(
            reinterpret_cast<void**>(resourceEntry), 1);
        resourceEntry += 0x10;
        --remainingEntries;
    }
    while (remainingEntries);

    GfxEnvironment_ReleaseResourceEntry(
        reinterpret_cast<void**>(&self->field_001c), 0);
    GfxEnvironment_ReleaseResourceEntry(
        reinterpret_cast<void**>(&self->field_000c), 0);
    GfxEnvironment_ReleaseResourceEntry(
        reinterpret_cast<void**>(&self->field_005c), 0);
    GfxEnvironment_ReleaseResourceEntry(
        reinterpret_cast<void**>(&self->field_006c), 1);

    if (self->field_0004)
    {
        int world = static_cast<int>(self->field_0004);
        int managerId = GetGlobalInt();
        ResolveClumpManager(managerId, nullptr);
        GfxLights_ForAllWorldLights(world);
        RpWorldForAllClumps(
            static_cast<int>(self->field_0004),
            reinterpret_cast<int (__cdecl*)(uint32_t*, int)>(
                CSF_ADDR_FN(0x006EE5B0)),
            0);
        RpWorldDestroy(static_cast<int>(self->field_0004));
        self->field_0004 = 0;
    }

    return GfxEnvironment_FinalizeResourceCleanup(self, nullptr);
}

HOOK(0x006BDCB0, C_008605d8_FinalizeRenderResources, "auto");
