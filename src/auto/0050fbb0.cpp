#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Callee at 0x006DEC40 is __cdecl. Bound exactly as given in the packet; it is
// called here with four forwarded values: the two original stack arguments, the
// incoming ECX (garbage in a __stdcall function, passed as 0), and the second
// argument repeated.
CSF_FUNC(int, __cdecl, ResetRecordRange, 0x006DEC40, (int* begin, int* end, ...));

// 0x0050FBB0: __stdcall thunk. Original pushes (arg1, ECX, arg1, arg0) so the callee
// sees (arg0, arg1, ECX, arg1). The two stack arguments are callee-cleaned (ret 8).
// ECX is not an input of this __stdcall function, so the third value is 0.
static int __stdcall BddLoader_DestroyRecordRange(int* firstArgument, int* secondArgument)
{
    return ResetRecordRange(
        firstArgument,
        secondArgument,
        0,
        secondArgument);
}

HOOK(0x0050FBB0, BddLoader_DestroyRecordRange, "auto");
