#include <cstdint>
#include "globals.h"
#include "hook.h"

// Original forwards a2/a3 to two virtual slots of a1, then tail-jumps through
// a1's vtable slot 2 with the caller's stack args untouched.
static hook::Tail __fastcall GuiEffect_DrawLine(void* self, const uint32_t* args) {
    (void)self;
    void* obj = reinterpret_cast<void*>(args[0]);  // a1
    if (obj != nullptr) {
        using SlotIntFn = void(__fastcall*)(void* self, void* edx, int value);

        // (*(a1->vtbl + 0x1C))(a1, a2)
        int a2 = static_cast<int>(args[1]);
        ((SlotIntFn)(*reinterpret_cast<void***>(obj))[0x1C / 4])(obj, nullptr, a2);

        // (*(a1->vtbl + 0x24))(a1, a3)
        int a3 = static_cast<int>(args[2]);
        ((SlotIntFn)(*reinterpret_cast<void***>(obj))[0x24 / 4])(obj, nullptr, a3);

        // jmp dword ptr [edx + 8] with edx = a1->vtbl, ecx = a1
        return hook::tail_jump(obj, (*reinterpret_cast<void***>(obj))[0x08 / 4]);
    }
    return hook::tail_ret(0);
}

HOOK_TAIL(0x00713C60, GuiEffect_DrawLine, "gui_effects");
