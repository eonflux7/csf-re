#include <cstddef>
#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Entry layout: 16 bytes = three zero dwords followed by 0xBF800000 (-1.0f).
// Object layout: +0x0 entry buffer pointer, +0x4 entry capacity, +0x8 used entry count.
static int __fastcall ExpandQuadEntryArrayNegOne(void* self, void* /*edx*/, int extraEntries)
{
    int entryCount = extraEntries + CSF_AT(int, self, 0x4);
    void* newBuffer = Mem_Alloc(static_cast<std::size_t>(entryCount) * 16);
    int result = 0;

    if (newBuffer != nullptr) {
        if (entryCount - 1 >= 0) {
            uint32_t* cursor = static_cast<uint32_t*>(newBuffer) + 2;
            int remaining = entryCount;
            do {
                cursor[-2] = 0;
                cursor[-1] = 0;
                cursor[0] = 0;
                cursor[1] = 0xBF800000;
                cursor += 4;
                --remaining;
            } while (remaining != 0);
        }
    }

    int usedCount = CSF_AT(int, self, 0x8);
    if (usedCount != 0) {
        std::memcpy(newBuffer, CSF_AT(const void*, self, 0x0),
                    static_cast<std::size_t>(usedCount) * 16);
        result = 0;
    }

    void* oldBuffer = CSF_AT(void*, self, 0x0);
    bool oldBufferWasNull = (oldBuffer == nullptr);
    CSF_AT(int, self, 0x4) += extraEntries;
    if (!oldBufferWasNull) {
        result = Mem_Free(oldBuffer);
        CSF_AT(void*, self, 0x0) = nullptr;
    }
    CSF_AT(void*, self, 0x0) = newBuffer;
    return result;
}

HOOK(0x0040AD70, ExpandQuadEntryArrayNegOne, "auto");
