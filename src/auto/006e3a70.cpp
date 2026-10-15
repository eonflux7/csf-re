#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue_6C5760, 0x006C5760, ());
CSF_FUNC(float*, __stdcall, GfxIm3D_TransformLightWorldBounds, 0x006E3080, (int light, std::uint8_t* color));
CSF_FUNC(int, __stdcall, GfxIm3D_DrawCircleWithDiameters, 0x006E3250, (int centerObject, int orientationObject, unsigned char* color));
CSF_FUNC(int, __fastcall, S_006e3740_RenderColoredCylinder, 0x006E3740, (S_006e3740* , void* , int shape, std::uint8_t* color));
CSF_FUNC(int, __fastcall, Im3dLight_DrawConeWithLtm, 0x006E3A30, (void* self, void* edx, int light, uint8_t* a2));

CSF_GLOBAL(void*, g_pRwEngineInstance, 0x008B68C0);

static int __fastcall S_006e3740_DrawColoredProvider(
    S_006e3740* self, void* /*edx*/, C_0085e7b8* object, float* components)
{
    void* provider = GetCachedObjectProvider(object, nullptr);

    std::uint8_t color[4];
    color[0] = static_cast<std::uint8_t>(static_cast<std::int64_t>(components[0]));
    color[1] = static_cast<std::uint8_t>(static_cast<std::int64_t>(components[1]));
    color[2] = static_cast<std::uint8_t>(static_cast<std::int64_t>(components[2]));
    color[3] = static_cast<std::uint8_t>(static_cast<std::int64_t>(components[3]));

    int globalObject = GetGlobalValue_6C5760();
    void* orientationProvider = GetCachedObjectProvider(
        CSF_AT(C_0085e7b8*, reinterpret_cast<void*>(globalObject), 0xF0),
        nullptr);

    using SetRenderState = int (__cdecl*)(int, int);
    reinterpret_cast<SetRenderState>(
        CSF_AT(void*, g_pRwEngineInstance, 0x20))(12, 1);
    reinterpret_cast<SetRenderState>(
        CSF_AT(void*, g_pRwEngineInstance, 0x20))(10, 5);
    reinterpret_cast<SetRenderState>(
        CSF_AT(void*, g_pRwEngineInstance, 0x20))(11, 6);
    reinterpret_cast<SetRenderState>(
        CSF_AT(void*, g_pRwEngineInstance, 0x20))(1, 0);
    reinterpret_cast<SetRenderState>(
        CSF_AT(void*, g_pRwEngineInstance, 0x20))(8, 0);

    (void)CSF_VTBL(0x006E3740);

    switch (CSF_AT(std::int8_t, provider, 0x1))
    {
    case 1:
        S_006e3740_RenderColoredCylinder(
            self, nullptr, reinterpret_cast<int>(provider), color);
        break;
    case 2:
        GfxIm3D_TransformLightWorldBounds(
            reinterpret_cast<int>(provider), color);
        break;
    case -128:
        GfxIm3D_DrawCircleWithDiameters(
            reinterpret_cast<int>(provider),
            reinterpret_cast<int>(orientationProvider), color);
        break;
    case -127:
    case -126:
        Im3dLight_DrawConeWithLtm(
            self, nullptr, reinterpret_cast<int>(provider), color);
        break;
    default:
        break;
    }

    return reinterpret_cast<SetRenderState>(
        CSF_AT(void*, g_pRwEngineInstance, 0x20))(8, 1);
}

HOOK(0x006E3A70, S_006e3740_DrawColoredProvider, "auto");
