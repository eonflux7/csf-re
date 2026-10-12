#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pRwEngineInstance, 0x008B68C0);
CSF_FUNC(char, __fastcall, C_00867260_UpdateRenderGeometry, 0x00737C90,
         (void* self, void* edx));

using RenderStateSetter = void (__cdecl*)(int state, uint32_t value);

static int __fastcall C_00867260_Render(C_00867260* self, void* /*edx*/)
{
    if (CSF_AT(int, self, 0x10) == 1)
    {
        void* geometry = CSF_AT(void*, self, 0x0C);
        if (!reinterpret_cast<bool (__fastcall*)(void*, void*)>(
                CSF_AT(void**, geometry, 0)[0x10 / 4])(geometry, nullptr))
        {
            reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
                self->vtbl[0x10 / 4])(self, nullptr, 0);
        }
    }

    if (!CSF_AT(uint32_t, self, 0x18))
        return 0;

    C_00867260_UpdateRenderGeometry(self, nullptr);

    CSF_AT(RenderStateSetter, g_pRwEngineInstance, 0x20)(12, 1);
    CSF_AT(RenderStateSetter, g_pRwEngineInstance, 0x20)(10, 5);
    CSF_AT(RenderStateSetter, g_pRwEngineInstance, 0x20)(14, 0);

    const uint32_t raster = CSF_AT(
        uint32_t, CSF_AT(void*, self, 0x18), 0);
    CSF_AT(RenderStateSetter, g_pRwEngineInstance, 0x20)(1, raster);

    CSF_AT(RenderStateSetter, g_pRwEngineInstance, 0x20)(9, 2);
    CSF_AT(RenderStateSetter, g_pRwEngineInstance, 0x20)(6, 0);
    CSF_AT(RenderStateSetter, g_pRwEngineInstance, 0x20)(8, 0);
    CSF_AT(RenderStateSetter, g_pRwEngineInstance, 0x20)(20, 1);

    if (CSF_AT(int, self, 0x78) == 1)
        CSF_AT(RenderStateSetter, g_pRwEngineInstance, 0x20)(11, 2);
    else
        CSF_AT(RenderStateSetter, g_pRwEngineInstance, 0x20)(11, 6);

    const uint32_t vertices = CSF_AT(uint32_t, self, 0x14);
    void* geometry = CSF_AT(void*, self, 0x0C);
    return reinterpret_cast<int (__fastcall*)(void*, void*, int, uint32_t, int)>(
        CSF_AT(void**, geometry, 0)[0x24 / 4])(
            geometry, nullptr, 4, vertices, 4);
}

HOOK(0x00737D70, C_00867260_Render, "auto");
