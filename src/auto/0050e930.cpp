#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Inner object referenced by the menu manager at +0x1C (its vtable is virtual-dispatched).
static uint8_t* MenuManager_InnerOf(int* self) {
    return reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, self, 0x1C));
}

// Forwarder: tail-jumps to a virtual/function pointer. Called with ecx = inner + 0x10 and no pushes.
CSF_FUNC(void, __fastcall, SceneLoader_EndContext, 0x0069AEF0, (void* self, void* edx));

CSF_FUNC(int, __cdecl, SceneLoader_SetActiveContext, 0x00697360, (int a1));

CSF_FUNC(char, __fastcall, TakeFlagAndClearPending, 0x00528830, (uint8_t* self, void* edx));

static int __fastcall MenuManager_SetInnerState(int* self, void* /*edx*/, char a2) {
    int result = CSF_AT(int, self, 0x1C);

    if (CSF_AT(uint8_t, MenuManager_InnerOf(self), 0x50)) {
        SceneLoader_EndContext(MenuManager_InnerOf(self) + 0x10, nullptr);
        SceneLoader_SetActiveContext(0);

        if (a2) {
            uint8_t* inner = MenuManager_InnerOf(self);
            ((void(__fastcall*)(void*, void*))CSF_AT(void**, inner, 0)[0x48 / 4])(inner, nullptr);
        }

        CSF_AT(uint8_t, MenuManager_InnerOf(self), 0x50) = 0;

        {
            uint8_t* inner = MenuManager_InnerOf(self);
            ((void(__fastcall*)(void*, void*))CSF_AT(void**, inner, 0)[0x3C / 4])(inner, nullptr);
        }

        {
            uint8_t* inner = MenuManager_InnerOf(self);
            result = ((int(__fastcall*)(void*, void*))CSF_AT(void**, inner, 0)[0x40 / 4])(inner, nullptr);
        }

        if (CSF_AT(int, self, 0x80)) {
            return TakeFlagAndClearPending(reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, self, 0x80)), nullptr);
        }
    }
    return result;
}

HOOK(0x0050E930, MenuManager_SetInnerState, "auto");
