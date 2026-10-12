#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, _rpAtomicResyncInterpolatedSphere, 0x0058EB40, (int a1));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, GlobalInt_ResolveAccessor, 0x006B13A0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_00860288_PrepareRenderState, 0x006B2F90, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_00860288_ApplyObjectConfiguration, 0x006B3C30, (void* self, void* edx, int a2));
CSF_FUNC(char*, __fastcall, C_00860288_UpdateObjectResources, 0x006B48D0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue_6C5760, 0x006C5760, ());
CSF_FUNC(unsigned int, __fastcall, C_00860288_ConfigureObjectRenderRegions, 0x006C6C60, (C_00860288* self, void* , C_0085e7b8* object));
CSF_FUNC(int, __fastcall, SceneLoader_GetSector, 0x006D4720, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, GfxObject_InitializeAttachment, 0x006E4480, (void* self, void* edx, void* a2));
CSF_FUNC(char, __fastcall, GfxObject_FinalizeAtomic, 0x006E6A90, (void* self, void* edx));
CSF_FUNC(int*, __fastcall, C_00861e98_Construct, 0x006E8F50, (C_00861e98* self, void*));
CSF_FUNC(char, __fastcall, GfxHanim_InitializeClump, 0x006E9FF0, (void* self, void* , int clump, int animation, char saveMatrices, char attachHierarchy));

CSF_GLOBAL(float, g_WheelTravelMultiplier, 0x0082E24C);
CSF_GLOBAL(float, g_ControlHoldThreshold, 0x0083BB18);

static int __fastcall C_00860288_InitializeFromConfiguration(
    C_00860288* self, void* /*edx*/, int configurationAddress)
{
    void* configuration = reinterpret_cast<void*>(configurationAddress);

    const uint32_t initialFlags = CSF_AT(uint32_t, configuration, 0);
    self->field_0064 = initialFlags;
    self->field_0064 = initialFlags | 0x80000;

    bool hasMultipleMorphTargets = false;
    if (self->field_001c)
    {
        auto* entry = reinterpret_cast<uint8_t*>(self->field_0018);
        unsigned int index = 0;
        do
        {
            void* object = CSF_AT(void*, entry, 0);
            void* atomic = CSF_AT(void*, object, 0x0C);
            void* geometry = CSF_AT(void*, atomic, 0x18);
            if (CSF_AT(int, geometry, 0x18) > 1)
            {
                hasMultipleMorphTargets = true;
                break;
            }
            ++index;
            entry += 4;
        }
        while (index < self->field_001c);
    }

    if (hasMultipleMorphTargets)
        self->field_0064 |= 0x10000;
    else
        self->field_0064 &= 0xFFFEFFFF;

    self->vSlot48((self->field_0064 & 0x80) == 0x80);

    if ((self->field_0064 & 3) != 0)
    {
        int animation = 0;
        void* globalObject = reinterpret_cast<void*>(GetGlobalInt());
        void* accessor = reinterpret_cast<void*>(
            GlobalInt_ResolveAccessor(globalObject, nullptr));

        if (self->field_0010 != static_cast<uint32_t>(-1))
        {
            const int sectorIndex = static_cast<int>(self->field_0010);
            void* provider = GetCachedObjectProvider(accessor, nullptr);
            void* sector = reinterpret_cast<void*>(
                SceneLoader_GetSector(provider, nullptr, sectorIndex));
            animation = CSF_AT(int, sector, 0x30);
        }

        auto* storage = static_cast<C_00861e98*>(Mem_Alloc(160));
        int* animationState = storage
            ? C_00861e98_Construct(storage, nullptr)
            : nullptr;

        const uint32_t flags = self->field_0064;
        self->field_0028 = reinterpret_cast<uint32_t>(animationState);
        const char saveMatrices = (flags & 4) == 4;
        const char attachHierarchy = static_cast<char>((~(flags >> 1)) & 1);
        if (!GfxHanim_InitializeClump(
                animationState, nullptr, static_cast<int>(self->field_000c),
                animation, saveMatrices, attachHierarchy))
        {
            self->field_0064 &= 0xFFFFFFFC;
        }
    }

    if (self->field_001c == 1 && !self->field_0028)
        self->field_0064 &= ~0x20u;

    C_00860288_PrepareRenderState(self, nullptr);
    C_00860288_ApplyObjectConfiguration(self, nullptr, configurationAddress);
    C_00860288_UpdateObjectResources(self, nullptr);

    auto* regionManager = reinterpret_cast<C_00860288*>(GetGlobalValue_6C5760());
    C_00860288_ConfigureObjectRenderRegions(
        regionManager, nullptr, reinterpret_cast<C_0085e7b8*>(self));

    unsigned int index = 0;
    const uint32_t sphereFlags = self->field_0064;
    if ((sphereFlags & 8) != 0 && (sphereFlags & 3) != 0 && self->field_001c)
    {
        do
        {
            void* object = CSF_AT(
                void*, reinterpret_cast<void*>(self->field_0018), 4 * index);
            void* atomic = CSF_AT(void*, object, 0x0C);
            if (atomic)
            {
                if ((CSF_AT(uint8_t, atomic, 0x50) & 2) != 0)
                    _rpAtomicResyncInterpolatedSphere(reinterpret_cast<int>(atomic));
                CSF_AT(float, atomic, 0x28) =
                    CSF_AT(float, atomic, 0x28) * g_WheelTravelMultiplier;
            }
            ++index;
        }
        while (index < self->field_001c);
        index = 0;
    }

    const uint32_t animationFlags = self->field_0064;
    if ((animationFlags & 4) == 0 && (animationFlags & 1) != 0 && self->field_001c)
    {
        do
        {
            void* object = CSF_AT(
                void*, reinterpret_cast<void*>(self->field_0018), 4 * index);
            void* atomic = CSF_AT(void*, object, 0x0C);
            if (atomic)
            {
                if ((CSF_AT(uint8_t, atomic, 0x50) & 2) != 0)
                    _rpAtomicResyncInterpolatedSphere(reinterpret_cast<int>(atomic));
                CSF_AT(float, atomic, 0x28) =
                    CSF_AT(float, atomic, 0x28) * g_ControlHoldThreshold;
            }
            ++index;
        }
        while (index < self->field_001c);
    }

    if (!self->field_007c && (CSF_AT(uint32_t, configuration, 0) & 0x4000) != 0)
    {
        void* attachment = Mem_Alloc(52);
        if (attachment)
        {
            CSF_AT(uint8_t, attachment, 0) = 0;
            CSF_AT(uint32_t, attachment, 0x04) = 0;
            CSF_AT(uint32_t, attachment, 0x08) = 0;
            CSF_AT(uint32_t, attachment, 0x0C) = 0;
            CSF_AT(uint32_t, attachment, 0x10) = 0;
            CSF_AT(uint32_t, attachment, 0x14) = 0;
            CSF_AT(uint32_t, attachment, 0x18) = 0;
        }
        self->field_007c = reinterpret_cast<uint32_t>(attachment);
        GfxObject_InitializeAttachment(attachment, nullptr, self);
    }

    for (unsigned int atomicIndex = 0; atomicIndex < self->field_001c; ++atomicIndex)
    {
        GfxObject_FinalizeAtomic(
            CSF_AT(void*, reinterpret_cast<void*>(self->field_0018), 4 * atomicIndex),
            nullptr);
    }

    self->field_00bc = CSF_AT(uint8_t, configuration, 0xA8);
    self->field_00c0 = CSF_AT(uint32_t, configuration, 0xAC);
    self->field_00c4 = CSF_AT(uint32_t, configuration, 0xB0);
    const int result = CSF_AT(int, configuration, 0xB4);
    self->field_00c8 = static_cast<uint32_t>(result);
    return result;
}

HOOK(0x006B4A50, C_00860288_InitializeFromConfiguration, "auto");
