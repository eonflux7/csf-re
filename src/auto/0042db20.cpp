#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __fastcall, IsSceneActive, 0x0044F850, (void* self, void* edx));
CSF_FUNC(double, __fastcall, GetSceneScaleValue, 0x0044F870, (void* self, void* edx));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

// EAX carries the original return value or tail object.
// EDX indicates whether the entry thunk should tail-dispatch.
static uint64_t __fastcall Hud_UpdateWidgets(void* self, void* /*edx*/)
{
    const int result = IsSceneActive(g_pCurrentScene, nullptr);
    if (static_cast<uint8_t>(result) == 0)
        return static_cast<uint32_t>(result);

    void* widget = CSF_AT(void*, self, 0x298);
    if (widget) {
        void** widgetVtable = CSF_AT(void**, widget, 0);
        const float scale = static_cast<float>(
            GetSceneScaleValue(g_pCurrentScene, nullptr));

        widget = CSF_AT(void*, self, 0x298);
        reinterpret_cast<void(__fastcall*)(void*, void*, float)>(
            widgetVtable[8])(widget, nullptr, scale);

        widget = CSF_AT(void*, self, 0x298);
        reinterpret_cast<void(__fastcall*)(void*, void*)>(
            CSF_AT(void**, widget, 0)[2])(widget, nullptr);
    }

    const uint32_t tailObject = CSF_AT(uint32_t, self, 0x29C);
    if (!tailObject)
        return 0;

    return (uint64_t{1} << 32) | tailObject;
}

static uint64_t (__fastcall* s_UpdateWidgets)(void*, void*) = &Hud_UpdateWidgets;

static __declspec(naked) int __fastcall Hud_UpdateAndDispatch(
    void* self, void* edx)
{
    __asm {
        mov eax, dword ptr [s_UpdateWidgets]
        call eax
        test edx, edx
        jne dispatch
        ret
    dispatch:
        mov ecx, eax
        mov eax, dword ptr [ecx]
        jmp dword ptr [eax + 8]
    }
}

HOOK(0x0042DB20, Hud_UpdateAndDispatch, "auto");
