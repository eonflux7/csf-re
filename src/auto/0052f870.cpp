#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// 0x00507810: __thiscall char f(int this, float a2) -- advances some menu sub-object at this+0x20.
// Called via ecx = (uint8_t*)self + 0x20, one float on the stack (callee pops 4).
CSF_FUNC(char, __fastcall, MenuWidget_AdvanceAnimation, 0x507810, (int self, void* /*edx*/, float a2));

// 0x00506AC0: void nullsub_2() -- no-op call kept for exact call ordering.
CSF_FUNC(void, __cdecl, Nullsub_506AC0, 0x506AC0, ());

// 0x0052F870: CMenuCredits::AdvanceAnimation(float)
static void __fastcall CMenuCredits_AdvanceAnimation(CMenuCredits* self, void* /*edx*/, float a2)
{
    // thiscall on the animation sub-object embedded at offset 0x20 of the menu.
    char advanced = MenuWidget_AdvanceAnimation(reinterpret_cast<int>((uint8_t*)self + 0x20), nullptr, a2);

    Nullsub_506AC0();

    if (advanced)
    {
        // Virtual slot at vtable byte offset 0x40 (index 16), invoked on the menu itself.
        void** vtbl = *reinterpret_cast<void***>(self);
        ((void (__fastcall*)(void*, void*))vtbl[0x40 / 4])(self, nullptr);
    }
}

HOOK(0x52F870, CMenuCredits_AdvanceAnimation, "auto");
