#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Applies the GfxObject record-range routine to one freshly claimed 8-byte slot (cdecl, 5 args, ret 0).
CSF_FUNC(void, __cdecl, GfxObject_ApplyToRecordRange,
         0x006B3E10,
         (int firstRecord, std::uint32_t recordCount, const void** context, int, int));

// Grows the storage (reallocating/copying) and inserts copies (__thiscall, ret 12).
CSF_FUNC(int, __fastcall, SkinListInsertRange,
         0x006B4F70,
         (void* self, void* edx, void** a1, unsigned int a3, void** arg8));

// Appends one 8-byte record to the vector-like container (__thiscall, ret 4).
static int __fastcall SkinList_AppendRecord8(
    void* self, void* /*edx*/, void** source)
{
    const std::uint32_t begin = CSF_AT(std::uint32_t, self, 4);

    const std::uint32_t elementCount = begin
        ? static_cast<std::uint32_t>(
              static_cast<std::int32_t>(
                  CSF_AT(std::uint32_t, self, 8) - begin) >> 3)
        : 0;

    if (!begin ||
        elementCount >= static_cast<std::uint32_t>(
            static_cast<std::int32_t>(
                CSF_AT(std::uint32_t, self, 12) - begin) >> 3))
    {
        const unsigned int insertResult = SkinListInsertRange(
            self, nullptr,
            reinterpret_cast<void**>(CSF_AT(std::uint32_t, self, 8)),
            1,
            source);
        return static_cast<int>(insertResult);
    }

    const std::uint32_t insertionAddress = CSF_AT(std::uint32_t, self, 8);

    GfxObject_ApplyToRecordRange(
        static_cast<int>(insertionAddress),
        1,
        const_cast<const void**>(source),
        static_cast<int>(reinterpret_cast<std::uintptr_t>(self)),
        static_cast<int>(reinterpret_cast<std::uintptr_t>(source)));

    CSF_AT(std::uint32_t, self, 8) = insertionAddress + 8;
    return 0;
}

HOOK(0x006B5460, SkinList_AppendRecord8, "auto");
