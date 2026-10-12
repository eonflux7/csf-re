#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"

// Callee bound by address: 0x0040AC30, __thiscall (ret 4), taking one stack argument.
CSF_FUNC(uint32_t*, __fastcall, CopySubobjectFromSource, 0x0040AC30, (void* self, void* edx, uint32_t* a2));

// __stdcall (ret 8): copies the first three dwords of the source into the destination,
// then forwards the remaining subobject (at +12 bytes of each) to CopySubobjectFromSource.
static uint32_t* __stdcall CopyTripleThenDelegate(uint32_t* dest, uint32_t* src)
{
    uint32_t* result = dest;
    if (dest)
    {
        dest[0] = src[0];
        dest[1] = src[1];
        dest[2] = src[2];
        return CopySubobjectFromSource(dest + 3, nullptr, src + 3);
    }
    return result;
}

HOOK(0x0040AEB0, CopyTripleThenDelegate, "auto");
