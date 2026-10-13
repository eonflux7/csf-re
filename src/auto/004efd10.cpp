#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(char, __fastcall, S_006b0320_ReleaseBuffers, 0x006B1010, (S_006b0320* self, void*));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

// Resets the text buffer owned by a CTextDatabase: releases the embedded
// S_006b0320 buffers, frees the heap block at +0x24 (if any), and, when the
// storage is in heap mode (capacity >= 16), frees the block at +0x08.
// Afterwards the object is back to its empty inline state (length 0, capacity 15).
static int __fastcall TextDatabase_ResetBuffers(uint8_t* self, void* /*edx*/)
{
    S_006b0320_ReleaseBuffers(reinterpret_cast<S_006b0320*>(self + 0x30), nullptr);

    int result = 0;
    void* primaryBlock = CSF_AT(void*, self, 0x24);
    if (primaryBlock != nullptr)
    {
        result = Mem_FreeDebug(primaryBlock);
    }
    CSF_AT(void*, self, 0x24) = nullptr;
    CSF_AT(int, self, 0x28) = 0;
    CSF_AT(int, self, 0x2C) = 0;

    if (CSF_AT(unsigned int, self, 0x1C) >= 0x10u)
    {
        result = 0;
        void* heapString = CSF_AT(void*, self, 8);
        if (heapString != nullptr)
        {
            result = Mem_FreeDebug(heapString);
        }
    }

    CSF_AT(int, self, 0x18) = 0;
    CSF_AT(int, self, 0x1C) = 15;
    CSF_AT(uint8_t, self, 8) = 0;
    return result;
}

HOOK(0x4EFD10, TextDatabase_ResetBuffers, "auto");
