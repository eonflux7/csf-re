#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

using AtomicVisitorFn = int(__cdecl*)(int atomic, int data);

CSF_FUNC(int, __cdecl, RpClumpForAllAtomics, 0x0058EF00, (int clump, AtomicVisitorFn visitor, int data));
CSF_FUNC(int, __fastcall, ResolveClumpManager, 0x00652020, (int managerId, void* edx));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_6C5760, 0x006C5760, ());
CSF_FUNC(char, __fastcall, GlobalHud_Commit_6C7C30, 0x006C7C30, (C_0085e7b8* self, void*));
CSF_FUNC(int, __fastcall, GfxSkin_AppendObjectRecord, 0x006C8390, (void* self, void* , int objIndex));
CSF_FUNC(char, __fastcall, C_00861c08_SetStateFlags, 0x006E68E0, (void* self, void* edx, uint8_t stateFlags, char enableFlag));
CSF_FUNC(char, __fastcall, C_00861c08_SetStateEnabled, 0x006E6920, (void* self, void* edx, char a2));
CSF_FUNC(int, __fastcall, C_00861988_SetSecondaryParameters, 0x006FD040, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(char*, __fastcall, C_00861988_GetSecondaryParameters, 0x006FD070, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_00861988_SetTertiaryParameters, 0x006FD080, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(char*, __fastcall, C_00861988_GetTertiaryParameters, 0x006FD0B0, (void* self, void* edx));
CSF_FUNC(char, __fastcall, C_00861988_ApplyParameters, 0x006FD0E0, (void* self, void* edx));

CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);

static char __fastcall GfxObjectInstances_ApplyAtomicScale(
    void* self, void* /*edx*/, C_0085e7b8* object,
    int /*unused*/, char temporarilyDisableStates)
{
    if (!GetCachedObjectProvider(object, nullptr))
        return 0;

    int stateCount =
        reinterpret_cast<int(__fastcall*)(void*, void*)>(
            object->vtbl[0x84 / 4])(object, nullptr);
    if (stateCount > 16)
        stateCount = 16;

    uint32_t savedStates[16];
    if (temporarilyDisableStates)
    {
        for (int index = 0; index < stateCount; ++index)
        {
            auto* state =
                reinterpret_cast<C_00861c08*(__fastcall*)(void*, void*, int)>(
                    object->vtbl[0x80 / 4])(object, nullptr, index);
            savedStates[index] = state->field_0018;
            C_00861c08_SetStateEnabled(state, nullptr, 0);
        }
    }

    int global = GetGlobalValue_6C5760();
    GfxSkin_AppendObjectRecord(CSF_AT(void*, global, 0x4), nullptr, 11);

    int managerId = GetGlobalInt();
    void* manager = reinterpret_cast<void*>(
        ResolveClumpManager(managerId, nullptr));

    // The dynamic parameter object's slots differ from the generated
    // C_00861988 wrappers.
    void* parameters =
        reinterpret_cast<void*(__fastcall*)(void*, void*)>(
            CSF_AT(void**, manager, 0)[0x48 / 4])(manager, nullptr);

    alignas(8) uint32_t savedPrimary[6];
    alignas(8) uint32_t savedSecondary[4];
    alignas(8) uint32_t savedTertiary[4];
    alignas(8) uint32_t temporary[4];

    void* primary =
        reinterpret_cast<void*(__fastcall*)(void*, void*)>(
            CSF_AT(void**, parameters, 0)[0x38 / 4])(parameters, nullptr);
    savedPrimary[0] = CSF_AT(uint32_t, primary, 0x0);
    savedPrimary[1] = CSF_AT(uint32_t, primary, 0x4);
    savedPrimary[2] = CSF_AT(uint32_t, primary, 0x8);
    savedPrimary[3] = CSF_AT(uint32_t, primary, 0xC);

    char* secondary = C_00861988_GetSecondaryParameters(parameters, nullptr);
    savedSecondary[0] = CSF_AT(uint32_t, secondary, 0x0);
    savedSecondary[1] = CSF_AT(uint32_t, secondary, 0x4);
    savedSecondary[2] = CSF_AT(uint32_t, secondary, 0x8);
    savedSecondary[3] = CSF_AT(uint32_t, secondary, 0xC);

    char* tertiary = C_00861988_GetTertiaryParameters(parameters, nullptr);
    savedTertiary[0] = CSF_AT(uint32_t, tertiary, 0x0);
    savedTertiary[1] = CSF_AT(uint32_t, tertiary, 0x4);
    savedTertiary[2] = CSF_AT(uint32_t, tertiary, 0x8);
    savedTertiary[3] = CSF_AT(uint32_t, tertiary, 0xC);

    float objectScale;
    if (reinterpret_cast<void*(__fastcall*)(void*, void*)>(
            object->vtbl[0x4C / 4])(object, nullptr))
    {
        void* scaleProvider =
            reinterpret_cast<void*(__fastcall*)(void*, void*)>(
                object->vtbl[0x4C / 4])(object, nullptr);
        objectScale =
            reinterpret_cast<float(__fastcall*)(void*, void*)>(
                CSF_AT(void**, scaleProvider, 0)[0x8 / 4])(
                    scaleProvider, nullptr);
    }
    else
    {
        objectScale = g_TransitionOne;
    }

    float scale = objectScale * CSF_AT(float, self, 0x30);

    temporary[0] = 1065353216;
    temporary[1] = 1065353216;
    temporary[2] = 1065353216;
    temporary[3] = 1065353216;
    reinterpret_cast<void(__fastcall*)(void*, void*, uint32_t*)>(
        CSF_AT(void**, parameters, 0)[0x34 / 4])(
            parameters, nullptr, temporary);

    temporary[0] = 1065353216;
    temporary[1] = 1065353216;
    temporary[2] = 1065353216;
    temporary[3] = 1065353216;
    C_00861988_SetSecondaryParameters(parameters, nullptr, temporary);

    temporary[0] = 0;
    temporary[1] = 0;
    temporary[2] = 0;
    temporary[3] = 0;
    C_00861988_SetTertiaryParameters(parameters, nullptr, temporary);
    C_00861988_ApplyParameters(parameters, nullptr);

    void* clump = GetCachedObjectProvider(object, nullptr);
    RpClumpForAllAtomics(
        reinterpret_cast<int>(clump),
        reinterpret_cast<AtomicVisitorFn>(CSF_ADDR_FN(0x006E4300)),
        reinterpret_cast<int>(&scale));

    reinterpret_cast<void(__fastcall*)(void*, void*, uint32_t*)>(
        CSF_AT(void**, parameters, 0)[0x34 / 4])(
            parameters, nullptr, savedPrimary);
    C_00861988_SetSecondaryParameters(parameters, nullptr, savedSecondary);
    C_00861988_SetSecondaryParameters(parameters, nullptr, savedTertiary);
    C_00861988_ApplyParameters(parameters, nullptr);

    if (temporarilyDisableStates)
    {
        for (int index = 0; index < stateCount; ++index)
        {
            auto* state =
                reinterpret_cast<C_00861c08*(__fastcall*)(void*, void*, int)>(
                    object->vtbl[0x80 / 4])(object, nullptr, index);
            C_00861c08_SetStateFlags(
                state, nullptr, static_cast<uint8_t>(savedStates[index]), 0);
        }
    }

    global = GetGlobalValue_6C5760();
    return GlobalHud_Commit_6C7C30(
        CSF_AT(C_0085e7b8*, global, 0x4), nullptr);
}

HOOK(0x006E4680, GfxObjectInstances_ApplyAtomicScale, "auto");
