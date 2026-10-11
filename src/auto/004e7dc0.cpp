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

// Primary-query worker: tests one priority level and reports whether it matched.
CSF_FUNC(bool, __fastcall, sub_4E7CB0, 0x004E7CB0, (void* self, void* edx, int arg0, int a3, uint32_t* a4));

// Tests the query at a2 against priority levels 3, 2, 1, 0 in that order.
// Returns true as soon as one level succeeds. A query whose field at +8 is
// negative is rejected immediately.
static bool __fastcall C_0083e9d0_TestPrimaryQuery(C_0083e9d0* self, void* /*edx*/, int a2, uint32_t* a3)
{
    uint8_t* query = reinterpret_cast<uint8_t*>(a2);

    if (CSF_AT(int, query, 0x008) < 0)
        return false;

    if (sub_4E7CB0(self, nullptr, 3, a2, a3))
        return true;
    if (sub_4E7CB0(self, nullptr, 2, a2, a3))
        return true;
    if (sub_4E7CB0(self, nullptr, 1, a2, a3))
        return true;
    if (sub_4E7CB0(self, nullptr, 0, a2, a3))
        return true;

    return false;
}

HOOK(0x004E7DC0, C_0083e9d0_TestPrimaryQuery, "auto");
