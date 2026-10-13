#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(void*, g_pRwEngineInstance, 0x008B68C0);

CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(void*, __fastcall, MenuControls_ResolveObject, 0x006B1390, (void* self, void* edx));

using RenderStateFn = void (__cdecl*)(int, uint32_t);

static int __fastcall C_00862a80_DrawQuad(
    C_00862a80* self, void* /*edx*/,
    int rgb, char alpha, int width, int height, int rasterAddress,
    uint8_t blendEnabled, float textureX, float textureY,
    float textureWidth, float textureHeight)
{
    const float x = self->field_0034;
    const float y = self->field_0038;
    void* raster = reinterpret_cast<void*>(rasterAddress);

    const float u0 =
        (rasterAddress ? textureX / static_cast<float>(CSF_AT(int, raster, 0x0C))
                       : g_fZero) + self->field_003c;
    const float v0 =
        (rasterAddress ? textureY / static_cast<float>(CSF_AT(int, raster, 0x10))
                       : g_fZero) + self->field_003c;
    const float u1 = textureWidth * self->field_0044 + u0;
    const float v1 = textureHeight * self->field_0048 + v0;

    CSF_AT(float, reinterpret_cast<void*>(self->field_0018), 0x00) = x;
    CSF_AT(float, reinterpret_cast<void*>(self->field_0018), 0x04) = y;
    CSF_AT(float, reinterpret_cast<void*>(self->field_0018), 0x14) = u0;
    CSF_AT(float, reinterpret_cast<void*>(self->field_0018), 0x18) = v0;
    CSF_AT(float, reinterpret_cast<void*>(self->field_0018), 0x1C) = x;

    const float bottom = static_cast<float>(height) + y;
    CSF_AT(float, reinterpret_cast<void*>(self->field_0018), 0x20) = bottom;
    CSF_AT(float, reinterpret_cast<void*>(self->field_0018), 0x30) = u0;

    const float right = static_cast<float>(width) + x;
    CSF_AT(float, reinterpret_cast<void*>(self->field_0018), 0x34) = v1;
    CSF_AT(float, reinterpret_cast<void*>(self->field_0018), 0x38) = right;
    CSF_AT(float, reinterpret_cast<void*>(self->field_0018), 0x3C) = y;
    CSF_AT(float, reinterpret_cast<void*>(self->field_0018), 0x4C) = u1;
    CSF_AT(float, reinterpret_cast<void*>(self->field_0018), 0x50) = v0;
    CSF_AT(float, reinterpret_cast<void*>(self->field_0018), 0x54) = right;
    CSF_AT(float, reinterpret_cast<void*>(self->field_0018), 0x58) = bottom;
    CSF_AT(float, reinterpret_cast<void*>(self->field_0018), 0x68) = u1;
    CSF_AT(float, reinterpret_cast<void*>(self->field_0018), 0x6C) = v1;

    const uint32_t color =
        (static_cast<uint32_t>(rgb) & 0x00FFFFFF) |
        (static_cast<uint32_t>(static_cast<uint8_t>(alpha)) << 24);

    CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0018), 0x10) = color;
    CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0018), 0x0C) = 1065353216;
    CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0018), 0x08) = 0;
    CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0018), 0x2C) = color;
    CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0018), 0x28) = 1065353216;
    CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0018), 0x24) = 0;
    CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0018), 0x48) = color;
    CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0018), 0x44) = 1065353216;
    CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0018), 0x40) = 0;
    CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0018), 0x64) = color;
    CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0018), 0x60) = 1065353216;
    CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0018), 0x5C) = 0;

    CSF_AT(RenderStateFn, g_pRwEngineInstance, 0x20)(12, blendEnabled);
    CSF_AT(RenderStateFn, g_pRwEngineInstance, 0x20)(6, 0);
    CSF_AT(RenderStateFn, g_pRwEngineInstance, 0x20)(8, 0);
    CSF_AT(RenderStateFn, g_pRwEngineInstance, 0x20)(1, rasterAddress);
    CSF_AT(RenderStateFn, g_pRwEngineInstance, 0x20)(7, 1);

    if (self->field_0030)
        CSF_AT(RenderStateFn, g_pRwEngineInstance, 0x20)(11, 2);
    else
        CSF_AT(RenderStateFn, g_pRwEngineInstance, 0x20)(11, 6);

    CSF_AT(RenderStateFn, g_pRwEngineInstance, 0x20)(10, 5);
    CSF_AT(RenderStateFn, g_pRwEngineInstance, 0x20)(14, 0);
    CSF_AT(RenderStateFn, g_pRwEngineInstance, 0x20)(20, 1);

    if (self->field_004c)
        CSF_AT(RenderStateFn, g_pRwEngineInstance, 0x20)(2, 3);
    else
        CSF_AT(RenderStateFn, g_pRwEngineInstance, 0x20)(2, 1);

    CSF_AT(RenderStateFn, g_pRwEngineInstance, 0x20)(9, 4);

    void* globalObject = reinterpret_cast<void*>(GetGlobalInt());
    void* renderer = MenuControls_ResolveObject(globalObject, nullptr);
    return reinterpret_cast<int (__fastcall*)(void*, void*, int, void*, int)>(
        CSF_AT(void**, renderer, 0)[0x24 / 4])(renderer, nullptr, 4, reinterpret_cast<void*>(self->field_0018), 4);
}

HOOK(0x00708AB0, C_00862a80_DrawQuad, "auto");
