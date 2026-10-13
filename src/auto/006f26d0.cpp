#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Forwards a single pointer argument to virtual slot 0x38 (index 14) of the object.
// The original pushes only a2; a3 is never read, and ret 8 pops both stack args.
static int __fastcall C_00862598_ForwardToVirtualSlot38(C_00862598* self, void* /*edx*/, int a2, int /*a3*/)
{
    return ((int(__fastcall*)(void*, void*, uint32_t*))self->vtbl[0x38 / 4])(
        self, nullptr, reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(a2)));
}

HOOK(0x6F26D0, C_00862598_ForwardToVirtualSlot38, "auto");
