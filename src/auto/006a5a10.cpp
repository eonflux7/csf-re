#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "core/memory.h"

// Allocator callback stored in the stream object (zlib-style zalloc).
// The original calls it as __cdecl with 3 args: (opaque, items, size); the caller pops them.
typedef void* (__cdecl* ZlibAllocFn)(void* opaque, unsigned int items, unsigned int size);

static int __cdecl ZlibLog_AllocateTaggedNode(char tagByte, char flagByte, int valueA, int valueB, void* stream)
{
    // zalloc(opaque, 1, 28): the function pointer lives at stream+0x20, the opaque context at stream+0x28.
    ZlibAllocFn allocate = CSF_AT(ZlibAllocFn, stream, 0x20);
    void* opaque = CSF_AT(void*, stream, 0x28);
    void* node = allocate(opaque, 1u, 28u);
    if (node != nullptr)
    {
        CSF_AT(uint8_t, node, 0x10) = static_cast<uint8_t>(tagByte);
        CSF_AT(uint8_t, node, 0x11) = static_cast<uint8_t>(flagByte);
        CSF_AT(int, node, 0x00) = 0;
        CSF_AT(int, node, 0x14) = valueA;
        CSF_AT(int, node, 0x18) = valueB;
    }
    return static_cast<int>(reinterpret_cast<intptr_t>(node));
}

HOOK(0x6A5A10, ZlibLog_AllocateTaggedNode, "auto");
