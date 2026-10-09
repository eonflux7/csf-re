#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(int, __fastcall, CopyStructuredMember, 0x004A40E0, (void* self, void* edx, void* a5));

static int __fastcall C_0082d410_Assign(
    C_0082d410* self, void* /*edx*/, C_0082d410* source)
{
    auto* destinationBytes = reinterpret_cast<std::uint8_t*>(self);
    auto* sourceBytes = reinterpret_cast<std::uint8_t*>(source);

    AssignStringSubstring(self, nullptr, source, 0, -1);
    AssignStringSubstring(destinationBytes + 28, nullptr, sourceBytes + 28, 0, -1);
    AssignStringSubstring(destinationBytes + 56, nullptr, sourceBytes + 56, 0, -1);
    AssignStringSubstring(destinationBytes + 84, nullptr, sourceBytes + 84, 0, -1);

    CSF_AT(std::uint32_t, self, 0x70) = CSF_AT(std::uint32_t, source, 0x70);
    CSF_AT(std::uint32_t, self, 0x74) = CSF_AT(std::uint32_t, source, 0x74);
    CSF_AT(std::uint32_t, self, 0x78) = CSF_AT(std::uint32_t, source, 0x78);
    CSF_AT(std::uint32_t, self, 0x7C) = CSF_AT(std::uint32_t, source, 0x7C);
    CSF_AT(std::uint32_t, self, 0x80) = CSF_AT(std::uint32_t, source, 0x80);

    AssignStringSubstring(destinationBytes + 132, nullptr, sourceBytes + 132, 0, -1);
    AssignStringSubstring(destinationBytes + 160, nullptr, sourceBytes + 160, 0, -1);

    CSF_AT(std::uint32_t, self, 0xBC) = CSF_AT(std::uint32_t, source, 0xBC);
    CSF_AT(std::uint32_t, self, 0xC0) = CSF_AT(std::uint32_t, source, 0xC0);
    CSF_AT(std::uint32_t, self, 0xC4) = CSF_AT(std::uint32_t, source, 0xC4);
    CSF_AT(std::uint32_t, self, 0xC8) = CSF_AT(std::uint32_t, source, 0xC8);
    CSF_AT(std::uint32_t, self, 0xCC) = CSF_AT(std::uint32_t, source, 0xCC);
    CSF_AT(std::uint32_t, self, 0xD0) = CSF_AT(std::uint32_t, source, 0xD0);
    CSF_AT(std::uint32_t, self, 0xD4) = CSF_AT(std::uint32_t, source, 0xD4);
    CSF_AT(std::uint32_t, self, 0xD8) = CSF_AT(std::uint32_t, source, 0xD8);
    CSF_AT(std::uint32_t, self, 0xDC) = CSF_AT(std::uint32_t, source, 0xDC);
    CSF_AT(std::uint8_t, self, 0xE0) = CSF_AT(std::uint8_t, source, 0xE0);

    CSF_AT(std::uint32_t, self, 0xE4) = CSF_AT(std::uint32_t, source, 0xE4);
    CSF_AT(std::uint32_t, self, 0xE8) = CSF_AT(std::uint32_t, source, 0xE8);
    CSF_AT(std::uint32_t, self, 0xEC) = CSF_AT(std::uint32_t, source, 0xEC);
    CSF_AT(std::uint32_t, self, 0xF0) = CSF_AT(std::uint32_t, source, 0xF0);
    CSF_AT(std::uint32_t, self, 0xF4) = CSF_AT(std::uint32_t, source, 0xF4);
    CSF_AT(std::uint32_t, self, 0xF8) = CSF_AT(std::uint32_t, source, 0xF8);
    CSF_AT(std::uint32_t, self, 0xFC) = CSF_AT(std::uint32_t, source, 0xFC);
    CSF_AT(std::uint32_t, self, 0x100) = CSF_AT(std::uint32_t, source, 0x100);
    CSF_AT(std::uint32_t, self, 0x104) = CSF_AT(std::uint32_t, source, 0x104);
    CSF_AT(std::uint32_t, self, 0x108) = CSF_AT(std::uint32_t, source, 0x108);
    CSF_AT(std::uint32_t, self, 0x10C) = CSF_AT(std::uint32_t, source, 0x10C);
    CSF_AT(std::uint32_t, self, 0x110) = CSF_AT(std::uint32_t, source, 0x110);
    CSF_AT(std::uint32_t, self, 0x114) = CSF_AT(std::uint32_t, source, 0x114);
    CSF_AT(std::uint32_t, self, 0x118) = CSF_AT(std::uint32_t, source, 0x118);
    CSF_AT(std::uint32_t, self, 0x11C) = CSF_AT(std::uint32_t, source, 0x11C);
    CSF_AT(std::uint32_t, self, 0x120) = CSF_AT(std::uint32_t, source, 0x120);
    CSF_AT(std::uint32_t, self, 0x124) = CSF_AT(std::uint32_t, source, 0x124);
    CSF_AT(std::uint32_t, self, 0x128) = CSF_AT(std::uint32_t, source, 0x128);
    CSF_AT(std::uint32_t, self, 0x12C) = CSF_AT(std::uint32_t, source, 0x12C);
    CSF_AT(std::uint32_t, self, 0x130) = CSF_AT(std::uint32_t, source, 0x130);
    CSF_AT(std::uint32_t, self, 0x134) = CSF_AT(std::uint32_t, source, 0x134);
    CSF_AT(std::uint32_t, self, 0x138) = CSF_AT(std::uint32_t, source, 0x138);
    CSF_AT(std::uint32_t, self, 0x13C) = CSF_AT(std::uint32_t, source, 0x13C);
    CSF_AT(std::uint32_t, self, 0x140) = CSF_AT(std::uint32_t, source, 0x140);
    CSF_AT(std::uint32_t, self, 0x144) = CSF_AT(std::uint32_t, source, 0x144);
    CSF_AT(std::uint32_t, self, 0x148) = CSF_AT(std::uint32_t, source, 0x148);
    CSF_AT(std::uint32_t, self, 0x14C) = CSF_AT(std::uint32_t, source, 0x14C);

    CopyStructuredMember(destinationBytes + 336, nullptr, sourceBytes + 336);
    CopyStructuredMember(destinationBytes + 352, nullptr, sourceBytes + 352);
    CopyStructuredMember(destinationBytes + 368, nullptr, sourceBytes + 368);
    CopyStructuredMember(destinationBytes + 384, nullptr, sourceBytes + 384);
    CopyStructuredMember(destinationBytes + 400, nullptr, sourceBytes + 400);

    CSF_AT(std::uint32_t, self, 0x1A0) = CSF_AT(std::uint32_t, source, 0x1A0);
    CSF_AT(std::uint32_t, self, 0x1A4) = CSF_AT(std::uint32_t, source, 0x1A4);
    CSF_AT(std::uint32_t, self, 0x1A8) = CSF_AT(std::uint32_t, source, 0x1A8);
    CSF_AT(std::uint32_t, self, 0x1AC) = CSF_AT(std::uint32_t, source, 0x1AC);
    CSF_AT(std::uint32_t, self, 0x1B0) = CSF_AT(std::uint32_t, source, 0x1B0);
    CSF_AT(std::uint32_t, self, 0x1B4) = CSF_AT(std::uint32_t, source, 0x1B4);
    CSF_AT(std::uint32_t, self, 0x1B8) = CSF_AT(std::uint32_t, source, 0x1B8);
    CSF_AT(std::uint32_t, self, 0x1BC) = CSF_AT(std::uint32_t, source, 0x1BC);
    CSF_AT(std::uint32_t, self, 0x1C0) = CSF_AT(std::uint32_t, source, 0x1C0);
    CSF_AT(std::uint32_t, self, 0x1C4) = CSF_AT(std::uint32_t, source, 0x1C4);
    CSF_AT(std::uint32_t, self, 0x1C8) = CSF_AT(std::uint32_t, source, 0x1C8);
    CSF_AT(std::uint32_t, self, 0x1CC) = CSF_AT(std::uint32_t, source, 0x1CC);
    CSF_AT(std::uint32_t, self, 0x1D0) = CSF_AT(std::uint32_t, source, 0x1D0);

    return reinterpret_cast<int>(self);
}

HOOK(0x00783B50, C_0082d410_Assign, "auto");
