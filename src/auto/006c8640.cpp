#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"

// Forwards a nonzero argument to the child object at +0x04 unless the child's
// virtual predicate (vtbl slot 17) reports it as handled.
static void __fastcall ForwardToChildIfUnhandled(void* self, void* /*edx*/, int a2)
{
    if (a2 != 0)
    {
        void* child = CSF_AT(void*, self, 0x4);
        if (child != nullptr)
        {
            // vtbl[0x44 / 4] : bool IsHandled(void) — no arguments
            bool handled = ((bool(__fastcall*)(void*, void*))CSF_AT(void**, child, 0)[0x44 / 4])(child, nullptr);
            if (!handled)
            {
                // The original re-reads the child pointer before this second call.
                void* childAgain = CSF_AT(void*, self, 0x4);
                // vtbl[0x3c / 4] : void Process(int) — thiscall, one stack argument
                ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, childAgain, 0)[0x3c / 4])(childAgain, nullptr, a2);
            }
        }
    }
}

HOOK(0x6C8640, ForwardToChildIfUnhandled, "auto");
