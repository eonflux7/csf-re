#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(unsigned int, __cdecl, Hud_WriteNormalizedScreenSize, 0x0042DE60, (float* out));
CSF_FUNC(uint32_t, __fastcall, GetSceneMenuContext, 0x0044FC60, (void* self, void* edx));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(std::uint32_t*, __cdecl, RwMatrixCreate, 0x00624D40, ());
CSF_FUNC(int, __fastcall, GfxMatrix_Scale, 0x006ADB60, (void* self, void* edx, int a2, int a3, int a4));
CSF_FUNC(int, __fastcall, GfxMatrix_Translate, 0x006ADBB0, (void* self, void* edx, int a2, int a3, int a4));

CSF_GLOBAL(float, g_MinimapZoomWide, 0x00830E4C);
CSF_GLOBAL(float, g_MinimapZoomNormal, 0x00830E48);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static int FloatBits(float f)
{
    int bits;
    std::memcpy(&bits, &f, sizeof(bits));
    return bits;
}

static int* __fastcall Minimap_ApplyRadarTransform(void* self, void* /*edx*/)
{
    int* result = CSF_AT(int*, self, 0xC);
    void* obj = *reinterpret_cast<void**>(result);
    if (obj == nullptr)
        return result;

    void** vtbl = CSF_AT(void**, obj, 0);

    float* pos = ((float*(__fastcall*)(void*, void*))vtbl[0x30 / 4])(obj, nullptr);
    float posX = pos[0];
    float posY = pos[1];

    alignas(8) uint8_t frame[12];
    // frame+0: matrix pointer, frame+4: owned flag
    CSF_AT(std::uint32_t*, frame, 0) = RwMatrixCreate();
    CSF_AT(uint8_t, frame, 4) = 1;

    std::uint32_t* m = CSF_AT(std::uint32_t*, frame, 0);
    m[10] = 0x3F800000;
    m[5] = 0x3F800000;
    m[0] = 0x3F800000;
    m[4] = 0;
    m[2] = 0;
    m[1] = 0;
    m[9] = 0;
    m[8] = 0;
    m[6] = 0;
    m[14] = 0;
    m[13] = 0;
    m[12] = 0;
    m[3] |= 0x20003;

    ((void(__fastcall*)(void*, void*, void*))vtbl[0x60 / 4])(obj, nullptr, frame);

    float scr[2];
    scr[0] = 0.0f;
    scr[1] = 0.0f;
    Hud_WriteNormalizedScreenSize(scr);

    GfxMatrix_Translate(frame, nullptr, FloatBits(-posX), FloatBits(-posY), 0);
    GfxMatrix_Scale(frame, nullptr, FloatBits(scr[0]), FloatBits(scr[1]), 0);

    uint32_t ctx1 = GetSceneMenuContext(g_pCurrentScene, nullptr);
    uint32_t zoomInfo = CSF_AT(uint32_t, reinterpret_cast<void*>(ctx1), 0x3C);
    uint32_t ctx2 = GetSceneMenuContext(g_pCurrentScene, nullptr);

    float scale;
    if ((CSF_AT(uint32_t, reinterpret_cast<void*>(ctx2), 0x2E0) & 0x40000) != 0)
        scale = g_MinimapZoomWide / CSF_AT(float, reinterpret_cast<void*>(zoomInfo), 0x38);
    else
        scale = g_MinimapZoomNormal / CSF_AT(float, reinterpret_cast<void*>(zoomInfo), 0x34);

    GfxMatrix_Scale(frame, nullptr, FloatBits(scale), FloatBits(scale), 0);

    float tx = scr[0] * posX;
    float ty = scr[1] * posY;
    GfxMatrix_Translate(frame, nullptr, FloatBits(tx), FloatBits(ty), 0);

    ((void(__fastcall*)(void*, void*, void*))vtbl[0x60 / 4])(obj, nullptr, frame);

    result = ((int*(__fastcall*)(void*, void*))vtbl[8 / 4])(obj, nullptr);

    if (CSF_AT(uint8_t, frame, 4) != 0)
        return reinterpret_cast<int*>(static_cast<intptr_t>(RwMatrixDestroy(CSF_AT(std::uint32_t*, frame, 0))));

    return result;
}

HOOK(0x0043C9A0, Minimap_ApplyRadarTransform, "auto");
