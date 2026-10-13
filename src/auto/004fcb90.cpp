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
CSF_FUNC(void, __fastcall, InvokeMemberVirtualSlot8, 0x004F5170, (void* self, void* edx, void* data));
CSF_FUNC(C_0083f1a0*, __fastcall, C_0083f1a0_InitializeHeadModel, 0x004F7E20, (C_0083f1a0* self, void* , int width, int height));
CSF_FUNC(int, __fastcall, AttachRegionToEntity, 0x004FBBE0, (void* self, void* edx, void* a2));
CSF_FUNC(uint32_t*, __fastcall, C_0083f364_InitializeRenderObject, 0x004FBC10, (C_0083f364* self, void* , uint32_t* parameters));
CSF_FUNC(char, __fastcall, CommandoAnims_PrepareRegionManagers, 0x004FBE10, (void* self, void*));
CSF_FUNC(int, __fastcall, AnimsUnpackColorToFloats, 0x004FC0B0, (float* self, void* , int packedColor));
CSF_FUNC(int, __fastcall, C_0083f448_AddSceneRegion, 0x004FC970, (C_0083f448* self, void* , int a2, int a3, int a4, int a5, int a6));
CSF_FUNC(int, __cdecl, GetGlobalValue_8B056C, 0x00506BD0, ());
CSF_FUNC(int, __cdecl, CoreMemory_AllocUntagged, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, ResolveRwManager, 0x006B13B0, (int managerId, void* edx));
CSF_FUNC(void*, __fastcall, MenuManager_SetIntroPageData, 0x006F2850, (void* self, void* edx, void* data));
CSF_FUNC(int, __cdecl, GetGlobalValue_753A30, 0x00753A30, ());

CSF_GLOBAL(float, g_CommandoAnimsDegreesToRadians, 0x0083F380);

static int __fastcall C_0083f448_ResetRegionManager(
    C_0083f448* self, void* /*edx*/)
{
    alignas(8) uint8_t renderParametersBytes[0x24];
    alignas(8) uint8_t colorBytes[0x10];
    alignas(8) uint8_t effectParametersBytes[0x10];

    void* settings = reinterpret_cast<void*>(GetValue8B0554());
    CSF_AT(int, effectParametersBytes, 0x04) = CSF_AT(int, settings, 0x904);
    CSF_AT(int, effectParametersBytes, 0x08) = CSF_AT(int, settings, 0x908);
    auto* regionSettings = reinterpret_cast<uint8_t*>(settings) + 0x54;

    void* globalSettings = reinterpret_cast<void*>(GetGlobalValue_753A30());
    float angle = CSF_AT(float, globalSettings, 0x13C) *
                  g_CommandoAnimsDegreesToRadians;
    // The stored -1.0f sentinel is tested as a raw dword.
    if (CSF_AT(uint32_t, regionSettings, 0x8B8) != 0xBF800000)
        angle = CSF_AT(float, regionSettings, 0x8B8) *
                g_CommandoAnimsDegreesToRadians;
    uint32_t angleBits;
    std::memcpy(&angleBits, &angle, sizeof(angleBits));

    int managerId = GetGlobalInt();
    void* manager = reinterpret_cast<void*>(ResolveRwManager(managerId, nullptr));
    MenuManager_SetIntroPageData(
        manager, nullptr, reinterpret_cast<void*>(angleBits));

    auto* headStorage = reinterpret_cast<C_0083f1a0*>(CoreMemory_AllocUntagged(312));
    C_0083f1a0* head = nullptr;
    if (headStorage)
        head = C_0083f1a0_InitializeHeadModel(
            headStorage, nullptr,
            CSF_AT(int, effectParametersBytes, 0x04),
            CSF_AT(int, effectParametersBytes, 0x08));
    CSF_AT(void*, effectParametersBytes, 0x0C) = head;

    InvokeMemberVirtualSlot8(
        CSF_AT(void*, effectParametersBytes, 0x0C),
        nullptr, reinterpret_cast<void*>(angleBits));

    // The packet labels the capacity-growth call address as a vtable reference.
    (void)CSF_VTBL(0x0041E5F0);
    if (self->field_000c >= self->field_0008)
        S_0041e5f0_GrowArrayCapacity(
            reinterpret_cast<S_0041e5f0*>(
                reinterpret_cast<uint8_t*>(self) + 0x04),
            nullptr, CSF_AT(int, self, 0x10));

    uint32_t headIndex = self->field_000c;
    auto* headEntry = reinterpret_cast<uint32_t*>(self->field_0004) + headIndex;
    self->field_000c = headIndex + 1;
    *headEntry = CSF_AT(uint32_t, effectParametersBytes, 0x0C);
    CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0074), 0x04) =
        CSF_AT(uint32_t, effectParametersBytes, 0x0C);

    std::memset(colorBytes, 0, 12);
    CSF_AT(float, colorBytes, 0x0C) = 1.0f;
    AnimsUnpackColorToFloats(
        reinterpret_cast<float*>(colorBytes), nullptr,
        CSF_AT(int, regionSettings, 0x8F8));

    CSF_AT(uint32_t, renderParametersBytes, 0x00) = 2;
    std::memset(renderParametersBytes + 0x04, 0, 28);
    CSF_AT(float, renderParametersBytes, 0x20) = 1.0f;

    auto* renderStorage =
        reinterpret_cast<C_0083f364*>(CoreMemory_AllocUntagged(20));
    uint32_t* renderObject = nullptr;
    if (renderStorage)
        renderObject = C_0083f364_InitializeRenderObject(
            renderStorage, nullptr,
            reinterpret_cast<uint32_t*>(renderParametersBytes));
    self->field_0034 = reinterpret_cast<uint32_t>(renderObject);

    void* entry = reinterpret_cast<void*>(
        ResolveEntryLookupValue(renderObject, nullptr));
    reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, entry, 0)[0x44 / 4])(entry, nullptr, 1);

    entry = reinterpret_cast<void*>(ResolveEntryLookupValue(
        reinterpret_cast<void*>(self->field_0034), nullptr));
    reinterpret_cast<void(__fastcall*)(void*, void*, float*)>(
        CSF_AT(void**, entry, 0)[0x18 / 4])(
            entry, nullptr, reinterpret_cast<float*>(colorBytes));

    uint32_t regionIndex =
        C_0083f448_AddSceneRegion(self, nullptr, 0, 0, 0, 0, 0);
    void* region = nullptr;
    if (regionIndex < self->field_001c)
        region = reinterpret_cast<void**>(self->field_0014)[regionIndex];

    entry = reinterpret_cast<void*>(ResolveEntryLookupValue(region, nullptr));
    reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, entry, 0)[0x44 / 4])(entry, nullptr, 1);

    entry = reinterpret_cast<void*>(ResolveEntryLookupValue(region, nullptr));
    reinterpret_cast<void(__fastcall*)(void*, void*, float*)>(
        CSF_AT(void**, entry, 0)[0x18 / 4])(
            entry, nullptr, reinterpret_cast<float*>(colorBytes));

    entry = reinterpret_cast<void*>(ResolveEntryLookupValue(region, nullptr));
    reinterpret_cast<void(__fastcall*)(void*, void*, int, int)>(
        CSF_AT(void**, entry, 0)[0x4C / 4])(entry, nullptr, 8, 1);

    if (CSF_AT(uint8_t, regionSettings, 0x1EEC))
    {
        CSF_AT(int, effectParametersBytes, 0x04) = 0;
        CSF_AT(int, effectParametersBytes, 0x00) = 0;
        managerId = GetGlobalInt();
        manager = reinterpret_cast<void*>(ResolveRwManager(managerId, nullptr));
        self->field_0058 =
            reinterpret_cast<uint32_t(__fastcall*)(void*, void*, void*)>(
                CSF_AT(void**, manager, 0)[0x20 / 4])(
                    manager, nullptr, effectParametersBytes);

        entry = reinterpret_cast<void*>(ResolveEntryLookupValue(region, nullptr));
        reinterpret_cast<void(__fastcall*)(void*, void*, uint32_t)>(
            CSF_AT(void**, entry, 0)[0x30 / 4])(
                entry, nullptr, self->field_0058);

        int parameter = CSF_AT(int, regionSettings, 0x1EF0);
        void* effect = reinterpret_cast<void*>(self->field_0058);
        reinterpret_cast<void(__fastcall*)(void*, void*, int, int)>(
            CSF_AT(void**, effect, 0)[0x1C / 4])(
                effect, nullptr, 0, parameter);

        parameter = CSF_AT(int, regionSettings, 0x1EF4);
        effect = reinterpret_cast<void*>(self->field_0058);
        reinterpret_cast<void(__fastcall*)(void*, void*, int, int)>(
            CSF_AT(void**, effect, 0)[0x1C / 4])(
                effect, nullptr, 1, parameter);

        parameter = CSF_AT(int, regionSettings, 0x1EF8);
        effect = reinterpret_cast<void*>(self->field_0058);
        reinterpret_cast<void(__fastcall*)(void*, void*, int, int)>(
            CSF_AT(void**, effect, 0)[0x1C / 4])(
                effect, nullptr, 2, parameter);
    }

    globalSettings = reinterpret_cast<void*>(GetGlobalValue_753A30());
    int additionalEffect = CSF_AT(int, globalSettings, 0x210);
    if (additionalEffect)
    {
        CSF_AT(int, effectParametersBytes, 0x04) = 0;
        CSF_AT(int, effectParametersBytes, 0x00) = 4;
        managerId = GetGlobalInt();
        manager = reinterpret_cast<void*>(ResolveRwManager(managerId, nullptr));
        uint32_t effectAddress =
            reinterpret_cast<uint32_t(__fastcall*)(void*, void*, void*)>(
                CSF_AT(void**, manager, 0)[0x20 / 4])(
                    manager, nullptr, effectParametersBytes);
        self->field_006c = effectAddress;
        if (effectAddress)
        {
            void* effect = reinterpret_cast<void*>(effectAddress);
            reinterpret_cast<void(__fastcall*)(void*, void*, int, int)>(
                CSF_AT(void**, effect, 0)[0x1C / 4])(
                    effect, nullptr, 0, additionalEffect);

            entry = reinterpret_cast<void*>(
                ResolveEntryLookupValue(region, nullptr));
            reinterpret_cast<void(__fastcall*)(void*, void*, uint32_t)>(
                CSF_AT(void**, entry, 0)[0x30 / 4])(
                    entry, nullptr, self->field_006c);

            void* globalState =
                reinterpret_cast<void*>(GetGlobalValue_8B056C());
            void** effectVtable =
                CSF_AT(void**, reinterpret_cast<void*>(self->field_006c), 0);
            void* stateOwner = CSF_AT(void*, globalState, 0x18);
            void* state = CSF_AT(void*, stateOwner, 0x0C);
            int stateValue = reinterpret_cast<int(__fastcall*)(void*, void*)>(
                CSF_AT(void**, state, 0)[0x30 / 4])(state, nullptr);
            reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
                effectVtable[0x34 / 4])(
                    reinterpret_cast<void*>(self->field_006c),
                    nullptr, stateValue);
        }
    }

    CSF_AT(int, effectParametersBytes, 0x04) = 0;
    CSF_AT(int, effectParametersBytes, 0x00) = 1;
    managerId = GetGlobalInt();
    manager = reinterpret_cast<void*>(ResolveRwManager(managerId, nullptr));
    self->field_005c =
        reinterpret_cast<uint32_t(__fastcall*)(void*, void*, void*)>(
            CSF_AT(void**, manager, 0)[0x20 / 4])(
                manager, nullptr, effectParametersBytes);

    AttachRegionToEntity(
        region, nullptr, CSF_AT(void*, effectParametersBytes, 0x0C));

    entry = reinterpret_cast<void*>(ResolveEntryLookupValue(region, nullptr));
    void* sceneGlobals = reinterpret_cast<void*>(GetGlobal8afc78());
    reinterpret_cast<void(__fastcall*)(void*, void*, uint32_t)>(
        CSF_AT(void**, entry, 0)[0x60 / 4])(
            entry, nullptr, CSF_AT(uint32_t, sceneGlobals, 0x228));

    return CommandoAnims_PrepareRegionManagers(self, nullptr);
}

HOOK(0x004FCB90, C_0083f448_ResetRegionManager, "auto");
