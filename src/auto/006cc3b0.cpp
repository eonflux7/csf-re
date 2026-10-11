#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* , int additionalCapacity));
CSF_FUNC(int, __cdecl, RpWorldAddClump, 0x0058E280, (int a1, uint32_t* a2));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, GlobalInt_ResolveAccessor, 0x006B13A0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GfxScene_SetClumpState, 0x006B2D10, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, GfxScene_ConstructClumpInstance, 0x006B4420, (void* self, void* edx, void* a2, int a3, void* a4, void* a5, int a6, int a7));
CSF_FUNC(int, __fastcall, C_00860bb8_GetWorld, 0x006CBEF0, (void* self, void* edx));
CSF_FUNC(unsigned int, __fastcall, GfxScene_ResolveResourceWithVariant, 0x006D5920, (void* self, void* edx, char* String2, uint32_t* a3));
CSF_FUNC(unsigned int, __fastcall, GfxScene_ResolveResource, 0x006D59B0, (void* self, void* edx, char* String2));

static uint32_t* __fastcall C_00860bb8_CreateClumpFromDescription(
    C_00860bb8* self, void* /*edx*/, int descriptionAddress)
{
    void* description = reinterpret_cast<void*>(descriptionAddress);
    char* name = CSF_AT(char*, description, 0x00);
    if (!name || !*name)
        return nullptr;

    int global = GetGlobalInt();
    void* accessor = reinterpret_cast<void*>(
        GlobalInt_ResolveAccessor(reinterpret_cast<void*>(global), nullptr));

    alignas(8) uint8_t variantBytes[0x10] = {};
    uint32_t* variant = reinterpret_cast<uint32_t*>(variantBytes);

    name = CSF_AT(char*, description, 0x00);
    void* provider = GetCachedObjectProvider(accessor, nullptr);
    unsigned int resource = GfxScene_ResolveResourceWithVariant(
        provider, nullptr, name, variant);

    int option6 = CSF_AT(int, description, 0x10);
    int option7 = CSF_AT(int, description, 0x14);
    char* secondaryName = CSF_AT(char*, description, 0x08);
    unsigned int secondaryResource = -1;
    unsigned int tertiaryResource = -1;

    if (secondaryName && *secondaryName)
    {
        provider = GetCachedObjectProvider(accessor, nullptr);
        secondaryResource = GfxScene_ResolveResource(
            provider, nullptr, secondaryName);
    }

    char* tertiaryName = CSF_AT(char*, description, 0x0C);
    if (tertiaryName && *tertiaryName)
    {
        provider = GetCachedObjectProvider(accessor, nullptr);
        tertiaryResource = GfxScene_ResolveResource(
            provider, nullptr, tertiaryName);
    }

    void* storage = Mem_Alloc(212);
    void* instance = nullptr;
    if (storage)
    {
        instance = reinterpret_cast<void*>(GfxScene_ConstructClumpInstance(
            storage, nullptr, reinterpret_cast<void*>(resource),
            static_cast<int>(*variant),
            reinterpret_cast<void*>(secondaryResource),
            reinterpret_cast<void*>(tertiaryResource), option6, option7));
    }

    if (GetCachedObjectProvider(instance, nullptr))
    {
        if (CSF_AT(uint8_t, description, 0x04))
        {
            int world = C_00860bb8_GetWorld(self, nullptr);
            if (world)
            {
                void* clump = GetCachedObjectProvider(instance, nullptr);
                RpWorldAddClump(world, reinterpret_cast<uint32_t*>(clump));
            }
        }
    }

    GfxScene_SetClumpState(instance, nullptr, 0);

    if (self->field_001c >= self->field_0018)
    {
        // Retain the packet's address annotation without adding a vtable write.
        (void)CSF_VTBL(0x0041E5F0);
        S_0041e5f0_GrowArrayCapacity(
            reinterpret_cast<S_0041e5f0*>(
                reinterpret_cast<uint8_t*>(self) + 0x14),
            nullptr, CSF_AT(int, self, 0x20));
    }

    uint32_t count = self->field_001c;
    void** slot = reinterpret_cast<void**>(self->field_0014) + count;
    self->field_001c = count + 1;
    *slot = instance;
    return reinterpret_cast<uint32_t*>(instance);
}

HOOK(0x006CC3B0, C_00860bb8_CreateClumpFromDescription, "auto");
