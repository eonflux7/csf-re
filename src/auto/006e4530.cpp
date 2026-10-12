#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pRwEngineInstance, 0x008B68C0);

CSF_FUNC(int, __cdecl, RwCameraEndUpdate, 0x00628E40, (int camera));
CSF_FUNC(int, __fastcall, C_00867358_ClearFields, 0x006C3620, (C_00867358* self, void*));
CSF_FUNC(int, __fastcall, C_00867358_AppendVertex, 0x006C36A0,
         (void* self, void* edx, uint32_t* a2, uint8_t* a3, uint32_t* a4));
CSF_FUNC(int, __fastcall, C_00867358_SubmitGeometry, 0x006C4330,
         (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue_6C5760, 0x006C5760, ());
CSF_FUNC(char, __fastcall, GlobalHud_Commit_6C7C30, 0x006C7C30, (C_0085e7b8* self, void*));
CSF_FUNC(int, __fastcall, GfxSkin_AppendObjectRecord, 0x006C8390, (void* self, void* , int objIndex));
CSF_FUNC(int, __fastcall, GfxObjectInstances_GetVertexRecord, 0x006D9D40,
         (void* self, void* edx));
CSF_FUNC(int, __fastcall, GfxObjectInstances_SubmitAtomicGeometry, 0x00701DE0,
         (void* self, void* edx, void* a2));

using RenderStateGet = void (__cdecl*)(int state, int* value);
using RenderStateSet = void (__cdecl*)(int state, uint32_t value);

static int __stdcall GfxObjectInstances_RenderGeometryAndEndCameraUpdate(
    int camera,
    int instanceAddress,
    S_00701d40* geometry,
    int mode,
    int /*unused*/,
    uint32_t* vertexData)
{
    int savedState = 0;
    CSF_AT(RenderStateGet, g_pRwEngineInstance, 0x24)(15, &savedState);
    CSF_AT(RenderStateSet, g_pRwEngineInstance, 0x20)(15, 0);

    void* globalState = reinterpret_cast<void*>(GetGlobalValue_6C5760());
    GfxSkin_AppendObjectRecord(CSF_AT(void*, globalState, 0x4), nullptr, 10);

    CSF_AT(RenderStateSet, g_pRwEngineInstance, 0x20)(2, 3);

    void* instance = reinterpret_cast<void*>(instanceAddress);
    CSF_AT(RenderStateSet, g_pRwEngineInstance, 0x20)(
        1, CSF_AT(uint32_t, instance, 0x4));

    if (mode == 3)
    {
        uint8_t color[4] = {0x7F, 0x7F, 0x7F, 0};
        auto* record = reinterpret_cast<C_00867358*>(
            GfxObjectInstances_GetVertexRecord(instance, nullptr));
        C_00867358_ClearFields(record, nullptr);

        alignas(8) uint8_t coordinateBytes[0x8];
        auto* coordinates = reinterpret_cast<uint32_t*>(coordinateBytes);

        coordinates[0] = vertexData[0];
        coordinates[1] = vertexData[1];
        C_00867358_AppendVertex(
            record, nullptr, vertexData + 12, color, coordinates);

        coordinates[0] = vertexData[3];
        coordinates[1] = vertexData[4];
        C_00867358_AppendVertex(
            record, nullptr, vertexData + 15, color, coordinates);

        coordinates[0] = vertexData[6];
        coordinates[1] = vertexData[7];
        C_00867358_AppendVertex(
            record, nullptr, vertexData + 18, color, coordinates);

        coordinates[0] = vertexData[9];
        coordinates[1] = vertexData[10];
        C_00867358_AppendVertex(
            record, nullptr, vertexData + 21, color, coordinates);

        record->field_001c = 4;
        C_00867358_SubmitGeometry(record, nullptr);
    }
    else
    {
        GfxObjectInstances_SubmitAtomicGeometry(
            geometry, nullptr, reinterpret_cast<uint8_t*>(instance) + 132);
    }

    globalState = reinterpret_cast<void*>(GetGlobalValue_6C5760());
    GlobalHud_Commit_6C7C30(
        CSF_AT(C_0085e7b8*, globalState, 0x4), nullptr);

    CSF_AT(RenderStateSet, g_pRwEngineInstance, 0x20)(
        15, static_cast<uint32_t>(savedState));
    return RwCameraEndUpdate(camera);
}

HOOK(0x006E4530, GfxObjectInstances_RenderGeometryAndEndCameraUpdate, "auto");
