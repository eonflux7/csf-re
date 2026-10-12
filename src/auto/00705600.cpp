#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, GetSceneRegionAt, 0x004FC130, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, GfxSkin_ShouldSkipProviderAttachment, 0x0068ECA0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue_6C5760, 0x006C5760, ());
CSF_FUNC(int, __stdcall, GfxSkin_ForwardToRwEngineSlot20, 0x006C7B40, (int a1, int a2));
CSF_FUNC(int, __stdcall, GfxSkin_ForwardToRwEngineSlot24, 0x006C7B60, (int a1, int a2));
CSF_FUNC(char, __fastcall, GlobalHud_Commit_6C7C30, 0x006C7C30, (C_0085e7b8* self, void*));
CSF_FUNC(int, __fastcall, C_0085e7b8_AttachWidget, 0x006C8420, (C_0085e7b8* self, void* , C_0085e7b8* widget));
CSF_FUNC(char, __fastcall, S_00705300_RenderPass, 0x00705300, (void* self, void* edx, float a2, int a3));

CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);
CSF_GLOBAL(uint32_t, g_FirstVertexColor, 0x008BBE80);
CSF_GLOBAL(uint32_t, g_SecondVertexColor, 0x008BBEA4);
CSF_GLOBAL(uint32_t, g_ThirdVertexColor, 0x008BBEC8);

static char __fastcall S_00705300_RenderBlendedPasses(
    S_00705300* self, void* /*edx*/)
{
    // The packet also identifies this direct callee address as a vtable.
    (void)CSF_VTBL(0x00705300);

    if (!self->field_0010)
        return 0;

    auto* provider = CSF_AT(
        C_0085e7b8*, reinterpret_cast<void*>(GetGlobalValue_6C5760()), 0x4);

    void* region = reinterpret_cast<void*>(
        GetSceneRegionAt(provider, nullptr, 7));
    const char skipAttachment =
        GfxSkin_ShouldSkipProviderAttachment(region, nullptr);

    if (!skipAttachment)
    {
        auto* widget = reinterpret_cast<C_0085e7b8*>(
            GetSceneRegionAt(provider, nullptr, 7));
        C_0085e7b8_AttachWidget(provider, nullptr, widget);
    }

    int savedState = 0;
    GfxSkin_ForwardToRwEngineSlot24(
        14, reinterpret_cast<int>(&savedState));
    GfxSkin_ForwardToRwEngineSlot20(14, 0);
    GfxSkin_ForwardToRwEngineSlot20(
        1, CSF_AT(int, reinterpret_cast<void*>(self->field_0010), 0));

    float weight = self->field_0038;
    g_FirstVertexColor =
        (uint32_t(self->field_0033) << 24) |
        (uint32_t(self->field_0030) << 16) |
        (uint32_t(self->field_0031) << 8) |
        uint32_t(self->field_0032);
    g_SecondVertexColor =
        (uint32_t(self->field_0033) << 24) |
        (uint32_t(self->field_0030) << 16) |
        (uint32_t(self->field_0031) << 8) |
        uint32_t(self->field_0032);
    g_ThirdVertexColor =
        (uint32_t(self->field_0033) << 24) |
        (uint32_t(self->field_0030) << 16) |
        (uint32_t(self->field_0031) << 8) |
        uint32_t(self->field_0032);
    S_00705300_RenderPass(self, nullptr, weight, 0);

    weight = g_TransitionOne - self->field_0038;
    g_FirstVertexColor =
        (uint32_t(self->field_0037) << 24) |
        (uint32_t(self->field_0034) << 16) |
        (uint32_t(self->field_0035) << 8) |
        uint32_t(self->field_0036);
    g_SecondVertexColor =
        (uint32_t(self->field_0037) << 24) |
        (uint32_t(self->field_0034) << 16) |
        (uint32_t(self->field_0035) << 8) |
        uint32_t(self->field_0036);
    g_ThirdVertexColor =
        (uint32_t(self->field_0037) << 24) |
        (uint32_t(self->field_0034) << 16) |
        (uint32_t(self->field_0035) << 8) |
        uint32_t(self->field_0036);
    S_00705300_RenderPass(self, nullptr, weight, 1);

    char result = static_cast<char>(
        GfxSkin_ForwardToRwEngineSlot20(14, savedState));
    if (!skipAttachment)
        result = GlobalHud_Commit_6C7C30(provider, nullptr);

    return result;
}

HOOK(0x00705600, S_00705300_RenderBlendedPasses, "auto");
