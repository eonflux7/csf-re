#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// RtAnimAnimationGetNumNodes(uint32_t* a1), cdecl, pops 4 bytes in the caller.
CSF_FUNC(int, __cdecl, RtAnimAnimationGetNumNodes, 0x0061EFB0, (uint32_t* a1));

// __thiscall, ret 0. Reads the first dword of the object (its stored pointer)
// and passes that value to RtAnimAnimationGetNumNodes; the result is discarded.
static void __fastcall Timevec_Destruct(void* self, void* /*edx*/)
{
    uint32_t firstWord = CSF_AT(uint32_t, self, 0);
    RtAnimAnimationGetNumNodes(reinterpret_cast<uint32_t*>(firstWord));
}

HOOK(0x006C2040, Timevec_Destruct, "auto");
