#include <cstdint>
#include "hook.h"

// Callback-shaped helper: returns the byte stored at offset 0x0C of the object
// passed as the second stack argument. The other three arguments are unused.
// Original: char __stdcall sub_74FD20(int a1, int a2, int a3, int a4);  ret 0x10
static char __stdcall ReadByteAtOffset0C(int /*unused1*/, int source, int /*unused2*/, int /*unused3*/)
{
    return *reinterpret_cast<char*>(source + 0x0C);
}

HOOK(0x74FD20, ReadByteAtOffset0C, "auto");
