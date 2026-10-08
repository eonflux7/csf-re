# include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// 0x00542BA0  (CMenuDemoExit)
// If the GUI sub-object pointer stored at +0x14 is non-null, invoke its first virtual
// method with argument 1 (a release/destroy request), then clear the stored pointer.
// Returns the callee's result; undefined when no sub-object is stored.
static int __fastcall CMenuDemoExit_ReleaseGui(CMenuDemoExit* self, void* /*edx*/)
{
    int result;
    void** gui = reinterpret_cast<void**>(static_cast<uintptr_t>(self->m_pGui));
    if ( gui )
    {
        // vtable slot 0, __thiscall with one stack argument (popped by the callee)
        result = reinterpret_cast<int(__fastcall*)(void*, void*, int)>(gui[0])(gui, nullptr, 1);
        self->m_pGui = 0;
    }
    return result;
}

HOOK(0x542BA0, CMenuDemoExit_ReleaseGui, "auto");
