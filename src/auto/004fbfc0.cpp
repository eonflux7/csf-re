#include <cstdint>
#include "hook.h"

// Returns the dword stored in the inline array at offset +0x38, indexed by `index`.
// Original: mov eax, [ecx + eax*4 + 0x38]; ret 4
static int __fastcall GetArrayElementAt38(void* self, void* /*edx*/, int index)
{
    return reinterpret_cast<int*>(static_cast<uint8_t*>(self) + 0x38)[index];
}

HOOK(0x004FBFC0, GetArrayElementAt38, "auto");
