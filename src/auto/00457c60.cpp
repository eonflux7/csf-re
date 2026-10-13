#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(char, __fastcall, ResetGlobalValueObject, 0x00457AF0, (void* self, void* edx));
CSF_FUNC(float*, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(bool, __fastcall, C_008342e8_IsContextActionAvailable, 0x00495730, (C_008342e8* self, void*));
CSF_FUNC(int, __fastcall, CharActionFsm_SetCharacterFlag, 0x00498F50, (void* self, void* , int flag, char enabled));
CSF_FUNC(int, __fastcall, C_00833b88_ResetActionState, 0x00499F00, (C_00833b88* self, void* , char keepWeaponState));
CSF_FUNC(int, __fastcall, CharacterModel_SetUpdateMode, 0x004F7810, (void* self, void* , int mode));
CSF_FUNC(int, __cdecl, GetGlobalValue_4FBDE0, 0x004FBDE0, ());
CSF_FUNC(int, __fastcall, GetFirstElement, 0x004FC1D0, (void* self, void*));
CSF_FUNC(void, __fastcall, C_0083f448_SetToggleMode, 0x004FC210, (C_0083f448* self, void* , int mode));
CSF_FUNC(void*, __fastcall, GetReferencedObject, 0x006683F0, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(float*, __fastcall, GfxMatrix_EulerAnglesToDirection, 0x006AE640, (float* self, void* , float* out));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);
CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_PositivePortraitAngle, 0x0083138C);
CSF_GLOBAL(float, g_NegativePortraitAngle, 0x00831390);

static char __fastcall S_00457af0_InitializeContextPortrait(
    S_00457af0* self, void* /*edx*/)
{
    // The packet also labels the reset function's address as a vtable.
    // There is no corresponding vtable store in this function.
    (void)CSF_VTBL(0x00457AF0);

    if ((self->field_000c & 0x80000000) != 0)
        return ResetGlobalValueObject(self, nullptr);

    auto* character = reinterpret_cast<C_008342e8*>(
        CHandleTable_Resolve(
            g_pCurrentScene, nullptr,
            reinterpret_cast<int>(reinterpret_cast<uint8_t*>(self) + 4)));

    auto* target = reinterpret_cast<C_008342e8*>(
        CHandleTable_Resolve(
            g_pCurrentScene, nullptr,
            reinterpret_cast<int>(reinterpret_cast<uint8_t*>(self) + 0x10)));

    if (!character || !target)
        return ResetGlobalValueObject(self, nullptr);

    CharActionFsm_SetCharacterFlag(character, nullptr, 0x80000, 1);
    const bool keepWeaponState =
        C_008342e8_IsContextActionAvailable(character, nullptr);
    C_00833b88_ResetActionState(
        reinterpret_cast<C_00833b88*>(character), nullptr, keepWeaponState);

    // The pushed zero belongs to EulerAnglesToDirection, not this slot.
    reinterpret_cast<void (__fastcall*)(void*, void*)>(
        character->vtbl[0x6C / 4])(character, nullptr);

    alignas(8) uint8_t directionBytes[0xC];
    alignas(8) uint8_t orientationBytes[0x8];

    float* angles =
        reinterpret_cast<float* (__fastcall*)(void*, void*, uint8_t*, float*)>(
            character->vtbl[0x38 / 4])(
                character, nullptr, orientationBytes,
                reinterpret_cast<float*>(directionBytes));

    GfxMatrix_EulerAnglesToDirection(angles, nullptr, nullptr);

    float* characterPosition = GetObjectPositionVector(character, nullptr);
    float* targetPosition = GetObjectPositionVector(target, nullptr);

    const float side =
        (targetPosition[0] - characterPosition[0]) *
            CSF_AT(float, directionBytes, 8) -
        (targetPosition[2] - characterPosition[2]) *
            CSF_AT(float, directionBytes, 0);

    // The original selects the positive angle for unordered comparisons too.
    self->field_0024 =
        side < g_fZero ? g_NegativePortraitAngle : g_PositivePortraitAngle;

    const int globalObject = GetGlobalValue_4FBDE0();
    const int firstElement =
        GetFirstElement(reinterpret_cast<void*>(globalObject), nullptr);
    void* provider =
        GetCachedObjectProvider(reinterpret_cast<void*>(firstElement), nullptr);

    CharacterModel_SetUpdateMode(provider, nullptr, 7);
    void* referencedObject = GetReferencedObject(provider, nullptr);
    void* timingObject =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, referencedObject, 0)[0x10 / 4])(
                referencedObject, nullptr);

    self->field_0028 =
        reinterpret_cast<double (__fastcall*)(void*, void*)>(
            CSF_AT(void**, timingObject, 0)[0x28 / 4])(
                timingObject, nullptr);

    auto* toggleObject =
        reinterpret_cast<C_0083f448*>(GetGlobalValue_4FBDE0());

    // Preserve the byte result forwarded by the original call site, despite
    // the shared binding describing SetToggleMode as void.
    return reinterpret_cast<char (__fastcall*)(C_0083f448*, void*, int)>(
        C_0083f448_SetToggleMode)(toggleObject, nullptr, 4);
}

HOOK(0x00457C60, S_00457af0_InitializeContextPortrait, "auto");
