#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "legacy/string.h"

CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(int*, __fastcall, AnimationDefs_CopySubrecord, 0x00772D20, (void* self, void* edx, int a2));

// Copies an animation definition record from source (a2) into self, returns self.
static char* __fastcall AnimationDefs_CopyConstruct(char* self, void* /*edx*/, const char* src)
{
    // Leading dword copied verbatim.
    CSF_AT(std::uint32_t, self, 0x00) = CSF_AT(std::uint32_t, src, 0x00);

    // First name string at self+4: reset to empty (capacity 15), then assign substring of src+4.
    CSF_AT(std::uint32_t, self, 0x1C) = 0xF;
    CSF_AT(std::uint32_t, self, 0x18) = 0;
    CSF_AT(std::uint8_t, self, 0x08) = 0;
    AssignStringSubstring(self + 4, nullptr, src + 4, 0, 0xFFFFFFFFu);

    // Sub-record at self+0x20 copied from src+0x20.
    AnimationDefs_CopySubrecord(self + 0x20, nullptr,
        static_cast<int>(reinterpret_cast<std::uintptr_t>(src + 0x20)));

    CSF_AT(std::uint32_t, self, 0x30) = CSF_AT(std::uint32_t, src, 0x30);
    CSF_AT(std::uint32_t, self, 0x34) = CSF_AT(std::uint32_t, src, 0x34);

    // Second name string at self+0x38.
    CSF_AT(std::uint32_t, self, 0x50) = 0xF;
    CSF_AT(std::uint32_t, self, 0x4C) = 0;
    CSF_AT(std::uint8_t, self, 0x3C) = 0;
    AssignStringSubstring(self + 0x38, nullptr, src + 0x38, 0, 0xFFFFFFFFu);

    CSF_AT(std::uint32_t, self, 0x54) = CSF_AT(std::uint32_t, src, 0x54);

    // Six flag/parameter bytes at 0x58..0x5D.
    CSF_AT(std::uint8_t, self, 0x58) = CSF_AT(std::uint8_t, src, 0x58);
    CSF_AT(std::uint8_t, self, 0x59) = CSF_AT(std::uint8_t, src, 0x59);
    CSF_AT(std::uint8_t, self, 0x5A) = CSF_AT(std::uint8_t, src, 0x5A);
    CSF_AT(std::uint8_t, self, 0x5B) = CSF_AT(std::uint8_t, src, 0x5B);
    CSF_AT(std::uint8_t, self, 0x5C) = CSF_AT(std::uint8_t, src, 0x5C);
    CSF_AT(std::uint8_t, self, 0x5D) = CSF_AT(std::uint8_t, src, 0x5D);

    return self;
}

HOOK(0x773160, AnimationDefs_CopyConstruct, "auto");
