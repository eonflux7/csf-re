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

// Original: if (this->field_0050 == 1) return FindEntryByKeyAndProcess(this->field_0030, 1);
// Otherwise the result is left uninitialized by the original; it is passed as 0.
static int __fastcall EntryLookup_FindEntryWhenState1(void* self, void* /*edx*/)
{
    int result = 0;

    if (CSF_AT(int, self, 0x50) == 1)
    {
        void* entryOwner = CSF_AT(void*, self, 0x30);
        result = static_cast<int>(FindEntryByKeyAndProcess(entryOwner, nullptr, 1));
    }

    return result;
}

HOOK(0x0040DA90, EntryLookup_FindEntryWhenState1, "auto");
