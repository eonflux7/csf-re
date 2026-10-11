#include <cstdint>
#include "globals.h"
#include <cstddef>
#include <cstring>
#include "hook.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Container layout as used here: +0 data pointer, +4 entry count, +8 used entries.
// Grows the buffer by extraEntries 8-byte entries, copies the used entries,
// frees the old buffer (if any), and installs the new one.
static void* __fastcall SpawnPoints_GrowEntryBuffer(void* self, void* /*edx*/, int extraEntries)
{
    void* newBuffer = Mem_Alloc(static_cast<std::size_t>(CSF_AT(int, self, 4) + extraEntries) * 8);
    int usedEntries = CSF_AT(int, self, 8);
    if (usedEntries != 0) {
        std::memcpy(newBuffer, CSF_AT(void*, self, 0), static_cast<std::size_t>(usedEntries) * 8);
    }

    void* oldBuffer = CSF_AT(void*, self, 0);
    CSF_AT(int, self, 4) += extraEntries;

    if (oldBuffer == nullptr) {
        CSF_AT(void*, self, 0) = newBuffer;
        return newBuffer;
    }

    int freeResult = Mem_Free(oldBuffer);
    CSF_AT(void*, self, 0) = nullptr;
    CSF_AT(void*, self, 0) = newBuffer;
    return reinterpret_cast<void*>(static_cast<std::uintptr_t>(freeResult));
}

HOOK(0x551CF0, SpawnPoints_GrowEntryBuffer, "auto");
