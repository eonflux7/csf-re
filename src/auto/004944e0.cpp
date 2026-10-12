#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(long double, __fastcall, Vector_GetLength, 0x00401FA0, (float* self, void*));
CSF_FUNC(float*, __fastcall, BuildFacingVectorBetween, 0x00402030, (float* self, void* , float* a2, float* a3));
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(char, __fastcall, CharActionFsm_CheckActionObjects, 0x00485BB0, (void* self, void*));
CSF_FUNC(float*, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(int, __cdecl, MenuExtras_ParseDescriptionId, 0x00698D90, (const char* text));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static void __fastcall CPlayerCharacter_EvaluateTargetStageThree(
    CPlayerCharacter* self, void* /*edx*/)
{
    if (self->field_0298 != 0 ||
        static_cast<std::int32_t>(self->field_0248) < 0)
        return;

    auto* target = reinterpret_cast<CPlayerCharacter*>(
        CHandleTable_Resolve(
            g_pCurrentScene, nullptr,
            reinterpret_cast<int>(
                reinterpret_cast<std::uint8_t*>(self) + 0x240)));

    if (!target || target->field_0050 != 24)
        return;

    if (!CharActionFsm_CheckActionObjects(
            reinterpret_cast<void*>(self->field_0044), nullptr))
        return;

    alignas(8) std::uint8_t facingBytes[0x10];
    float* targetPosition = GetObjectPositionVector(target, nullptr);
    float* playerPosition = GetObjectPositionVector(self, nullptr);
    float* facing = BuildFacingVectorBetween(
        playerPosition, nullptr,
        reinterpret_cast<float*>(facingBytes), targetPosition);

    const float distance = static_cast<float>(Vector_GetLength(facing, nullptr));
    void* globalValues = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());

    if (distance < CSF_AT(float, globalValues, 0xB00))
    {
        self->field_0298 = 4;
        self->field_02a0 = target->field_0020;
        self->field_02a4 = target->field_0024;
        self->field_02ac =
            MenuExtras_ParseDescriptionId(CSF_STR(0x00833DE8, "G003"));
    }
}

HOOK(0x004944E0, CPlayerCharacter_EvaluateTargetStageThree, "auto");
