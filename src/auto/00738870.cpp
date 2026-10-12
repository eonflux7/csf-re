#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pRwEngineInstance, 0x008B68C0);

using RenderStateSet = int (__cdecl*)(int, int);
using RenderStateGet = void (__cdecl*)(int, int*);
using RenderIndexedPrimitives = void (__cdecl*)(
    uint32_t, uint32_t, uint32_t, uint32_t, uint32_t);

static int __fastcall C_00867358_RenderPrimitives(
    C_00867358* self, void* /*edx*/)
{
    const int vertexCount = static_cast<int>(self->field_000c);
    if (vertexCount <= 0)
        return vertexCount;

    CSF_AT(RenderStateSet, g_pRwEngineInstance, 0x20)(12, 1);
    CSF_AT(RenderStateSet, g_pRwEngineInstance, 0x20)(10, 5);
    CSF_AT(RenderStateSet, g_pRwEngineInstance, 0x20)(14, 0);

    const int* texture = reinterpret_cast<const int*>(self->field_0020);
    const int raster = texture ? *texture : 0;
    CSF_AT(RenderStateSet, g_pRwEngineInstance, 0x20)(1, raster);
    CSF_AT(RenderStateSet, g_pRwEngineInstance, 0x20)(11, 6);
    CSF_AT(RenderStateSet, g_pRwEngineInstance, 0x20)(9, 2);
    CSF_AT(RenderStateSet, g_pRwEngineInstance, 0x20)(6, 0);
    CSF_AT(RenderStateSet, g_pRwEngineInstance, 0x20)(8, 0);
    CSF_AT(RenderStateSet, g_pRwEngineInstance, 0x20)(20, 1);

    int savedAddressMode = 0;
    CSF_AT(RenderStateGet, g_pRwEngineInstance, 0x24)(2, &savedAddressMode);
    CSF_AT(RenderStateSet, g_pRwEngineInstance, 0x20)(2, 3);

    const uint32_t indexCount = self->field_0018;
    if (static_cast<int>(indexCount) > 0)
    {
        CSF_AT(RenderIndexedPrimitives, g_pRwEngineInstance, 0x34)(
            self->field_001c,
            self->field_0004,
            self->field_000c,
            self->field_0010,
            indexCount);
    }
    else
    {
        void* renderer = reinterpret_cast<void*>(self->field_0024);
        ((void (__fastcall*)(void*, void*, uint32_t, uint32_t, uint32_t))
            CSF_AT(void**, renderer, 0)[0x24 / 4])(
                renderer, nullptr,
                self->field_001c,
                self->field_0004,
                self->field_000c);
    }

    return CSF_AT(RenderStateSet, g_pRwEngineInstance, 0x20)(
        2, savedAddressMode);
}

HOOK(0x00738870, C_00867358_RenderPrimitives, "auto");
