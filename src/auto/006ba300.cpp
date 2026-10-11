#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Initializes a run of 8-byte elements in raw storage (cdecl, 5 args, ret 0).
CSF_FUNC(int, __cdecl, GfxObjectProps_InitElementRange_6B8FF0,
         0x006B8FF0,
         (int a1, int a2, int a3, int a4, int a5));

// Grows the storage (reallocating/copying) and inserts copies (__thiscall, ret 12).
CSF_FUNC(char, __fastcall, GfxObjectProps_InsertElementCopies_6B99A0,
         0x006B99A0,
         (void* self, void* edx, int a1, unsigned int a3, uint32_t* arg8));

// Appends one 8-byte element to the vector-like container (__thiscall, ret 4).
static char __fastcall GfxObjectProps_AppendElement8(
    S_006b9580* self, void* /*edx*/, uint32_t* source)
{
    const uint32_t elementBegin = self->field_0004;

    const uint32_t elementCount = elementBegin
        ? static_cast<uint32_t>(
              static_cast<int32_t>(self->field_0008 - elementBegin) >> 3)
        : 0;

    if (!elementBegin ||
        elementCount >= static_cast<uint32_t>(
            static_cast<int32_t>(self->field_000c - elementBegin) >> 3))
    {
        return GfxObjectProps_InsertElementCopies_6B99A0(
            static_cast<void*>(self), nullptr,
            static_cast<int>(self->field_0008),
            1,
            source);
    }

    const uint32_t insertionAddress = self->field_0008;

    const int initResult = GfxObjectProps_InitElementRange_6B8FF0(
        static_cast<int>(insertionAddress),
        1,
        static_cast<int>(reinterpret_cast<std::uintptr_t>(source)),
        static_cast<int>(reinterpret_cast<std::uintptr_t>(self)),
        static_cast<int>(reinterpret_cast<std::uintptr_t>(source)));

    const char result = static_cast<char>(initResult);

    self->field_0008 = insertionAddress + 8;
    return result;
}

HOOK(0x006BA300, GfxObjectProps_AppendElement8, "auto");
