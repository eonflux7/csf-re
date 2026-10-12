#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* , int additionalCapacity));
CSF_FUNC(int32_t, __cdecl, GetGlobal8afc78, 0x0042D6D0, ());
CSF_FUNC(int, __fastcall, ResolveEntryLookupValue, 0x00471540, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(void, __fastcall, InvokeMemberVirtualSlot8, 0x004F5170, (void* self, void* edx, uint32_t orientation));
CSF_FUNC(C_0083f1a0*, __fastcall, C_0083f1a0_InitializeHeadModel, 0x004F7E20, (C_0083f1a0* self, void* , int width, int height));
CSF_FUNC(int, __fastcall, AttachRegionToEntity, 0x004FBBE0, (void* self, void* edx, void* a2));
CSF_FUNC(uint32_t*, __fastcall, C_0083f364_InitializeRenderObject, 0x004FBC10, (C_0083f364* self, void* , uint32_t* parameters));
CSF_FUNC(char, __fastcall, SceneLoader_PrepareRegionManager, 0x004FBE10, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CommandoAnims_SetRegionOrientation, 0x004FC0B0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, AddSceneRegion, 0x004FC970, (void* self, void* edx, int a2, int a3, int a4, int a5, int a6));
CSF_FUNC(int, __cdecl, GetGlobalValue_8B056C, 0x00506BD0, ());
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, ResolveRwManager, 0x006B13B0, (int managerId, void* edx));
CSF_FUNC(void*, __fastcall, MenuManager_SetIntroPageData, 0x006F2850, (void* self, void* edx, void* data));
CSF_FUNC(int, __cdecl, GetGlobalValue_753A30, 0x00753A30, ());

CSF_GLOBAL(float, g_DegreesToRadians, 0x0083F380);

static char __fastcall C_0083f448_ResetRegionManager(
    C_0083f448* self, void* /*edx*/)
{
    alignas(8) uint8_t renderParametersBytes[0x24];
    alignas(8) uint8_t orientationBytes[0x10];
    alignas(8) uint8_t requestBytes[0x10];

    auto* renderParameters = reinterpret_cast<uint32_t*>(renderParametersBytes);
    auto* orientation = reinterpret_cast<float*>(orientationBytes);
    auto* request = reinterpret_cast<uint32_t*>(requestBytes);

    void* sceneSettings = reinterpret_cast<void*>(GetValue8B0554());
    request[1] = CSF_AT(uint32_t, sceneSettings, 0x904);
    request[2] = CSF_AT(uint32_t, sceneSettings, 0x908);
    auto* regionSettings = reinterpret_cast<uint8_t*>(sceneSettings) + 0x54;

    void* globalSettings = reinterpret_cast<void*>(GetGlobalValue_753A30());
    float angle = CSF_AT(float, globalSettings, 0x13C) * g_DegreesToRadians;
    if (CSF_AT(int32_t, regionSettings, 0x8B8) != -1082130432)
        angle = CSF_AT(float, regionSettings, 0x8B8) * g_DegreesToRadians;

    uint32_t angleBits;
    std::memcpy(&angleBits, &angle, sizeof(angleBits));
    request[3] = angleBits;

    void* manager = reinterpret_cast<void*>(ResolveRwManager(GetGlobalInt(), nullptr));
    MenuManager_SetIntroPageData(
        manager, nullptr, reinterpret_cast<void*>(angleBits));

    auto* headStorage = static_cast<C_0083f1a0*>(Mem_Alloc(312));
    if (headStorage)
    {
        request[3] = reinterpret_cast<uint32_t>(
            C_0083f1a0_InitializeHeadModel(
                headStorage, nullptr,
                static_cast<int>(request[1]), static_cast<int>(request[2])));
    }
    else
    {
        request[3] = 0;
    }

    InvokeMemberVirtualSlot8(reinterpret_cast<void*>(request[3]), nullptr, angleBits);

    if (self->field_000c >= self->field_0008)
    {
        // The packet also identifies this call address as a vtable reference.
        (void)CSF_VTBL(0x0041E5F0);
        S_0041e5f0_GrowArrayCapacity(
            reinterpret_cast<S_0041e5f0*>(
                reinterpret_cast<uint8_t*>(self) + 0x4),
            nullptr, CSF_AT(int, self, 0x10));
    }

    uint32_t headIndex = self->field_000c;
    auto* headSlot = reinterpret_cast<uint32_t*>(self->field_0004) + headIndex;
    self->field_000c = headIndex + 1;
    *headSlot = request[3];
    CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0074), 0x4) = request[3];

    std::memset(orientationBytes, 0, 12);
    orientation[3] = 1.0f;
    CommandoAnims_SetRegionOrientation(
        orientationBytes, nullptr, CSF_AT(int, regionSettings, 0x8F8));

    renderParameters[8] = 1065353216;
    renderParameters[0] = 2;
    std::memset(renderParameters + 1, 0, 28);

    auto* renderStorage = static_cast<C_0083f364*>(Mem_Alloc(20));
    uint32_t* renderObject = nullptr;
    if (renderStorage)
        renderObject = C_0083f364_InitializeRenderObject(
            renderStorage, nullptr, renderParameters);
    self->field_0034 = reinterpret_cast<uint32_t>(renderObject);

    void* entry = reinterpret_cast<void*>(
        ResolveEntryLookupValue(renderObject, nullptr));
    ((void(__fastcall*)(void*, void*, int))
        CSF_AT(void**, entry, 0)[0x44 / 4])(entry, nullptr, 1);

    entry = reinterpret_cast<void*>(ResolveEntryLookupValue(
        reinterpret_cast<void*>(self->field_0034), nullptr));
    ((void(__fastcall*)(void*, void*, float*))
        CSF_AT(void**, entry, 0)[0x18 / 4])(entry, nullptr, orientation);

    uint32_t regionIndex = static_cast<uint32_t>(
        AddSceneRegion(self, nullptr, 0, 0, 0, 0, 0));
    void* region = nullptr;
    if (regionIndex < self->field_001c)
        region = reinterpret_cast<void**>(self->field_0014)[regionIndex];

    entry = reinterpret_cast<void*>(ResolveEntryLookupValue(region, nullptr));
    ((void(__fastcall*)(void*, void*, int))
        CSF_AT(void**, entry, 0)[0x44 / 4])(entry, nullptr, 1);

    entry = reinterpret_cast<void*>(ResolveEntryLookupValue(region, nullptr));
    ((void(__fastcall*)(void*, void*, float*))
        CSF_AT(void**, entry, 0)[0x18 / 4])(entry, nullptr, orientation);

    entry = reinterpret_cast<void*>(ResolveEntryLookupValue(region, nullptr));
    ((void(__fastcall*)(void*, void*, int, int))
        CSF_AT(void**, entry, 0)[0x4C / 4])(entry, nullptr, 8, 1);

    if (CSF_AT(uint8_t, regionSettings, 0x1EEC))
    {
        request[1] = 0;
        request[0] = 0;
        manager = reinterpret_cast<void*>(ResolveRwManager(GetGlobalInt(), nullptr));
        self->field_0058 =
            ((uint32_t(__fastcall*)(void*, void*, uint32_t*))
                CSF_AT(void**, manager, 0)[0x20 / 4])(manager, nullptr, request);

        entry = reinterpret_cast<void*>(ResolveEntryLookupValue(region, nullptr));
        ((void(__fastcall*)(void*, void*, uint32_t))
            CSF_AT(void**, entry, 0)[0x30 / 4])(
                entry, nullptr, self->field_0058);

        uint32_t value = CSF_AT(uint32_t, regionSettings, 0x1EF0);
        void* effect = reinterpret_cast<void*>(self->field_0058);
        ((void(__fastcall*)(void*, void*, int, uint32_t))
            CSF_AT(void**, effect, 0)[0x1C / 4])(effect, nullptr, 0, value);

        value = CSF_AT(uint32_t, regionSettings, 0x1EF4);
        effect = reinterpret_cast<void*>(self->field_0058);
        ((void(__fastcall*)(void*, void*, int, uint32_t))
            CSF_AT(void**, effect, 0)[0x1C / 4])(effect, nullptr, 1, value);

        value = CSF_AT(uint32_t, regionSettings, 0x1EF8);
        effect = reinterpret_cast<void*>(self->field_0058);
        ((void(__fastcall*)(void*, void*, int, uint32_t))
            CSF_AT(void**, effect, 0)[0x1C / 4])(effect, nullptr, 2, value);
    }

    globalSettings = reinterpret_cast<void*>(GetGlobalValue_753A30());
    int additionalEffect = CSF_AT(int, globalSettings, 0x210);
    if (additionalEffect)
    {
        request[1] = 0;
        request[0] = 4;
        manager = reinterpret_cast<void*>(ResolveRwManager(GetGlobalInt(), nullptr));
        uint32_t effectAddress =
            ((uint32_t(__fastcall*)(void*, void*, uint32_t*))
                CSF_AT(void**, manager, 0)[0x20 / 4])(manager, nullptr, request);
        self->field_006c = effectAddress;

        if (effectAddress)
        {
            void* effect = reinterpret_cast<void*>(effectAddress);
            ((void(__fastcall*)(void*, void*, int, int))
                CSF_AT(void**, effect, 0)[0x1C / 4])(
                    effect, nullptr, 0, additionalEffect);

            entry = reinterpret_cast<void*>(ResolveEntryLookupValue(region, nullptr));
            ((void(__fastcall*)(void*, void*, uint32_t))
                CSF_AT(void**, entry, 0)[0x30 / 4])(
                    entry, nullptr, self->field_006c);

            void* global = reinterpret_cast<void*>(GetGlobalValue_8B056C());
            void** effectVtable = CSF_AT(
                void**, reinterpret_cast<void*>(self->field_006c), 0);
            void* owner = CSF_AT(void*, global, 0x18);
            void* source = CSF_AT(void*, owner, 0xC);
            int sourceValue =
                ((int(__fastcall*)(void*, void*))
                    CSF_AT(void**, source, 0)[0x30 / 4])(source, nullptr);
            ((void(__fastcall*)(void*, void*, int))
                effectVtable[0x34 / 4])(
                    reinterpret_cast<void*>(self->field_006c), nullptr, sourceValue);
        }
    }

    request[1] = 0;
    request[0] = 1;
    manager = reinterpret_cast<void*>(ResolveRwManager(GetGlobalInt(), nullptr));
    self->field_005c =
        ((uint32_t(__fastcall*)(void*, void*, uint32_t*))
            CSF_AT(void**, manager, 0)[0x20 / 4])(manager, nullptr, request);

    AttachRegionToEntity(region, nullptr, reinterpret_cast<void*>(request[3]));
    entry = reinterpret_cast<void*>(ResolveEntryLookupValue(region, nullptr));
    void* global = reinterpret_cast<void*>(GetGlobal8afc78());
    uint32_t sceneValue = CSF_AT(uint32_t, global, 0x228);
    ((void(__fastcall*)(void*, void*, uint32_t))
        CSF_AT(void**, entry, 0)[0x60 / 4])(entry, nullptr, sceneValue);

    return SceneLoader_PrepareRegionManager(self, nullptr);
}

HOOK(0x004FCB90, C_0083f448_ResetRegionManager, "auto");
