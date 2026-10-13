#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RpPVSSetViewPosition, 0x005860C0, (int a1, int* a2, int a3));
CSF_FUNC(int, __cdecl, RpPVSWorldUnhook, 0x005861F0, (int a1));
CSF_FUNC(int, __cdecl, RpPrtStdWorldSetRenderCallBack, 0x005865A0, (int a1));
CSF_FUNC(int, __cdecl, RpWorldRender, 0x0058CBB0, (int a1));
CSF_FUNC(int, __cdecl, RpWorldSetSectorRenderCallBack, 0x0058CC60, (int a1, int (__cdecl *a2)(int a1)));
CSF_FUNC(int, __cdecl, RpWorldGetSectorRenderCallBack, 0x0058CC80, (int a1));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(int, __fastcall, Environment_RenderWorldStage, 0x006BC310, (S_006bc310* self, void*));
CSF_FUNC(int, __cdecl, GetGlobalValue_6C5760, 0x006C5760, ());
CSF_FUNC(int, __stdcall, GfxSkin_ForwardToRwEngineSlot20, 0x006C7B40, (int a1, int a2));
CSF_FUNC(char, __fastcall, GlobalHud_Commit_6C7C30, 0x006C7C30, (C_0085e7b8* self, void*));
CSF_FUNC(int, __fastcall, GfxSkin_AppendObjectRecord, 0x006C8390, (void* self, void* , int objIndex));

static char __fastcall S_006bc310_RenderWorldWithVisibility(
    S_006bc310* self, void* /*edx*/)
{
    // The packet annotates the render-stage function address as a vtable.
    (void)CSF_VTBL(0x006BC310);

    auto* globals = reinterpret_cast<void*>(GetGlobalValue_6C5760());
    auto* camera = CSF_AT(C_0085e7b8*, globals, 0xF0);
    GetCachedObjectProvider(camera, nullptr);
    void* provider = camera->vGetCachedObjectProvider();
    int* sourcePosition =
        reinterpret_cast<int* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, provider, 0)[0x10 / 4])(provider, nullptr);

    alignas(8) uint8_t positionBytes[0xC];
    int* viewPosition = reinterpret_cast<int*>(positionBytes);
    viewPosition[0] = sourcePosition[0];
    viewPosition[1] = sourcePosition[1];
    viewPosition[2] = sourcePosition[2];

    globals = reinterpret_cast<void*>(GetGlobalValue_6C5760());
    GfxSkin_AppendObjectRecord(CSF_AT(void*, globals, 0x4), nullptr, 2);

    if (self->field_0090)
    {
        // This argument remains on the original stack during the PVS call,
        // where the supplied three-argument binding observes it again.
        int callbackWorld = static_cast<int>(self->field_0004);
        RpPrtStdWorldSetRenderCallBack(callbackWorld);
        RpPVSSetViewPosition(
            static_cast<int>(self->field_0004), viewPosition, callbackWorld);
        Environment_RenderWorldStage(self, nullptr);
        RpPVSWorldUnhook(static_cast<int>(self->field_0004));
    }
    else
    {
        Environment_RenderWorldStage(self, nullptr);
    }

    globals = reinterpret_cast<void*>(GetGlobalValue_6C5760());
    char result = GlobalHud_Commit_6C7C30(
        CSF_AT(C_0085e7b8*, globals, 0x4), nullptr);

    if ((self->field_008c & 4) != 0)
    {
        int savedCallback =
            RpWorldGetSectorRenderCallBack(static_cast<int>(self->field_0004));
        RpWorldSetSectorRenderCallBack(
            static_cast<int>(self->field_0004),
            reinterpret_cast<int (__cdecl*)(int)>(CSF_ADDR_FN(0x006EE540)));

        GetGlobalValue_6C5760();
        GfxSkin_ForwardToRwEngineSlot20(1, 0);
        RpWorldRender(static_cast<int>(self->field_0004));

        return static_cast<char>(RpWorldSetSectorRenderCallBack(
            static_cast<int>(self->field_0004),
            reinterpret_cast<int (__cdecl*)(int)>(savedCallback)));
    }

    return result;
}

HOOK(0x006BCC20, S_006bc310_RenderWorldWithVisibility, "auto");
