#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, TypeRegistry_InitElementRange8,
         0x0054B330,
         (int a1, int a2, int a3, int a4, int a5));

CSF_FUNC(unsigned int, __fastcall, TypeRegistry_InsertElementCopies8,
         0x0054B9F0,
         (void* self, void* edx, int a1, unsigned int a3, uint32_t* arg8));

static int __fastcall TypeRegistry_AppendElement8(
    void* self, void* /*edx*/, uint32_t* source)
{
    const std::uint32_t begin = CSF_AT(std::uint32_t, self, 0x04);
    const std::uint32_t elementCount = begin
        ? static_cast<std::uint32_t>(
              static_cast<std::int32_t>(
                  CSF_AT(std::uint32_t, self, 0x08) - begin) >> 3)
        : 0;

    if (!begin ||
        elementCount >= static_cast<std::uint32_t>(
            static_cast<std::int32_t>(
                CSF_AT(std::uint32_t, self, 0x0C) - begin) >> 3))
    {
        return static_cast<int>(TypeRegistry_InsertElementCopies8(
            self, nullptr,
            static_cast<int>(CSF_AT(std::uint32_t, self, 0x08)),
            1, source));
    }

    const std::uint32_t insertionAddress =
        CSF_AT(std::uint32_t, self, 0x08);

    const int result = TypeRegistry_InitElementRange8(
        static_cast<int>(insertionAddress),
        1,
        static_cast<int>(reinterpret_cast<std::uintptr_t>(source)),
        static_cast<int>(reinterpret_cast<std::uintptr_t>(self)),
        static_cast<int>(reinterpret_cast<std::uintptr_t>(source)));

    CSF_AT(std::uint32_t, self, 0x08) = insertionAddress + 8;
    return result;
}

HOOK(0x0054BE50, TypeRegistry_AppendElement8, "auto");
