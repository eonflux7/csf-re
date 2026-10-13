#include <cstdint>
#include "globals.h"
#include "hook.h"

// Validates a table key character: letters and digits pass, 'A'..'Z' are
// lowercased in place. Returns false for any other character.
static bool __stdcall SortedTable_NormalizeAlnumChar(uint16_t* charPtr) {
    uint16_t ch = *charPtr;

    bool isLower = ch >= 0x61 && ch <= 0x7A;
    bool isUpper = ch >= 0x41 && ch <= 0x5A;
    bool isDigit = ch >= 0x30 && ch <= 0x39;

    if (!isLower && !isUpper && !isDigit) {
        return false;
    }

    if (isUpper) {
        *charPtr = static_cast<uint16_t>(ch + 0x20);
        return true;
    }

    *charPtr = ch;
    return true;
}

HOOK(0x698C50, SortedTable_NormalizeAlnumChar, "auto");
