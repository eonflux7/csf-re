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

// Compares a 16-bit string stored in the object against a narrow char string.
// Short strings (capacity < 8) are stored inline at +0x004; longer ones are
// referenced through the pointer at +0x004. Returns the first character
// difference, or -1 / 0 / 1 once the narrow string ends.
static int __fastcall VariantValues_CompareWideStringToNarrow(S_00696af0* self, void* /*edx*/, char* text) {
    int result = 0;

    uint16_t* wideCursor;
    if (self->field_0018 < 8) {
        wideCursor = reinterpret_cast<uint16_t*>(&self->field_0004);
    } else {
        wideCursor = reinterpret_cast<uint16_t*>(self->field_0004);
    }

    while (true) {
        uint16_t wideChar = *wideCursor;
        if (wideChar == 0 || *text == 0) {
            break;
        }
        char narrowChar = *text;
        result = static_cast<int>(wideChar) - static_cast<int>(narrowChar);
        ++wideCursor;
        ++text;
        if (result != 0) {
            return result;
        }
    }

    if (result == 0) {
        int wideNext = *wideCursor;
        int narrowNext = *text;
        if (wideNext <= narrowNext) {
            if (wideNext < narrowNext) {
                return -1;
            }
        } else {
            return 1;
        }
    }
    return result;
}

HOOK(0x696AF0, VariantValues_CompareWideStringToNarrow, "auto");
