#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Initializes a run of 32-byte _String_base elements in raw storage (cdecl, 5 args, ret 0).
CSF_FUNC(int, __cdecl, CoreFiles_InitStringBaseRange_697C50,
         0x00697C50,
         (int a1, int a2, void* a3, int a4, int a5));

// Grows the storage (reallocating/copying) and inserts copies (__thiscall, ret 12).
CSF_FUNC(char, __fastcall, CoreFiles_InsertStringBaseCopies_697D50,
         0x00697D50,
         (void* self, void* edx, void* a1, unsigned int a3, void* arg8));

// Appends one 32-byte std::_String_base element to the vector-like container (__thiscall, ret 4).
static int __fastcall CoreFiles_AppendStringBaseElement(
    void* self, void* /*edx*/, void* sourceString)
{
    const std::uint32_t begin = CSF_AT(std::uint32_t, self, 4);

    const std::uint32_t elementCount = begin
        ? static_cast<std::uint32_t>(
              static_cast<std::int32_t>(
                  CSF_AT(std::uint32_t, self, 8) - begin) >> 5)
        : 0;

    if (!begin ||
        elementCount >= static_cast<std::uint32_t>(
            static_cast<std::int32_t>(
                CSF_AT(std::uint32_t, self, 12) - begin) >> 5))
    {
        const char insertResult = CoreFiles_InsertStringBaseCopies_697D50(
            self, nullptr,
            reinterpret_cast<void*>(static_cast<std::uintptr_t>(CSF_AT(std::uint32_t, self, 8))),
            1,
            sourceString);
        return static_cast<int>(insertResult);
    }

    const std::uint32_t insertionAddress = CSF_AT(std::uint32_t, self, 8);

    const int result = CoreFiles_InitStringBaseRange_697C50(
        static_cast<int>(insertionAddress),
        1,
        sourceString,
        static_cast<int>(reinterpret_cast<std::uintptr_t>(self)),
        static_cast<int>(reinterpret_cast<std::uintptr_t>(sourceString)));

    CSF_AT(std::uint32_t, self, 8) = insertionAddress + 32;
    return result;
}

HOOK(0x00698200, CoreFiles_AppendStringBaseElement, "auto");
