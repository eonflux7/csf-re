#include <cstdint>
#include "hook.h"

// Returns the part of a path after its last '/' or '\', or the whole string if neither occurs.
// The original computes strlen inline (no call), so the length scan is written out here.
static int __cdecl CoreString_GetFileNameComponent(const char* a1) {
    int result = static_cast<int>(reinterpret_cast<intptr_t>(a1));

    size_t length = 0;
    while (a1[length] != '\0') {
        ++length;
    }

    if (length != 0) {
        for (int index = static_cast<int>(length) - 1; index >= 0; --index) {
            const char current = a1[index];
            if (current == '/' || current == '\\') {
                return static_cast<int>(reinterpret_cast<intptr_t>(&a1[index + 1]));
            }
        }
    }
    return result;
}

HOOK(0x00694A60, CoreString_GetFileNameComponent, "auto");
