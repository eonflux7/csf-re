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

// Stored effect object at +0x08 is queried via its vtable slot 4 (+0x10),
// a __stdcall method taking (this, &out1, &out2). Result is the second out value.
static int __fastcall C_00863d18_QueryEffectValue(C_00863d18* self, void* /*edx*/) {
    using EffectQueryFn = int(__stdcall*)(void*, int*, int*);

    int firstOutValue = 0;
    int secondOutValue = 0;
    const uint32_t effectObjAddr = self->field_0008;

    if (effectObjAddr != 0) {
        void* effectObj = reinterpret_cast<void*>(effectObjAddr);
        EffectQueryFn queryFn = reinterpret_cast<EffectQueryFn>(CSF_AT(void**, effectObj, 0)[0x10 / 4]);
        queryFn(effectObj, &firstOutValue, &secondOutValue);
        return secondOutValue;
    }
    return 0;
}

HOOK(0x7117C0, C_00863d18_QueryEffectValue, "auto");
