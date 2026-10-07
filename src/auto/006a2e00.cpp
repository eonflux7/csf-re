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

// 0x006A2E00 — returns field_001c + field_0010 of C_0085ee28.
static int __fastcall C_0085ee28_SumFields(C_0085ee28* self, void* /*edx*/)
{
    return static_cast<int>(self->field_001c + self->field_0010);
}

HOOK(0x6A2E00, C_0085ee28_SumFields, "auto");
