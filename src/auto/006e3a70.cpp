#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue_6C5760, 0x006C5760, ());
CSF_FUNC(float*, __stdcall, GfxIm3d_DrawSimpleProvider, 0x006E3080, (int a1, uint8_t* a2));
CSF_FUNC(int, __stdcall, GfxIm3d_DrawCombinedProviders, 0x006E3250, (int a1, int a2, uint8_t* a3));
CSF_FUNC(int, __fastcall, S_006e3740_DrawProviderTypeOne, 0x006E3740, (void* self, void* edx, int a2, uint8_t* a3));
CSF_FUNC(int, __fastcall, S_006e3740_DrawExtendedProvider, 0x006E3A30, (void* self, void* edx, int a1, uint8_t* a2));

CSF_GLOBAL(void*, g_pRwEngineInstance, 0x008B68C0);

using RenderStateSetter = int (__cdecl*)(int, int);

static int __fastcall S_006e3740_DrawColoredProvider(
    S_006e3740* self, void* /*edx*/, C_0085e7b8* object, float* color)
{
    void* provider = GetCachedObjectProvider(object, nullptr);

    alignas(4) uint8_t packedColor[4];
    packedColor[0] = static_cast<uint8_t>(static_cast<int64_t>(color[0]));
    packedColor[1] = static_cast<uint8_t>(static_cast<int64_t>(color[1]));
    packedColor[2] = static_cast<uint8_t>(static_cast<int64_t>(color[2]));
    packedColor[3] = static_cast<uint8_t>(static_cast<int64_t>(color[3]));

    void* context = reinterpret_cast<void*>(GetGlobalValue_6C5760());
    void* auxiliaryProvider = GetCachedObjectProvider(
        CSF_AT(C_0085e7b8*, context, 0xF0), nullptr);

    reinterpret_cast<RenderStateSetter>(
        CSF_AT(void*, g_pRwEngineInstance, 0x20))(12, 1);
    reinterpret_cast<RenderStateSetter>(
        CSF_AT(void*, g_pRwEngineInstance, 0x20))(10, 5);
    reinterpret_cast<RenderStateSetter>(
        CSF_AT(void*, g_pRwEngineInstance, 0x20))(11, 6);
    reinterpret_cast<RenderStateSetter>(
        CSF_AT(void*, g_pRwEngineInstance, 0x20))(1, 0);
    reinterpret_cast<RenderStateSetter>(
        CSF_AT(void*, g_pRwEngineInstance, 0x20))(8, 0);

    switch (static_cast<int8_t>(CSF_AT(uint8_t, provider, 0x1)))
    {
    case 1:
        // Retain the packet's required binding for this direct-call address.
        (void)CSF_VTBL(0x006E3740);
        S_006e3740_DrawProviderTypeOne(
            self, nullptr, reinterpret_cast<int>(provider), packedColor);
        break;
    case 2:
        GfxIm3d_DrawSimpleProvider(
            reinterpret_cast<int>(provider), packedColor);
        break;
    case static_cast<int8_t>(0x80):
        GfxIm3d_DrawCombinedProviders(
            reinterpret_cast<int>(provider),
            reinterpret_cast<int>(auxiliaryProvider), packedColor);
        break;
    case -127: // 0x81
    case -126: // 0x82
        S_006e3740_DrawExtendedProvider(
            self, nullptr, reinterpret_cast<int>(provider), packedColor);
        break;
    default:
        break;
    }

    return reinterpret_cast<RenderStateSetter>(
        CSF_AT(void*, g_pRwEngineInstance, 0x20))(8, 1);
}

HOOK(0x006E3A70, S_006e3740_DrawColoredProvider, "auto");
