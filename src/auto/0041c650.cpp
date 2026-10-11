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
CSF_FUNC(unsigned int, __fastcall, ProcessEntryByKey, 0x00408540, (void* self, void* edx, int a2));

// Original: if (this->field_008C == 1) return ProcessEntryByKey(this->field_0030, 25);
// Otherwise the result is left uninitialized by the original; it is passed as 0.
static int __fastcall Wheels_ProcessEntryWhenState1(void* self, void* /*edx*/)
{
    int result = 0;

    if (CSF_AT(int, self, 0x8C) == 1)
    {
        void* entryOwner = CSF_AT(void*, self, 0x30);
        result = static_cast<int>(ProcessEntryByKey(entryOwner, nullptr, 25));
    }

    return result;
}

HOOK(0x0041C650, Wheels_ProcessEntryWhenState1, "auto");
