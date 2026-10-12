#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, C_008672e4_PrepareRender, 0x00737F40, (void* self, void* edx));
CSF_GLOBAL(void*, g_pRwEngineInstance, 0x008B68C0);

using RenderStateSetter = void (__cdecl*)(int, int);

static int __fastcall C_008672e4_Render(C_008672e4* self, void* /*edx*/)
{
    if (self->field_0004 && self->field_0008 == 1)
    {
        void* object = reinterpret_cast<void*>(self->field_0004);
        if (!reinterpret_cast<bool (__fastcall*)(void*, void*)>(
                CSF_AT(void**, object, 0)[0x10 / 4])(object, nullptr))
        {
            reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, self, 0)[0x10 / 4])(self, nullptr, 0);
        }
    }

    C_008672e4_PrepareRender(self, nullptr);

    CSF_AT(RenderStateSetter, g_pRwEngineInstance, 0x20)(10, 5);
    CSF_AT(RenderStateSetter, g_pRwEngineInstance, 0x20)(11, 6);
    CSF_AT(RenderStateSetter, g_pRwEngineInstance, 0x20)(12, 1);
    CSF_AT(RenderStateSetter, g_pRwEngineInstance, 0x20)(14, 0);
    CSF_AT(RenderStateSetter, g_pRwEngineInstance, 0x20)(1, 0);
    CSF_AT(RenderStateSetter, g_pRwEngineInstance, 0x20)(9, 2);
    CSF_AT(RenderStateSetter, g_pRwEngineInstance, 0x20)(6, 0);
    CSF_AT(RenderStateSetter, g_pRwEngineInstance, 0x20)(8, 0);
    CSF_AT(RenderStateSetter, g_pRwEngineInstance, 0x20)(20, 1);

    const uint32_t renderData = self->field_000c;
    void* object = reinterpret_cast<void*>(self->field_0004);
    return reinterpret_cast<int (__fastcall*)(void*, void*, int, uint32_t, int)>(
        CSF_AT(void**, object, 0)[0x24 / 4])(object, nullptr, 1, renderData, 2);
}

HOOK(0x00738070, C_008672e4_Render, "auto");
