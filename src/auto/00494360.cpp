#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(long double, __fastcall, Vector_GetLength, 0x00401FA0, (float* self, void*));
CSF_FUNC(float*, __fastcall, BuildFacingVectorBetween, 0x00402030, (float* self, void* , float* a2, float* a3));
CSF_FUNC(bool, __cdecl, IsCurrentSceneObjectInRange, 0x0041E380, (S_004e0c20* a1));
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, SceneObject_PassesFirstCheck, 0x0048E120, (void* self, void* edx));
CSF_FUNC(float*, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __cdecl, MenuExtras_ParseDescriptionId, 0x00698D90, (const char* text));
CSF_FUNC(int, __cdecl, IsAlternateControlKind, 0x00755860, (int kind));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

static void __fastcall CPlayerCharacter_EvaluateTargetStageTwo(
    CPlayerCharacter* self, void* /*edx*/)
{
    if (self->field_0298 != 0 || static_cast<int32_t>(self->field_0248) < 0)
        return;

    auto* target = reinterpret_cast<C_00833908*>(
        CHandleTable_Resolve(
            g_pCurrentScene, nullptr,
            reinterpret_cast<int>(&self->field_0240)));

    if (!target || (target->field_0170 & 0x2000) == 0)
        return;

    if (static_cast<uint8_t>(
            IsAlternateControlKind(static_cast<int>(target->field_0050))) == 0)
        return;

    if (static_cast<uint8_t>(SceneObject_PassesFirstCheck(target, nullptr)) != 0)
        return;

    void* control = reinterpret_cast<void*>(target->field_0034);
    if (!control)
        return;

    if (((int(__fastcall*)(void*, void*))
             CSF_AT(void**, control, 0)[0x98 / 4])(control, nullptr) != 13)
        return;

    if (!IsCurrentSceneObjectInRange(
            reinterpret_cast<S_004e0c20*>(
                GetObjectPositionVector(self, nullptr))))
        return;

    void* context = reinterpret_cast<void*>(GetValue8B0554());
    void* registry = CSF_AT(void*, context, 0x1FC8);
    int objectId = CSF_AT(
        int, reinterpret_cast<void*>(target->field_002c), 0xC0);

    int index = ((int(__fastcall*)(void*, void*, int))
                     CSF_AT(void**, registry, 0)[0x10 / 4])(
        registry, nullptr, objectId);

    bool needsDistanceCheck = index == -1;
    if (!needsDistanceCheck)
    {
        context = reinterpret_cast<void*>(GetValue8B0554());
        registry = CSF_AT(void*, context, 0x1FC8);
        void* entry = ((void*(__fastcall*)(void*, void*, int))
                           CSF_AT(void**, registry, 0)[0x08 / 4])(
            registry, nullptr, index);

        int relatedId = CSF_AT(int, entry, 0xC0);
        needsDistanceCheck = relatedId == -1;
        if (!needsDistanceCheck)
        {
            void* relationships = reinterpret_cast<void*>(self->field_0044);
            void* relationship = ((void*(__fastcall*)(void*, void*, int))
                                      CSF_AT(void**, relationships, 0)[0xE8 / 4])(
                relationships, nullptr, relatedId);

            needsDistanceCheck = !relationship;
            if (relationship)
            {
                needsDistanceCheck =
                    ((bool(__fastcall*)(void*, void*))
                         CSF_AT(void**, relationship, 0)[0x58 / 4])(
                        relationship, nullptr);
            }
        }
    }

    if (needsDistanceCheck)
    {
        alignas(8) uint8_t facingBytes[0x10];
        float* targetPosition = GetObjectPositionVector(target, nullptr);
        float* playerPosition = GetObjectPositionVector(self, nullptr);
        float* facing = BuildFacingVectorBetween(
            playerPosition, nullptr,
            reinterpret_cast<float*>(facingBytes), targetPosition);
        float distance = static_cast<float>(Vector_GetLength(facing, nullptr));

        void* settings = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
        if (!(distance < CSF_AT(float, settings, 0xB04)))
            return;

        self->field_0298 = 3;
        self->field_02a0 = self->field_0244;
        self->field_02a4 = self->field_0248;
    }

    self->field_02ac =
        MenuExtras_ParseDescriptionId(CSF_STR(0x00833DE0, "G002"));
}

HOOK(0x00494360, CPlayerCharacter_EvaluateTargetStageTwo, "auto");
