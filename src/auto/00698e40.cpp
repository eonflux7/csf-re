#include <cstdint>
#include <cstddef>
#include "hook.h"
#include "types/auto_classes.h"

// Skips spaces, tabs and CRLF pairs starting at the cursor (field_0018), bounded by
// the end pointer (field_001c). Returns 1 if the end of the buffer was reached,
// otherwise 0 with the cursor left on the first non-blank character.
static char __fastcall SortedTable_SkipWhitespaceToEnd(S_00698c10* self, void* /*edx*/) {
    const uint32_t end = self->field_001c;

    for (;;) {
        const uint32_t cursor = self->field_0018;
        const uint32_t next = cursor + 2;
        if (next > end) {
            return 1;
        }

        const int16_t ch = *reinterpret_cast<const int16_t*>(static_cast<uintptr_t>(cursor));
        self->field_0018 = next;

        if (ch == ' ' || ch == '\t') {
            continue;
        }

        if (ch != '\r') {
            break;
        }

        // Carriage return: only consume it if a line feed follows.
        const uint32_t afterCr = next + 2;
        if (afterCr > end) {
            break;
        }

        const int16_t lf = *reinterpret_cast<const int16_t*>(static_cast<uintptr_t>(next));
        self->field_0018 = afterCr;
        if (lf == '\n') {
            continue;
        }

        self->field_0018 = afterCr - 2;
        break;
    }

    self->field_0018 -= 2;
    return 0;
}

HOOK_KEEP_ECX(0x698E40, SortedTable_SkipWhitespaceToEnd, 0, "core_sorted_table");
