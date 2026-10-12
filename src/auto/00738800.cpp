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

// Selects a phase code from a 0/1/other index. Index 0 sets the field to 4,
// index 1 sets it to 3, and any other value only returns (index - 1) without
// touching the field. The return value is the index minus one, except that
// index 0 returns 0 (the original keeps the untouched a2 in eax on that path).
static int __fastcall C_00867358_SetPhaseByIndex(C_00867358* self, void* /*edx*/, int index) {
    int result = index;
    if (index != 0) {
        result = index - 1;
        if (index == 1) {
            self->field_001c = 3;
        }
    } else {
        self->field_001c = 4;
    }
    return result;
}

HOOK(0x738800, C_00867358_SetPhaseByIndex, "auto");
