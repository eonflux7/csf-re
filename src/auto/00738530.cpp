#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, C_00867318_PrepareRender, 0x00738300,
         (void* self, void* edx));

CSF_GLOBAL(void*, g_pRwEngineInstance, 0x008B68C0);

static int __fastcall C_00867318_Render(C_00867318* self, void* /*edx*/)
{
    if (!self->field_001c && !self->field_0020)
        return 0;

    if (self->field_0004 && self->field_0008 == 1 &&
        !reinterpret_cast<bool (__fastcall*)(void*, void*)>(
            CSF_AT(void**, reinterpret_cast<void*>(self->field_0004), 0)[4])(
                reinterpret_cast<void*>(self->field_0004), nullptr))
    {
        reinterpret_cast<void (__fastcall*)(C_00867318*, void*, int)>(
            self->vtbl[4])(self, nullptr, 0);
    }

    C_00867318_PrepareRender(self, nullptr);

    reinterpret_cast<void (__cdecl*)(int, int)>(
        CSF_AT(void*, g_pRwEngineInstance, 0x20))(10, 5);
    reinterpret_cast<void (__cdecl*)(int, int)>(
        CSF_AT(void*, g_pRwEngineInstance, 0x20))(11, 6);
    reinterpret_cast<void (__cdecl*)(int, int)>(
        CSF_AT(void*, g_pRwEngineInstance, 0x20))(12, 1);
    reinterpret_cast<void (__cdecl*)(int, int)>(
        CSF_AT(void*, g_pRwEngineInstance, 0x20))(14, 0);
    reinterpret_cast<void (__cdecl*)(int, int)>(
        CSF_AT(void*, g_pRwEngineInstance, 0x20))(1, 0);
    reinterpret_cast<void (__cdecl*)(int, int)>(
        CSF_AT(void*, g_pRwEngineInstance, 0x20))(9, 2);
    reinterpret_cast<void (__cdecl*)(int, int)>(
        CSF_AT(void*, g_pRwEngineInstance, 0x20))(6, 0);
    reinterpret_cast<void (__cdecl*)(int, int)>(
        CSF_AT(void*, g_pRwEngineInstance, 0x20))(8, 0);
    reinterpret_cast<void (__cdecl*)(int, int)>(
        CSF_AT(void*, g_pRwEngineInstance, 0x20))(20, 1);

    const bool alternateMode = self->field_0034 != 0;
    void* renderObject = reinterpret_cast<void*>(self->field_0004);
    const uint32_t drawArgument = self->field_000c;
    auto draw = reinterpret_cast<int (__stdcall*)(int, uint32_t, int)>(
        CSF_AT(void**, renderObject, 0)[9]);

    if (alternateMode)
        return draw(4, drawArgument, 4);

    return draw(2, drawArgument, 5);
}

HOOK(0x00738530, C_00867318_Render, "auto");
