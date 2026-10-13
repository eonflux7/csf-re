#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Child object pointer stored at this+0x134.
// Global flag byte whose address is passed to the child's virtual method.
CSF_GLOBAL(uint8_t, g_byte_82E65C, 0x0082E65C);

// Forwards to the child at this+0x134: calls its vtable slot 15 (offset 0x3C)
// with &g_byte_82E65C as the single argument, returning the callee's result.
static int __fastcall CSubObject_ReleaseChild(void* self, void* /*edx*/)
{
    void* child = CSF_AT(void*, self, 0x134);
    void** childVtbl = CSF_AT(void**, child, 0);
    using ChildSlot15Fn = int(__fastcall*)(void* self, void* edx, uint8_t* flag);
    return reinterpret_cast<ChildSlot15Fn>(childVtbl[15])(child, nullptr, &g_byte_82E65C);
}

HOOK(0x00436770, CSubObject_ReleaseChild, "auto");
