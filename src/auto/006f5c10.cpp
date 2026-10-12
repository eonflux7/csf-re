#include <cstdint>
#include "globals.h"
#include <cstddef>
#include <cstring>
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(void*, __cdecl, _memcpy, 0x007AEE60, (void* destination, const void* source, std::size_t byteCount));

// Finds the entry in this object's pointer table that equals *entryPtr, removes it
// from the table (shifting the tail down), frees the entry's block, and nulls the
// caller's pointer. Does nothing if the table is empty or the entry is not found.
static void __fastcall C_00862874_RemoveAndFreeEntry(C_00862874* self, void* /*edx*/, void** entryPtr) {
    uint32_t count = self->field_0010;
    uint32_t index = 0;
    if (count == 0) {
        return;
    }

    void* target = *entryPtr;
    void** entries = reinterpret_cast<void**>(self->field_0008);
    for (; entries[index] != target; ++index) {
        if (index + 1 >= count) {
            return;
        }
    }

    if (index != 0xFFFFFFFFu) {
        uint32_t newCount = self->field_0010 - 1;
        self->field_0010 = newCount;
        if (index != newCount) {
            void** table = reinterpret_cast<void**>(self->field_0008);
            _memcpy(static_cast<void*>(&table[index]),
                    static_cast<const void*>(&table[index + 1]),
                    static_cast<std::size_t>(4u * (newCount - index)));
        }

        void* entry = *entryPtr;
        if (entry) {
            Mem_Free(entry);
            *entryPtr = nullptr;
        }
        *entryPtr = nullptr;
    }
}

HOOK(0x006F5C10, C_00862874_RemoveAndFreeEntry, "auto");
