#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __fastcall, GetLinkedNodeFor, 0x006E9030, (void* self, void* edx, int a2));

// Returns true when the node resolved by GetLinkedNodeFor has bit 2 set in its
// flags dword at +0x14. Returns false when the pointer at +0x1C is null.
static char __fastcall C_00861e98_IsBit2SetOnLinkedNode(C_00861e98* self, void* /*edx*/, int a2)
{
    if (CSF_AT(uint32_t, self, 0x1C) != 0)
    {
        void* node = reinterpret_cast<void*>(static_cast<uintptr_t>(
            GetLinkedNodeFor(self, nullptr, a2)));
        return static_cast<char>((CSF_AT(uint32_t, node, 0x14) >> 2) & 1);
    }
    return 0;
}

HOOK(0x006E9790, C_00861e98_IsBit2SetOnLinkedNode, "auto");
