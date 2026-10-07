#include <cstdint>
#include "hook.h"

// Returns the embedded sub-object starting at this+4, or nullptr when the
// incoming pointer equals the sentinel value 4 (original: lea eax,[ecx-4];
// test eax,eax).  __thiscall: ecx = self, no stack args (ret 0).
static char* __fastcall GetInnerObjectAtOffset4(char* self, void* /*edx*/)
{
    if (self == reinterpret_cast<char*>(4))
        return nullptr;
    return self + 4;
}

HOOK(0x006B35C0, GetInnerObjectAtOffset4, "auto");
