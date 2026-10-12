#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original: 0x00651D30, __fastcall-style callee with one stack argument (ret 4).
CSF_FUNC(int, __fastcall, NetSession_CopyObjectReference, 0x00651D30, (void* self, void* edx, int* a2));

// Original: 0x0074A750, __thiscall with one stack argument (ret 4).
// Copies the object reference from a2 via NetSession, then returns this.
static void* __fastcall C_00873b14_CopyRefAndReturnSelf(C_00873b14* self, void* /*edx*/, int* a2)
{
    NetSession_CopyObjectReference(self, nullptr, a2);
    return self;
}

HOOK(0x74A750, C_00873b14_CopyRefAndReturnSelf, "auto");
