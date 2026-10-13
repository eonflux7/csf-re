#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __fastcall, GfxObjectProps_SetOrClearFlagBits, 0x0040ED00, (void* self, void* , int bitMask, char enable));
CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(void*, __fastcall, AssignStringRange_488830, 0x00488830, (void* self, void* edx, void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(void*, __fastcall, C_0082d410_AssignStringFromSource, 0x0069D8E0, (void* self, void* edx, void* a2));
CSF_FUNC(int, __fastcall, CoreErrorLog_AssignEmbeddedString, 0x0069D900, (void* self, void* edx, int a2));

static char __fastcall CoreErrorLog_AssignPropertyValue(
    void* self, void* /*edx*/, void* source)
{
    const std::uint32_t type = CSF_AT(std::uint32_t, self, 0xC) & 0xDFFFFFFF;

    switch (type)
    {
    case 2:
    case 4:
    {
        void* destination = CSF_AT(void*, self, 0x4);
        const char value = CSF_AT(char, source, 0);
        CSF_AT(char, destination, 0) = value;
        return value;
    }

    case 8:
    case 16:
    case 32:
    case 2048:
    case 4096:
    case 0x4000:
    case 0x8000:
    case 0x20000:
    case 0x40000:
    case 0x80000:
    case 0x100000:
    case 0x1000000:
    {
        const std::uint32_t value = CSF_AT(std::uint32_t, source, 0);
        void* destination = CSF_AT(void*, self, 0x4);
        CSF_AT(std::uint32_t, destination, 0) = value;
        return static_cast<char>(value);
    }

    case 64:
    case 0x2000000:
        return static_cast<char>(reinterpret_cast<std::uintptr_t>(
            AssignStringSubstring(CSF_AT(void*, self, 0x4), nullptr,
                                  source, 0, 0xFFFFFFFF)));

    case 128:
    {
        const std::uint32_t first = CSF_AT(std::uint32_t, source, 0);
        void* destination = CSF_AT(void*, self, 0x4);
        CSF_AT(std::uint32_t, destination, 0) = first;
        const std::uint32_t second = CSF_AT(std::uint32_t, source, 0x4);
        CSF_AT(std::uint32_t, destination, 0x4) = second;
        return static_cast<char>(second);
    }

    case 256:
    {
        void* destination = CSF_AT(void*, self, 0x4);
        CSF_AT(std::uint32_t, destination, 0) = CSF_AT(std::uint32_t, source, 0);
        CSF_AT(std::uint32_t, destination, 0x4) = CSF_AT(std::uint32_t, source, 0x4);
        CSF_AT(std::uint32_t, destination, 0x8) = CSF_AT(std::uint32_t, source, 0x8);
        return static_cast<char>(reinterpret_cast<std::uintptr_t>(destination));
    }

    case 512:
    case 1024:
        return static_cast<char>(CoreErrorLog_AssignEmbeddedString(
            CSF_AT(void*, self, 0x4), nullptr,
            static_cast<int>(reinterpret_cast<std::uintptr_t>(source))));

    case 0x2000:
    {
        void* destination = CSF_AT(void*, self, 0x4);
        CSF_AT(std::uint32_t, destination, 0) = CSF_AT(std::uint32_t, source, 0);
        CSF_AT(std::uint32_t, destination, 0x4) = CSF_AT(std::uint32_t, source, 0x4);
        CSF_AT(std::uint32_t, destination, 0x8) = CSF_AT(std::uint32_t, source, 0x8);
        CSF_AT(std::uint32_t, destination, 0xC) = CSF_AT(std::uint32_t, source, 0xC);
        return static_cast<char>(reinterpret_cast<std::uintptr_t>(destination));
    }

    case 0x10000:
    {
        void* destination = CSF_AT(void*, self, 0x4);
        CSF_AT(std::uint32_t, destination, 0) = CSF_AT(std::uint32_t, source, 0);
        CSF_AT(std::uint32_t, destination, 0x4) = CSF_AT(std::uint32_t, source, 0x4);
        CSF_AT(std::uint32_t, destination, 0x8) = CSF_AT(std::uint32_t, source, 0x8);
        const std::uint32_t last = CSF_AT(std::uint32_t, source, 0xC);
        CSF_AT(std::uint32_t, destination, 0xC) = last;
        return static_cast<char>(last);
    }

    case 0x200000:
        return static_cast<char>(reinterpret_cast<std::uintptr_t>(
            C_0082d410_AssignStringFromSource(
                CSF_AT(void*, self, 0x4), nullptr, source)));

    case 0x400000:
    {
        const char enable = CSF_AT(char, source, 0);
        const int bitMask = CSF_AT(int, self, 0x8);
        void* destination = CSF_AT(void*, self, 0x4);
        return static_cast<char>(
            GfxObjectProps_SetOrClearFlagBits(destination, nullptr, bitMask, enable));
    }

    case 0x800000:
        return static_cast<char>(reinterpret_cast<std::uintptr_t>(
            AssignStringRange_488830(CSF_AT(void*, self, 0x4), nullptr,
                                    source, 0, 0xFFFFFFFF)));

    default:
        return static_cast<char>(type <= 0x80 ? type - 2 : type);
    }
}

HOOK(0x0069D930, CoreErrorLog_AssignPropertyValue, "auto");
