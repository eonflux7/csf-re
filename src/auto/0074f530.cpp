#include <cstdint>
#include "hook.h"

// Returns the byte stored at +0x48 of the second argument (a pointer passed as int).
// The original only writes AL (genuine byte-sized return).
static char __stdcall GetByteAtOffset48(int /*a1*/, int a2, int /*a3*/, int /*a4*/)
{
    return *reinterpret_cast<const char*>(static_cast<uintptr_t>(a2) + 0x48);
}

HOOK(0x74F530, GetByteAtOffset48, "auto");
