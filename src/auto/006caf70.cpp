#include <cstring>

#include "globals.h"
#include "hook.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Copies the array descriptor from `source` into `self`, reallocating the
// element buffer when `source` holds more slots than `self` currently has.
// Layout (both objects): +0 element pointer, +4 capacity, +8 count, +12 extra.
static void* __fastcall CharacterDefs_AssignEmbeddedMember(void* self, void* /*edx*/, void* source)
{
    if (CSF_AT(int, source, 0x8) > CSF_AT(int, self, 0x4))
    {
        void* newBlock = Mem_Alloc(4 * CSF_AT(int, source, 0x4));
        void* oldBlock = CSF_AT(void*, self, 0x0);
        CSF_AT(int, self, 0x4) = CSF_AT(int, source, 0x4);
        if (oldBlock != nullptr)
        {
            Mem_Free(oldBlock);
            CSF_AT(void*, self, 0x0) = nullptr;
        }
        CSF_AT(void*, self, 0x0) = newBlock;
    }

    int count = CSF_AT(int, source, 0x8);
    if (count != 0)
    {
        std::memcpy(CSF_AT(void*, self, 0x0), CSF_AT(const void*, source, 0x0), 4 * count);
    }

    CSF_AT(int, self, 0x8) = CSF_AT(int, source, 0x8);
    CSF_AT(int, self, 0xC) = CSF_AT(int, source, 0xC);
    return self;
}

HOOK(0x006CAF70, CharacterDefs_AssignEmbeddedMember, "auto");
