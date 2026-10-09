#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(double, __fastcall, Vector_GetLength, 0x00401FA0, (void* self, void* edx));
CSF_FUNC(float*, __fastcall, BuildFacingVectorBetween, 0x00402030, (void* self, void* edx, float* a2, float* a3));
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, SceneObj_IsInactive, 0x0048E100, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneObject_PassesFirstCheck, 0x0048E120, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(int, __fastcall, IsField1BCNonNegative, 0x00492B10, (void* self, void*));
CSF_FUNC(int, __fastcall, Hud_GetAlternateSceneSelection, 0x004950B0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, MenuExtras_ParseDescriptionId, 0x00698D90, (const char* text));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

// If no hint/action is pending and the tracked target is valid, decides whether to
// trigger message "G176" (target's definition flag set) or "G005" (target close enough).
static void __fastcall C_00833ee8_UpdateTargetHintMessage(C_00833ee8* self, void* /*edx*/)
{
    if (self->field_0298 != 0)
        return;
    if (self->field_03a0 == 0xFFFFFFFFu)
        return;
    if (static_cast<int>(self->field_0248) < 0)
        return;

    void* target = reinterpret_cast<void*>(
        CHandleTable_Resolve(g_pCurrentScene, nullptr, reinterpret_cast<int>(&self->field_0240)));
    if (target == nullptr)
        return;
    if (CSF_AT(int, target, 0x50) != 1)
        return;
    if (static_cast<uint8_t>(SceneObject_PassesFirstCheck(target, nullptr)) != 0)
        return;
    if (static_cast<uint8_t>(SceneObj_IsInactive(target, nullptr)) != 0)
        return;
    if (CSF_AT(int8_t, target, 0x1F0) < 0)
        return;
    if (static_cast<uint8_t>(IsField1BCNonNegative(target, nullptr)) != 0)
        return;

    int messageId;
    if (CSF_AT(uint8_t, CSF_AT(void*, target, 0x34), 0x33C) != 0)
    {
        self->field_0298 = 6;
        self->field_02a0 = self->field_0244;
        self->field_02a4 = self->field_0248;
        messageId = MenuExtras_ParseDescriptionId(CSF_STR(0x00833E18, "G176"));
    }
    else
    {
        int targetLevel = CSF_AT(int, target, 0x7C);
        if (targetLevel >= Hud_GetAlternateSceneSelection(self, nullptr))
            return;
        if (CSF_AT(int, target, 0x1D8) != 0)
            return;

        void* targetDef = CSF_AT(void*, target, 0x34);
        if (targetDef == nullptr)
            return;
        if (((int(__fastcall*)(void*, void*))CSF_AT(void**, targetDef, 0)[0x98 / 4])(targetDef, nullptr) != 1)
            return;

        float delta[4];
        float* targetPos = reinterpret_cast<float*>(GetObjectPositionVector(target, nullptr));
        void* selfPos = reinterpret_cast<void*>(GetObjectPositionVector(self, nullptr));
        float* diff = BuildFacingVectorBetween(selfPos, nullptr, delta, targetPos);
        float distance = static_cast<float>(Vector_GetLength(diff, nullptr));

        if (!(distance < CSF_AT(float, GetGlobalValue_8BC1E0(), 0xB0C)))
            return;

        self->field_0298 = 6;
        self->field_02a0 = self->field_0244;
        self->field_02a4 = self->field_0248;
        messageId = MenuExtras_ParseDescriptionId(CSF_STR(0x00833E10, "G005"));
    }

    self->field_02ac = static_cast<uint32_t>(messageId);
}

HOOK(0x00496750, C_00833ee8_UpdateTargetHintMessage, "auto");
