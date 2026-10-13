#include <cstddef>
#include "globals.h"
#include <cstdint>

#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, CoreMemory_AllocUntagged, 0x006910F0, (std::size_t size));

// Returns a heap copy of the NUL-terminated string, or nullptr if the source is null or allocation fails.
static char* __cdecl DuplicateCStringUntagged(const char* source) {
    char* result = nullptr;
    if (source) {
        std::size_t length = 0;
        while (source[length] != '\0') {
            ++length;
        }

        result = reinterpret_cast<char*>(
            static_cast<std::uintptr_t>(CoreMemory_AllocUntagged(length + 1)));
        if (result) {
            std::size_t i = 0;
            do {
                result[i] = source[i];
            } while (source[i++] != '\0');
        }
    }
    return result;
}

HOOK(0x0069ADC0, DuplicateCStringUntagged, "auto");
