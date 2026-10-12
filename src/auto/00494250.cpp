#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(float*, __fastcall, BuildFacingVectorBetween, 0x00402030, (float* self, void* , float* a2, float* a3));
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, WeaponFirearms_CanFire, 0x004767F0, (void* self, void*));
CSF_FUNC(int, __fastcall, SceneObject_PassesSecondCheck, 0x0048E160, (void* self, void* edx));
CSF_FUNC(float*, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(double, __fastcall, CalculateSquaredVectorLength, 0x0068E300, (void* self, void* edx));
CSF_FUNC(int, __cdecl, MenuExtras_ParseDescriptionId, 0x00698D90, (const char* text));
CSF_FUNC(int, __cdecl, IsAlternateControlKind, 0x00755860, (int kind));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

static void __fastcall CPlayerCharacter_EvaluateTargetStageOne(
    CPlayerCharacter* self, void* /*edx*/)
{
    if (self->field_0298 != 0 ||
        static_cast<int32_t>(self->field_0248) < 0)
        return;

    auto* target = reinterpret_cast<CPlayerCharacter*>(
        CHandleTable_Resolve(
            g_pCurrentScene, nullptr,
            reinterpret_cast<int>(&self->field_0240)));

    if (!target)
        return;
    if (static_cast<uint8_t>(SceneObject_PassesSecondCheck(target, nullptr)) == 0)
        return;
    if ((target->field_0170 & 0x2000) == 0)
        return;
    if (static_cast<uint8_t>(
            IsAlternateControlKind(static_cast<int>(target->field_0050))) == 0)
        return;

    alignas(8) uint8_t facingBytes[0x10];
    float& rangeSquared = CSF_AT(float, facingBytes, 0x0C);

    const int firstSettings = GetGlobalValue_8BC1E0();
    const int secondSettings = GetGlobalValue_8BC1E0();
    rangeSquared =
        CSF_AT(float, reinterpret_cast<void*>(secondSettings), 0xB14) *
        CSF_AT(float, reinterpret_cast<void*>(firstSettings), 0xB14);

    float* playerPosition = GetObjectPositionVector(self, nullptr);
    float* targetPosition = GetObjectPositionVector(target, nullptr);
    float* facingVector = BuildFacingVectorBetween(
        targetPosition, nullptr,
        reinterpret_cast<float*>(facingBytes), playerPosition);

    const double distanceSquared =
        CalculateSquaredVectorLength(facingVector, nullptr);

    // The original also proceeds for an unordered comparison.
    if (distanceSquared > rangeSquared)
        return;

    void* firearm = CSF_AT(
        void*, reinterpret_cast<void*>(self->field_0044), 0x88);
    if (!firearm)
        return;
    if (static_cast<uint8_t>(WeaponFirearms_CanFire(firearm, nullptr)) == 0)
        return;

    self->field_0298 = 9;
    self->field_02a0 = self->field_0244;
    self->field_02a4 = self->field_0248;
    self->field_02ac =
        MenuExtras_ParseDescriptionId(CSF_STR(0x00833DD8, "G008"));
}

HOOK(0x00494250, CPlayerCharacter_EvaluateTargetStageOne, "auto");
