#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, sub_4690A0, 0x004690A0, (void* self, void* edx, int a2));
CSF_FUNC(uint32_t*, __fastcall, sub_471130, 0x00471130, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Detaches and destroys every entry whose +0x10 field equals ownerBlock, then
// frees the owner block itself through the game heap.
static void __fastcall S_0045d610_RemoveEntriesByOwnerAndFree(S_0045d610* self, void* /*edx*/, int ownerBlock)
{
    uint32_t entryCount = self->field_0030;

    for (uint32_t i = 0; i < entryCount; ++i)
    {
        uint32_t entry = reinterpret_cast<uint32_t*>(self->field_0028)[i];
        if (entry == 0)
            continue;

        void* entryObj = reinterpret_cast<void*>(entry);
        if (CSF_AT(int, entryObj, 0x10) != ownerBlock)
            continue;

        sub_4690A0(self, nullptr, static_cast<int>(entry));

        uint32_t current = reinterpret_cast<uint32_t*>(self->field_0028)[i];
        if (current != 0)
        {
            void* currentObj = reinterpret_cast<void*>(current);
            // Virtual slot 0 is the deleting destructor; flag 1 = free storage.
            auto deleter = reinterpret_cast<void (__fastcall*)(void*, void*, int)>(CSF_AT(void**, currentObj, 0)[0]);
            deleter(currentObj, nullptr, 1);
            reinterpret_cast<uint32_t*>(self->field_0028)[i] = 0;
        }
    }

    if (ownerBlock != 0)
    {
        sub_471130(reinterpret_cast<void*>(ownerBlock), nullptr);
        Mem_Free(reinterpret_cast<void*>(ownerBlock));
    }
}

HOOK(0x00469120, S_0045d610_RemoveEntriesByOwnerAndFree, "auto");
