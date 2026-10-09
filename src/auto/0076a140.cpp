#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Copies one dword into the slot at `slot` (0x768FA0, __cdecl, pops 0).
CSF_FUNC(uint32_t*, __cdecl, CopyDwordIntoSlot_768FA0, 0x00768FA0,
         (uint32_t* a1, int a2, uint32_t* a3, int a4, int a5));

// Grows the vector-like container and inserts elements at `position`
// (0x769D60, __thiscall, pops 12 stack bytes).
CSF_FUNC(char, __fastcall, InsertDwordElementsAt_769D60, 0x00769D60,
         (void* self, void* edx, uint32_t* Src, unsigned int a3, uint32_t* a4));

// Appends one dword to the buffer owned by `self` (0x76A140, __thiscall,
// pops 4 stack bytes). Returns the low byte of the copier's result, or the
// result of the grow/insert path.
static char __fastcall AppendBufferedDword(S_0076a140* self, void* /*edx*/,
                                           uint32_t* sourceValue)
{
    const uint32_t begin = self->field_0004;
    unsigned int size = 0;

    if (begin != 0) {
        size = static_cast<unsigned int>(
            static_cast<int>(self->field_0008 - begin) >> 2);
    }

    if (begin == 0 ||
        size >= static_cast<unsigned int>(
                    static_cast<int>(self->field_000c - begin) >> 2)) {
        return InsertDwordElementsAt_769D60(
            self, nullptr,
            reinterpret_cast<uint32_t*>(
                static_cast<std::uintptr_t>(self->field_0008)),
            1,
            sourceValue);
    }

    uint32_t* slot = reinterpret_cast<uint32_t*>(
        static_cast<std::uintptr_t>(self->field_0008));

    uint32_t* copyResult = CopyDwordIntoSlot_768FA0(
        slot, 1, sourceValue,
        static_cast<int>(reinterpret_cast<std::uintptr_t>(self)),
        0);

    self->field_0008 = static_cast<uint32_t>(
        reinterpret_cast<std::uintptr_t>(slot + 1));

    return static_cast<char>(reinterpret_cast<std::uintptr_t>(copyResult));
}

HOOK(0x0076A140, AppendBufferedDword, "auto");
