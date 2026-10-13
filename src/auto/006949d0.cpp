#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(char*, __cdecl, _strncpy, 0x007AFEC0, (char* dest, const char* src, size_t count));

// Splits a path into its base name (text after the last '/' or '\\', up to the last '.')
// and copies it into destination when it fits in capacity; otherwise copies the text before the last '.'.
// Returns the low byte of the last value the original leaves in eax.
static char __cdecl CoreString_NormalizeResourceName(char* source, char* destination, uint32_t capacity)
{
    // Inline strlen
    uint32_t length = 0;
    while (source[length] != '\0')
        ++length;

    uint8_t resultByte = static_cast<uint8_t>(length);
    uint32_t baseEnd = length;            // ebx: end of the name, moved back to a '.' when found
    uint32_t cursor = length - 1;         // esi: scan index, wraps to 0xFFFFFFFF when empty
    bool separatorFound = false;

    if (cursor != 0xFFFFFFFFu) {
        for (;;) {
            char ch = source[cursor];
            resultByte = static_cast<uint8_t>(ch);
            if (ch == '.') {
                baseEnd = cursor;
            } else if (ch == '/' || ch == '\\') {
                separatorFound = true;
                break;
            }
            if (--cursor == 0xFFFFFFFFu)
                break;
        }
    }

    if (separatorFound) {
        resultByte = static_cast<uint8_t>(cursor + 1);
        uint32_t nameStart = cursor + 1;
        uint32_t nameLength = baseEnd - nameStart;
        if (nameLength < capacity) {
            _strncpy(destination, source + nameStart, nameLength);
            resultByte = static_cast<uint8_t>(reinterpret_cast<uintptr_t>(destination));
            destination[nameLength] = '\0';
        }
        return static_cast<char>(resultByte);
    }

    if (baseEnd < capacity) {
        char* copyResult = _strncpy(destination, source, baseEnd);
        resultByte = static_cast<uint8_t>(reinterpret_cast<uintptr_t>(copyResult));
        destination[baseEnd] = '\0';
    }

    return static_cast<char>(resultByte);
}

HOOK(0x6949D0, CoreString_NormalizeResourceName, "auto");
