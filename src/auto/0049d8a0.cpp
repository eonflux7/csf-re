#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, C_008310f8_Initialize, 0x00446550, (C_008310f8* self, void* , int owner));
CSF_FUNC(uint32_t*, __fastcall, CGuiControl_SubmitVertexRecord, 0x00455DB0, (CGuiControl* self, void* , int a2, uint32_t* a3));
CSF_FUNC(int, __cdecl, GetGlobalObjectField184, 0x00475470, ());
CSF_FUNC(uint32_t, __fastcall, DispatchRecordToGlobalObject, 0x004758B0, (void* self, void* edx, void* record, char flag));
CSF_FUNC(int, __stdcall, GetGlobal8B0578Thunk, 0x0048AA60, (int , int , int));
CSF_FUNC(uint32_t*, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __fastcall, FxParticleManager_FindSceneEntryAtPosition, 0x004EF6E0, (void* self, void* edx, void* a2, char a3));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());

CSF_GLOBAL(float, g_MessageBoxAnimationScale, 0x0082D7C0);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static int __fastcall C_00834050_EmitSceneEffect(C_00834050* self, void* /*edx*/)
{
    void* globalObject = reinterpret_cast<void*>(GetValue8B0554());
    void* definitionManager = CSF_AT(void*, globalObject, 0x1FC0);
    void* definition = reinterpret_cast<void*(__fastcall*)(void*, void*, uint32_t)>(
        CSF_AT(void**, definitionManager, 0)[8 / 4])(
            definitionManager, nullptr, self->field_0194);
    auto* effectData = reinterpret_cast<uint32_t*>(
        static_cast<uint8_t*>(definition) + 4);

    globalObject = reinterpret_cast<void*>(GetValue8B0554());
    void* particleManager = CSF_AT(void*, globalObject, 0x1FDC);
    uint32_t* position = GetObjectPositionVector(self, nullptr);
    if (FxParticleManager_FindSceneEntryAtPosition(
            particleManager, nullptr, position, 1) != -1)
    {
        return self->vSlotA0();
    }

    alignas(8) uint8_t vertexBytes[0x28];
    CSF_AT(float, vertexBytes, 0x10) =
        reinterpret_cast<const float&>(self->field_019c) * g_MessageBoxAnimationScale;
    CSF_AT(float, vertexBytes, 0x14) =
        reinterpret_cast<const float&>(self->field_01a0) * g_MessageBoxAnimationScale;
    CSF_AT(float, vertexBytes, 0x18) =
        reinterpret_cast<const float&>(self->field_01a4) * g_MessageBoxAnimationScale;

    position = GetObjectPositionVector(self, nullptr);
    CSF_AT(float, vertexBytes, 0x1C) =
        CSF_AT(float, vertexBytes, 0x10) + CSF_AT(float, position, 0);
    CSF_AT(float, vertexBytes, 0x20) =
        CSF_AT(float, vertexBytes, 0x14) + CSF_AT(float, position, 4);
    CSF_AT(float, vertexBytes, 0x24) =
        CSF_AT(float, vertexBytes, 0x18) + CSF_AT(float, position, 8);

    if (self->field_0050 == 16)
    {
        void* relatedObject = reinterpret_cast<void*>(self->vGetField016c());
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, relatedObject, 0)[0x3C / 4])(
                relatedObject, nullptr, 0);
    }

    alignas(8) uint8_t recordBytes[0x60];
    C_008310f8& record = *reinterpret_cast<C_008310f8*>(recordBytes);
    C_008310f8_Initialize(&record, nullptr, 14);
    record.field_0024 = self->field_0050;

    position = GetObjectPositionVector(self, nullptr);
    record.field_0034 = position[0];
    record.field_0038 = position[1];
    uint32_t directionX = self->field_019c;
    record.field_003c = position[2];
    uint32_t effectId = effectData[104];
    uint32_t directionY = self->field_01a0;
    record.field_0028 = effectId;
    record.field_0050 = self->field_01a4;
    record.field_0040 = reinterpret_cast<uint32_t>(self);
    record.field_0048 = directionX;
    record.field_004c = directionY;
    record.field_0044 = 1;
    record.field_0054 = effectData[72];
    record.field_0058 = effectData[69];

    void* dispatcher = reinterpret_cast<void*>(GetGlobalObjectField184());
    DispatchRecordToGlobalObject(dispatcher, nullptr, &record, 1);

    CSF_AT(float, vertexBytes, 0x0C) = CSF_AT(float, effectData, 0x120);
    CSF_AT(uint32_t, vertexBytes, 0x04) = CSF_AT(uint32_t, vertexBytes, 0x20);
    CSF_AT(uint32_t, vertexBytes, 0x00) = CSF_AT(uint32_t, vertexBytes, 0x1C);
    CSF_AT(uint32_t, vertexBytes, 0x08) = CSF_AT(uint32_t, vertexBytes, 0x24);

    if (self->field_0050 == 15)
    {
        CGuiControl_SubmitVertexRecord(
            reinterpret_cast<CGuiControl*>(g_pCurrentScene),
            nullptr,
            static_cast<int>(effectData[104]),
            reinterpret_cast<uint32_t*>(vertexBytes));
    }

    GetGlobal8B0578Thunk(
        0, reinterpret_cast<int>(vertexBytes), static_cast<int>(effectData[104]));

    int result = GetGlobalState_50D190();
    if (!CSF_AT(uint8_t, reinterpret_cast<void*>(result), 0xD0))
    {
        result = static_cast<int>(self->field_0034);
        if (!result)
            result = self->vSlotA0();
    }

    self->field_0184 = 1;
    return result;
}

HOOK(0x0049D8A0, C_00834050_EmitSceneEffect, "auto");
