#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

static int16_t __fastcall ScanToCrLfPairInBuffer(S_00698c10* self, void* /*edx*/) {
    const uint32_t limit = self->field_001c;
    int16_t result = 0;

    for (;;) {
        uint16_t* cursor = reinterpret_cast<uint16_t*>(self->field_0018);
        uint16_t* next = cursor + 1;
        result = static_cast<int16_t>(reinterpret_cast<uintptr_t>(next));
        if (reinterpret_cast<uintptr_t>(next) > limit) {
            break;
        }

        uint16_t unit = *cursor;
        self->field_0018 = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(next));
        if (unit != 13) {
            continue;
        }

        uint16_t* after = next + 1;
        if (reinterpret_cast<uintptr_t>(after) > limit) {
            continue;
        }

        uint16_t following = *next;
        self->field_0018 = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(after));
        if (following == 10) {
            return static_cast<int16_t>(following);
        }
        self->field_0018 = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(next));
    }

    return result;
}

HOOK_KEEP_ECX(0x698C10, ScanToCrLfPairInBuffer, 0, "core_sorted_table");
