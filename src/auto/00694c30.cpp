#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(char*, __cdecl, _strncpy, 0x007AFEC0, (char* dest, const char* src, size_t count));

// Returns the length of a NUL-terminated string (inline loop in the original, no CRT call).
static uint32_t GetNulTerminatedLength(const char* text) {
    uint32_t length = 0;
    while (text[length] != '\0') {
        ++length;
    }
    return length;
}

// Appends Source into the fixed-size buffer `buffer` (capacity `capacity`) after its
// current contents, copying at most the remaining space and forcing a NUL at the last byte.
// Returns the strncpy result when the copy happens, otherwise the current string length.
static char* __cdecl GfxTextureLoader_AppendResourceName(char* buffer, int capacity, const char* source) {
    uint32_t currentLength = GetNulTerminatedLength(buffer);
    char* result = reinterpret_cast<char*>(static_cast<uintptr_t>(currentLength));

    if (currentLength < static_cast<uint32_t>(capacity - 1)) {
        int remaining = capacity - static_cast<int>(currentLength);
        char* destination = buffer + currentLength;
        result = _strncpy(destination, source, static_cast<size_t>(remaining));
        destination[remaining - 1] = '\0';
    }
    return result;
}

HOOK(0x694C30, GfxTextureLoader_AppendResourceName, "auto");
