#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"

// Binds the base-chain initializer (0x0043ABF0). The original reaches it via a tail jmp,
// so this wrapper forwards self and a null edx and returns its result unchanged.
CSF_FUNC(int, __fastcall, Minimap_InitChain, 0x0043ABF0, (void* self, void* edx));

// Sets this object's vtable to C_00830d90, then tail-forwards to the base initializer.
static int __fastcall C_00830d90_SetVtableAndForwardInit(void* self, void* /*edx*/)
{
    *(void***)self = CSF_VTBL(0x00830D90);
    return Minimap_InitChain(self, nullptr);
}

HOOK(0x0043AD60, C_00830d90_SetVtableAndForwardInit, "auto");
