#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0,
         (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));

// Copies a 0x5C-byte record (two embedded strings followed by nine dwords) from source
// into a freshly constructed object at self. Returns self (__thiscall, one stack arg).
static char* __fastcall GfxCollisionShapes_CopyConstructRecord(char* self, void* /*edx*/, std::uint32_t* source)
{
    char* source_bytes = reinterpret_cast<char*>(source);

    // First the header dword is copied.
    CSF_AT(std::uint32_t, self, 0x00) = source[0];

    // First embedded string (at self+4): empty, capacity 15.
    CSF_AT(std::uint32_t, self, 0x1C) = 15;
    CSF_AT(std::uint32_t, self, 0x18) = 0;
    CSF_AT(std::uint8_t, self, 0x08) = 0;
    AssignStringSubstring(self + 4, nullptr, source_bytes + 4, 0, 0xFFFFFFFFu);

    // Second embedded string (at self+0x20): empty, capacity 15.
    CSF_AT(std::uint32_t, self, 0x38) = 15;
    CSF_AT(std::uint32_t, self, 0x34) = 0;
    CSF_AT(std::uint8_t, self, 0x24) = 0;
    AssignStringSubstring(self + 0x20, nullptr, source_bytes + 0x20, 0, 0xFFFFFFFFu);

    // Nine dwords copied verbatim: source[15..23] -> self[15..23].
    CSF_AT(std::uint32_t, self, 0x3C) = source[15];
    CSF_AT(std::uint32_t, self, 0x40) = source[16];
    CSF_AT(std::uint32_t, self, 0x44) = source[17];
    CSF_AT(std::uint32_t, self, 0x48) = source[18];
    CSF_AT(std::uint32_t, self, 0x4C) = source[19];
    CSF_AT(std::uint32_t, self, 0x50) = source[20];
    CSF_AT(std::uint32_t, self, 0x54) = source[21];
    CSF_AT(std::uint32_t, self, 0x58) = source[22];
    CSF_AT(std::uint32_t, self, 0x5C) = source[23];

    return self;
}

HOOK(0x006CE1E0, GfxCollisionShapes_CopyConstructRecord, "auto");
