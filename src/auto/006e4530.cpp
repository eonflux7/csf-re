#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pRwEngineInstance, 0x008B68C0);

CSF_FUNC(int, __cdecl, RwCameraEndUpdate, 0x00628E40, (void* camera));
CSF_FUNC(int, __fastcall, C_00867358_ClearFields, 0x006C3620, (C_00867358* self, void*));
CSF_FUNC(int, __fastcall, GfxParticleSystemDefs_AppendTriangleVertex, 0x006C36A0, (void* self, void* , uint32_t* position, uint8_t* color, uint32_t* uv));
CSF_FUNC(int, __fastcall, ParticleInner_Update, 0x006C4330, (void* self, void*));
CSF_FUNC(int, __cdecl, GetGlobalValue_6C5760, 0x006C5760, ());
CSF_FUNC(char, __fastcall, GlobalHud_Commit_6C7C30, 0x006C7C30, (C_0085e7b8* self, void*));
CSF_FUNC(int, __fastcall, GfxSkin_AppendObjectRecord, 0x006C8390, (void* self, void* , int objIndex));
CSF_FUNC(int, __fastcall, GfxObjectInstances_GetVertexRecord, 0x006D9D40, (void* self, void*));
CSF_FUNC(int, __fastcall, ParticleEmitter_ReleaseOwnedInstances, 0x00701DE0, (S_00701d40* self, void* , S_007013b0* a2));

static int __stdcall GfxObjectInstances_RenderAtomicGeometry(
    void* camera,
    void* geometry,
    S_00701d40* emitter,
    int mode,
    int /*unused*/,
    uint32_t* vertexData)
{
    uint32_t savedRenderState;
    // 006E4540/41 push the output pointer and state 15; 006E4553
    // cleans BOTH callbacks' four stack words. ECX is not a callback argument.
    reinterpret_cast<void (__cdecl**)(int, uint32_t*)>(
        g_pRwEngineInstance)[9](15, &savedRenderState);

    reinterpret_cast<void (__cdecl**)(int, uint32_t)>(
        g_pRwEngineInstance)[8](15, 0);

    void* context = reinterpret_cast<void*>(GetGlobalValue_6C5760());
    GfxSkin_AppendObjectRecord(CSF_AT(void*, context, 0x4), nullptr, 10);

    reinterpret_cast<void (__cdecl**)(int, uint32_t)>(
        g_pRwEngineInstance)[8](2, 3);
    reinterpret_cast<void (__cdecl**)(int, uint32_t)>(
        g_pRwEngineInstance)[8](1, CSF_AT(uint32_t, geometry, 0x4));

    if (mode == 3)
    {
        uint32_t color = 8355711;
        auto* record = reinterpret_cast<C_00867358*>(
            GfxObjectInstances_GetVertexRecord(geometry, nullptr));
        C_00867358_ClearFields(record, nullptr);

        alignas(8) uint8_t uvBytes[0x8];
        auto* uv = reinterpret_cast<uint32_t*>(uvBytes);

        uv[0] = vertexData[0];
        uv[1] = vertexData[1];
        GfxParticleSystemDefs_AppendTriangleVertex(
            record, nullptr, vertexData + 12,
            reinterpret_cast<uint8_t*>(&color), uv);

        uv[0] = vertexData[3];
        uv[1] = vertexData[4];
        GfxParticleSystemDefs_AppendTriangleVertex(
            record, nullptr, vertexData + 15,
            reinterpret_cast<uint8_t*>(&color), uv);

        uv[0] = vertexData[6];
        uv[1] = vertexData[7];
        GfxParticleSystemDefs_AppendTriangleVertex(
            record, nullptr, vertexData + 18,
            reinterpret_cast<uint8_t*>(&color), uv);

        uv[0] = vertexData[9];
        uv[1] = vertexData[10];
        GfxParticleSystemDefs_AppendTriangleVertex(
            record, nullptr, vertexData + 21,
            reinterpret_cast<uint8_t*>(&color), uv);

        record->field_001c = 4;
        ParticleInner_Update(record, nullptr);
    }
    else
    {
        ParticleEmitter_ReleaseOwnedInstances(
            emitter, nullptr,
            reinterpret_cast<S_007013b0*>(
                reinterpret_cast<uint8_t*>(geometry) + 0x84));
    }

    context = reinterpret_cast<void*>(GetGlobalValue_6C5760());
    GlobalHud_Commit_6C7C30(
        CSF_AT(C_0085e7b8*, context, 0x4), nullptr);

    reinterpret_cast<void (__cdecl**)(int, uint32_t)>(
        g_pRwEngineInstance)[8](15, savedRenderState);

    return RwCameraEndUpdate(camera);
}

HOOK(0x006E4530, GfxObjectInstances_RenderAtomicGeometry, "auto");
