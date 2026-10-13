#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(char, __fastcall, GfxFrame_InitRotationDegreesFromMatrix, 0x006BF380, (void* self, void* /*edx*/, int matrixBase));
CSF_FUNC(int, __cdecl, CoreMemory_AllocUntagged, 0x006910F0, (std::size_t size));

// __thiscall bool-style initializer: copies a name string into a heap buffer and stores a value.
// Original: ret 12 (three stack args: a2, a3, a4).
static char __fastcall C_00861dd8_InitializeFromFrame(void* self, void* /*edx*/, int a2, const char* a3, int a4)
{
    if (!a2 || !a3 || !*a3)
        return 0;

    GfxFrame_InitRotationDegreesFromMatrix(CSF_AT(void*, self, 0xC), nullptr, a2);

    // Length of the string (no terminator), then allocate len + 1 bytes.
    std::size_t length = 0;
    while (a3[length] != '\0')
        ++length;

    char* buffer = reinterpret_cast<char*>(static_cast<std::uintptr_t>(CoreMemory_AllocUntagged(length + 1)));
    CSF_AT(uint32_t, self, 0x8) = static_cast<uint32_t>(reinterpret_cast<std::uintptr_t>(buffer));

    // Copy including the terminating null.
    for (std::size_t i = 0;; ++i) {
        buffer[i] = a3[i];
        if (a3[i] == '\0')
            break;
    }

    CSF_AT(int, self, 0x4) = a4;
    return 1;
}

HOOK(0x006E7DA0, C_00861dd8_InitializeFromFrame, "auto");
