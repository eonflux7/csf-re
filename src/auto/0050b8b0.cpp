#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Returns a pointer to the character storage of a string-like object.
// Layout: an inline character buffer lives at +0x38, and a size/capacity-like
// counter lives at +0x4C. If that counter is below 8 the object still uses its
// small inline buffer (address self+0x38); otherwise the pointer stored at
// +0x38 refers to the separately allocated heap buffer.
static void* __fastcall GetInlineOrHeapBuffer(void* self, void* /*edx*/)
{
    if (CSF_AT(uint32_t, self, 0x4c) < 8u)
        return static_cast<uint8_t*>(self) + 0x38;
    return CSF_AT(void*, self, 0x38);
}

HOOK(0x0050B8B0, GetInlineOrHeapBuffer, "auto");
