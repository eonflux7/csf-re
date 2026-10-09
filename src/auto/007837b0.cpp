#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(int, __fastcall, WeaponDefs_CopyTwelveByteVector, 0x0054D9D0, (void* self, void* edx, int a2));

static void InitializeAndCopyString(C_0082d410* destination, const void* source)
{
    CSF_AT(std::uint32_t, destination, 0x18) = 15;
    CSF_AT(std::uint32_t, destination, 0x14) = 0;
    *reinterpret_cast<std::uint8_t*>(&destination->field_0004) = 0;
    AssignStringSubstring(destination, nullptr, source, 0,
                          static_cast<std::uint32_t>(-1));
}

static int __fastcall C_0082d410_CopyConstruct(
    C_0082d410* self, void* /*edx*/, int sourceAddress)
{
    auto* destination = reinterpret_cast<std::uint8_t*>(self);
    const auto* source = reinterpret_cast<const std::uint8_t*>(sourceAddress);

    InitializeAndCopyString(self, source);
    InitializeAndCopyString(
        reinterpret_cast<C_0082d410*>(destination + 28), source + 28);
    InitializeAndCopyString(
        reinterpret_cast<C_0082d410*>(destination + 56), source + 56);
    InitializeAndCopyString(
        reinterpret_cast<C_0082d410*>(destination + 84), source + 84);

    CSF_AT(std::uint32_t, self, 0x70) = CSF_AT(std::uint32_t, source, 0x70);
    CSF_AT(std::uint32_t, self, 0x74) = CSF_AT(std::uint32_t, source, 0x74);
    CSF_AT(std::uint32_t, self, 0x78) = CSF_AT(std::uint32_t, source, 0x78);
    CSF_AT(std::uint32_t, self, 0x7C) = CSF_AT(std::uint32_t, source, 0x7C);
    CSF_AT(std::uint32_t, self, 0x80) = CSF_AT(std::uint32_t, source, 0x80);

    InitializeAndCopyString(
        reinterpret_cast<C_0082d410*>(destination + 132), source + 132);
    InitializeAndCopyString(
        reinterpret_cast<C_0082d410*>(destination + 160), source + 160);

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

    WeaponDefs_CopyTwelveByteVector(
        destination + 336, nullptr, reinterpret_cast<int>(source + 336));
    WeaponDefs_CopyTwelveByteVector(
        destination + 352, nullptr, reinterpret_cast<int>(source + 352));
    WeaponDefs_CopyTwelveByteVector(
        destination + 368, nullptr, reinterpret_cast<int>(source + 368));
    WeaponDefs_CopyTwelveByteVector(
        destination + 384, nullptr, reinterpret_cast<int>(source + 384));
    WeaponDefs_CopyTwelveByteVector(
        destination + 400, nullptr, reinterpret_cast<int>(source + 400));

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

HOOK(0x007837B0, C_0082d410_CopyConstruct, "auto");
