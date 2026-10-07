#include <cstdint>
#include "hook.h"

// Returns the raw byte field stored at object offset 0x24 of the second argument.
// The original only loads AL (upper bits of EAX keep the pointer value), so the
// result is a plain 8-bit char.
static char __stdcall GetByteFieldAtOffset24(int /*a1*/, int a2, int /*a3*/, int /*a4*/)
{
    return *reinterpret_cast<const char*>(static_cast<uintptr_t>(static_cast<uint32_t>(a2)) + 0x24);
}

HOOK(0x00742A10, GetByteFieldAtOffset24, "auto");
