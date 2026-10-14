#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

using AtomicVisitorFn = int (__cdecl*)(int atomic, int data);

CSF_FUNC(int, __cdecl, RpClumpForAllAtomics, 0x0058EF00, (int clump, AtomicVisitorFn visitor, int data));
CSF_FUNC(int, __fastcall, ResolveClumpManager, 0x00652020, (int managerId, void* edx));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_6C5760, 0x006C5760, ());
CSF_FUNC(char, __fastcall, GlobalHud_Commit_6C7C30, 0x006C7C30, (C_0085e7b8* self, void*));
CSF_FUNC(int, __fastcall, GfxSkin_AppendObjectRecord, 0x006C8390, (void* self, void* , int objIndex));
CSF_FUNC(char, __fastcall, C_00861c08_SetStateFlags, 0x006E68E0, (C_00861c08* self, void* , uint8_t a2, char a3));
CSF_FUNC(char, __fastcall, C_00861c08_SelectHighestGeometrySlot, 0x006E6920, (C_00861c08* self, void* , char a2));
CSF_FUNC(int, __fastcall, CLightObject_StoreSecondaryVec4AndSetFlag81, 0x006FD040, (CLightObject* self, void* , uint32_t* src));
CSF_FUNC(char*, __fastcall, C_00861988_GetSecondaryParameters, 0x006FD070, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CLightObject_StoreTertiaryVec4AndSetFlag81, 0x006FD080, (CLightObject* self, void* , uint32_t* src));
CSF_FUNC(char*, __fastcall, C_00861988_GetTertiaryParameters, 0x006FD0B0, (void* self, void* edx));
CSF_FUNC(char, __fastcall, C_00861988_ApplyParameters, 0x006FD0E0, (C_00861988* self, void*));

CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);

static char __fastcall GfxObjectInstances_ApplyAtomicScale(
    void* self, void* /*edx*/, C_0085e7b8* object,
    int /*unused*/, char disableStates)
{
    if (!GetCachedObjectProvider(object, nullptr))
        return 0;

    // 006E4683 reserves 0x8C bytes. Snapshot buffers start at -4C/-3C/-2C;
    // the scale at -14 and reusable setter argument at -10 are distinct.
    alignas(8) uint8_t workspace[0x8C];
    auto* savedStates = reinterpret_cast<uint32_t*>(workspace);
    auto* savedSecondary = reinterpret_cast<uint32_t*>(workspace + 0x40);
    auto* savedTertiary = reinterpret_cast<uint32_t*>(workspace + 0x50);
    auto* savedPrimary = reinterpret_cast<uint32_t*>(workspace + 0x60);

    int count = reinterpret_cast<int (__fastcall*)(void*, void*)>(
        object->vtbl[0x84 / 4])(object, nullptr);
    if (count > 16)
        count = 16;

    if (disableStates)
    {
        for (int i = 0; i < count; ++i)
        {
            auto* geometry =
                reinterpret_cast<C_00861c08* (__fastcall*)(void*, void*, int)>(
                    object->vtbl[0x80 / 4])(object, nullptr, i);
            savedStates[i] = static_cast<uint8_t>(geometry->field_0018);
            C_00861c08_SelectHighestGeometrySlot(geometry, nullptr, 0);
        }
    }

    int global = GetGlobalValue_6C5760();
    GfxSkin_AppendObjectRecord(
        CSF_AT(void*, reinterpret_cast<void*>(global), 0x4), nullptr, 11);

    int manager = ResolveClumpManager(GetGlobalInt(), nullptr);
    void* parameters = reinterpret_cast<void* (__fastcall*)(void*, void*)>(
        CSF_AT(void**, reinterpret_cast<void*>(manager), 0)[0x48 / 4])(
            reinterpret_cast<void*>(manager), nullptr);

    // The runtime light interface differs from the generated class wrappers.
    auto* source = reinterpret_cast<uint32_t* (__fastcall*)(void*, void*)>(
        CSF_AT(void**, parameters, 0)[0x38 / 4])(parameters, nullptr);
    savedPrimary[0] = source[0];
    savedPrimary[1] = source[1];
    savedPrimary[2] = source[2];
    savedPrimary[3] = source[3];

    source = reinterpret_cast<uint32_t*>(
        C_00861988_GetSecondaryParameters(parameters, nullptr));
    savedSecondary[0] = source[0];
    savedSecondary[1] = source[1];
    savedSecondary[2] = source[2];
    savedSecondary[3] = source[3];

    source = reinterpret_cast<uint32_t*>(
        C_00861988_GetTertiaryParameters(parameters, nullptr));
    savedTertiary[0] = source[0];
    savedTertiary[1] = source[1];
    savedTertiary[2] = source[2];
    savedTertiary[3] = source[3];

    float objectScale;
    if (reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            object->vtbl[0x4C / 4])(object, nullptr))
    {
        void* scaleProvider = reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            object->vtbl[0x4C / 4])(object, nullptr);
        objectScale = reinterpret_cast<float (__fastcall*)(void*, void*)>(
            CSF_AT(void**, scaleProvider, 0)[0x8 / 4])(scaleProvider, nullptr);
    }
    else
    {
        objectScale = g_TransitionOne;
    }

    CSF_AT(float, workspace, 0x78) = objectScale * CSF_AT(float, self, 0x30);

    CSF_AT(uint32_t, workspace, 0x7C) = 1065353216;
    CSF_AT(uint32_t, workspace, 0x80) = 1065353216;
    CSF_AT(uint32_t, workspace, 0x84) = 1065353216;
    CSF_AT(uint32_t, workspace, 0x88) = 1065353216;
    reinterpret_cast<void (__fastcall*)(void*, void*, uint32_t*)>(
        CSF_AT(void**, parameters, 0)[0x34 / 4])(
            parameters, nullptr, reinterpret_cast<uint32_t*>(workspace + 0x7C));

    CSF_AT(uint32_t, workspace, 0x7C) = 1065353216;
    CSF_AT(uint32_t, workspace, 0x80) = 1065353216;
    CSF_AT(uint32_t, workspace, 0x84) = 1065353216;
    CSF_AT(uint32_t, workspace, 0x88) = 1065353216;
    CLightObject_StoreSecondaryVec4AndSetFlag81(
        reinterpret_cast<CLightObject*>(parameters), nullptr,
        reinterpret_cast<uint32_t*>(workspace + 0x7C));

    CSF_AT(uint32_t, workspace, 0x7C) = 0;
    CSF_AT(uint32_t, workspace, 0x80) = 0;
    CSF_AT(uint32_t, workspace, 0x84) = 0;
    CSF_AT(uint32_t, workspace, 0x88) = 0;
    CLightObject_StoreTertiaryVec4AndSetFlag81(
        reinterpret_cast<CLightObject*>(parameters), nullptr,
        reinterpret_cast<uint32_t*>(workspace + 0x7C));
    C_00861988_ApplyParameters(
        reinterpret_cast<C_00861988*>(parameters), nullptr);

    void* clump = GetCachedObjectProvider(object, nullptr);
    RpClumpForAllAtomics(
        reinterpret_cast<int>(clump),
        reinterpret_cast<AtomicVisitorFn>(CSF_ADDR_FN(0x006E4300)),
        reinterpret_cast<int>(workspace + 0x78));

    reinterpret_cast<void (__fastcall*)(void*, void*, uint32_t*)>(
        CSF_AT(void**, parameters, 0)[0x34 / 4])(
            parameters, nullptr, reinterpret_cast<uint32_t*>(workspace + 0x60));
    CLightObject_StoreSecondaryVec4AndSetFlag81(
        reinterpret_cast<CLightObject*>(parameters), nullptr,
        reinterpret_cast<uint32_t*>(workspace + 0x40));
    // 006E4840 and 006E484B both reload ECX from ESI; both following calls
    // (006E4842/006E484D) use the SECONDARY setter, even for the tertiary copy.
    CLightObject_StoreSecondaryVec4AndSetFlag81(
        reinterpret_cast<CLightObject*>(parameters), nullptr,
        reinterpret_cast<uint32_t*>(workspace + 0x50));
    C_00861988_ApplyParameters(
        reinterpret_cast<C_00861988*>(parameters), nullptr);

    if (disableStates)
    {
        for (int i = 0; i < count; ++i)
        {
            auto* geometry =
                reinterpret_cast<C_00861c08* (__fastcall*)(void*, void*, int)>(
                    object->vtbl[0x80 / 4])(object, nullptr, i);
            C_00861c08_SetStateFlags(
                geometry, nullptr, static_cast<uint8_t>(savedStates[i]), 0);
        }
    }

    global = GetGlobalValue_6C5760();
    return GlobalHud_Commit_6C7C30(
        CSF_AT(C_0085e7b8*, reinterpret_cast<void*>(global), 0x4), nullptr);
}

HOOK(0x006E4680, GfxObjectInstances_ApplyAtomicScale, "auto");
