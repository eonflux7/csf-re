#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, LookupObjectById, 0x00652160, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_6C5760, 0x006C5760, ());
CSF_FUNC(char, __fastcall, GlobalHud_Commit_6C7C30, 0x006C7C30, (C_0085e7b8* self, void*));
CSF_FUNC(int, __fastcall, GfxSkin_AppendObjectRecord, 0x006C8390, (void* self, void* , int objIndex));
CSF_FUNC(void, __fastcall, GfxCameraProjection_ApplyTransform, 0x006DB0D0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GfxCameraProjection_RenderObject, 0x006F5F00, (void* self, void* edx));

CSF_GLOBAL(void*, g_pRwEngineInstance, 0x008B68C0);

static char __fastcall GfxCameraProjection_RenderObjects(void* self, void* /*edx*/)
{
    const char enabled = CSF_AT(char, self, 0x4);
    if (!enabled)
        return enabled;
    if (!CSF_AT(uint32_t, self, 0x10))
        return 0;

    GfxCameraProjection_ApplyTransform(
        reinterpret_cast<uint8_t*>(self) + 0x58, nullptr);
    GfxCameraProjection_ApplyTransform(
        reinterpret_cast<uint8_t*>(self) + 0x18, nullptr);

    void* hudGlobal = reinterpret_cast<void*>(GetGlobalValue_6C5760());
    GfxSkin_AppendObjectRecord(CSF_AT(void*, hudGlobal, 0x4), nullptr, 10);

    alignas(8) uint8_t renderStateBytes[0x8];
    reinterpret_cast<void(__cdecl*)(int, void*)>(
        reinterpret_cast<void**>(g_pRwEngineInstance)[0x24 / 4])(
            15, renderStateBytes);
    reinterpret_cast<void(__cdecl*)(int, uint32_t)>(
        reinterpret_cast<void**>(g_pRwEngineInstance)[0x20 / 4])(15, 0);

    void* object = LookupObjectById(reinterpret_cast<void*>(GetGlobalInt()), nullptr);
    void* device = CSF_AT(void*, object, 0x5C);
    CSF_AT(int32_t, renderStateBytes, 0x4) = -1185827049;
    reinterpret_cast<void(__stdcall*)(void*, int, int32_t)>(
        CSF_AT(void**, device, 0)[0xE4 / 4])(
            device, 195, CSF_AT(int32_t, renderStateBytes, 0x4));

    reinterpret_cast<void(__cdecl*)(int, uint32_t)>(
        reinterpret_cast<void**>(g_pRwEngineInstance)[0x20 / 4])(10, 2);
    reinterpret_cast<void(__cdecl*)(int, uint32_t)>(
        reinterpret_cast<void**>(g_pRwEngineInstance)[0x20 / 4])(11, 2);
    reinterpret_cast<void(__cdecl*)(int, uint32_t)>(
        reinterpret_cast<void**>(g_pRwEngineInstance)[0x20 / 4])(2, 3);

    for (uint32_t index = 0; index < CSF_AT(uint32_t, self, 0x10); ++index)
    {
        GfxCameraProjection_RenderObject(
            CSF_AT(void**, self, 0x8)[index], nullptr);
    }

    object = LookupObjectById(reinterpret_cast<void*>(GetGlobalInt()), nullptr);
    device = CSF_AT(void*, object, 0x5C);
    reinterpret_cast<void(__stdcall*)(void*, int, int32_t)>(
        CSF_AT(void**, device, 0)[0xE4 / 4])(device, 195, 0);

    reinterpret_cast<void(__cdecl*)(int, uint32_t)>(
        reinterpret_cast<void**>(g_pRwEngineInstance)[0x20 / 4])(
            15, CSF_AT(uint32_t, renderStateBytes, 0));

    hudGlobal = reinterpret_cast<void*>(GetGlobalValue_6C5760());
    return GlobalHud_Commit_6C7C30(
        CSF_AT(C_0085e7b8*, hudGlobal, 0x4), nullptr);
}

HOOK(0x006F61B0, GfxCameraProjection_RenderObjects, "auto");
