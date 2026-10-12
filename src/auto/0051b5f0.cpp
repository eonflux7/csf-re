#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original 0x0051B5F0 is a tail jump into C_00869cd0_GetRecordAddress. It never writes ecx,
// so ecx on entry (the caller's `this`) reaches the callee unchanged. The stack argument is
// the record index, and the callee pops it with ret 4.
CSF_FUNC(int, __fastcall, C_00869cd0_GetRecordAddress, 0x00755BB0, (C_00869cd0* self, void* /*edx*/, int index));

// Entry layout of the original: ecx = caller's this (passed through), edx = unused,
// [esp+4] = index. Declared __fastcall with a third parameter so the index is read from the
// stack and popped by ret 4, matching the original.
static int __fastcall BddTables_ForwardGetRecordAddress(C_00869cd0* self, void* /*edx*/, int recordIndex)
{
    return C_00869cd0_GetRecordAddress(self, nullptr, recordIndex);
}

HOOK(0x0051B5F0, BddTables_ForwardGetRecordAddress, "auto");
