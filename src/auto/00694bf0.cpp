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

// Replaces every occurrence of `from` with `to` in a NUL-terminated buffer, in place.
// Returns a pointer to the terminating NUL (or to `buffer` if it is null/empty).
static uint8_t* __cdecl ReplaceCharactersInBuffer(uint8_t* buffer, char fromChar, char toChar) {
    uint8_t* cursor = buffer;
    if (buffer && *buffer) {
        do {
            if (*cursor == static_cast<uint8_t>(fromChar)) {
                *cursor = static_cast<uint8_t>(toChar);
            }
        } while (*++cursor);
    }
    return cursor;
}

HOOK(0x00694BF0, ReplaceCharactersInBuffer, "auto");
