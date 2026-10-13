#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(C_00860178*, __fastcall, C_00860178_Construct, 0x006B2820, (C_00860178* self, void*));
CSF_FUNC(int, __cdecl, GfxFrames_ForEachClumpAtomicWithPair, 0x006C0200, (int clump, int pairA, int pairB));

// Constructs a C_00862900 for clump enumeration: runs the base constructor, sets up a
// 4-dword parameter block (0, 0, 0, 10) inside the object, stores the clump handle and
// the caller's extra argument, then enumerates the clump's atomics with a callback.
static C_00862900* __fastcall C_00862900_ConstructAndEnumerateClumps(C_00862900* self, void* /*edx*/, int clump, int extraArg)
{
    C_00860178_Construct(reinterpret_cast<C_00860178*>(self), nullptr);
    self->vtbl = CSF_VTBL(0x00862900);

    uint8_t* parameterBlock = reinterpret_cast<uint8_t*>(self) + 0x14;
    CSF_AT(uint32_t, self, 0x14) = 0;
    CSF_AT(uint32_t, self, 0x18) = 0;
    CSF_AT(uint32_t, self, 0x1C) = 0;
    CSF_AT(uint32_t, self, 0x20) = 10;

    self->field_000c = 0;
    self->field_0004 = static_cast<uint32_t>(clump);
    self->field_0008 = 0xFFFFFFFFu;
    self->field_0010 = static_cast<uint32_t>(extraArg);

    GfxFrames_ForEachClumpAtomicWithPair(
        clump,
        static_cast<int>(reinterpret_cast<uintptr_t>(CSF_ADDR_FN(0x006F7540))),
        static_cast<int>(reinterpret_cast<uintptr_t>(parameterBlock)));

    return self;
}

HOOK(0x6F76C0, C_00862900_ConstructAndEnumerateClumps, "auto");
