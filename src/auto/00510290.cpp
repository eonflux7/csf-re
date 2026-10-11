#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original callee: fills an out-pointer and two pointers derived from the block (3 stack args, ret 12).
CSF_FUNC(void**, __fastcall, sub_50F230, 0x0050F230, (void* self, void* edx, void** a2, int* a3, int* a4));
// Original game-heap free (cdecl, caller pops).
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

// Vtable reference listed in the packet for the 0x50F230 call site. Kept as a file-local, side-effect-free binding.
static const void* const kS_0050f230VtblRef = CSF_VTBL(0x0050F230);

static int __fastcall S_0050f230_ReleaseBlockAndClear(S_0050f230* self, void* /*edx*/)
{
    // Load the block pointer once; the original keeps it in a register across the first call.
    uint32_t blockPtr = self->field_0004;
    void** block = reinterpret_cast<void**>(blockPtr);
    int* firstWord = reinterpret_cast<int*>(*block);

    // Out-value written by the callee (original uses a 4-byte stack slot, left uninitialized).
    void* outValue;
    sub_50F230(self, nullptr, &outValue, firstWord, reinterpret_cast<int*>(blockPtr));

    // Re-read the field after the call, as the original does.
    int result = static_cast<int>(self->field_0004);
    if (result != 0) {
        result = Mem_FreeDebug(reinterpret_cast<void*>(self->field_0004));
    }

    self->field_0004 = 0;
    self->field_0008 = 0;
    return result;
}

HOOK(0x510290, S_0050f230_ReleaseBlockAndClear, "auto");
