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

CSF_FUNC(char, __fastcall, ValidateTableTail, 0x00699020, (void* self, void* edx));

// Reads the 16-bit marker at the table cursor, advances the cursor by 2, and
// succeeds only if the marker is 0xFEFF and the tail validator returns zero.
static bool __fastcall S_00698c10_AdvanceCursorAndMatchMarker(S_00698c10* self, void* /*edx*/)
{
    uint32_t cursor = self->field_0018;
    uint32_t nextCursor = cursor + 2;
    if (nextCursor > self->field_001c)
        return false;

    int16_t marker = *reinterpret_cast<int16_t*>(cursor);
    self->field_0018 = nextCursor;
    if (marker != static_cast<int16_t>(0xFEFF))
        return false;

    return ValidateTableTail(self, nullptr) == 0;
}

HOOK(0x006991F0, S_00698c10_AdvanceCursorAndMatchMarker, "auto");
