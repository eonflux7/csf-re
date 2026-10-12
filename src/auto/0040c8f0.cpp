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

// Bound by address: __fastcall callee (ecx = self, edx unused), pops 4 bytes (ret 4).
CSF_FUNC(uint32_t, __fastcall, FindEntryByKeyAndProcess, 0x00408540, (void* self, void* /*edx*/, int key));

// Original: if (this->field_0090 == 1) return FindEntryByKeyAndProcess(this->field_008C, 0);
// Otherwise the result is left uninitialized by the original; it is passed as 0.
static int __fastcall EntryLookup_FindEntryForOwnerWhenState1(void* self, void* /*edx*/)
{
    int result = 0;

    if (CSF_AT(int, self, 0x90) == 1)
    {
        void* entryOwner = CSF_AT(void*, self, 0x8C);
        result = static_cast<int>(FindEntryByKeyAndProcess(entryOwner, nullptr, 0));
    }

    return result;
}

HOOK(0x0040C8F0, EntryLookup_FindEntryForOwnerWhenState1, "auto");
