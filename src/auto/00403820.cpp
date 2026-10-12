#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Inline small-buffer capacity threshold: a heap buffer exists once capacity >= 16.
static constexpr uint32_t kInlineCapacity = 15;
static constexpr uint32_t kHeapThreshold = 0x10;

// Sets the string length to a3 and null-terminates it. If a2 is set and the string
// currently lives in a heap buffer (capacity >= 16), the data is copied back into the
// inline buffer at +4 and the heap block is freed.
static unsigned int __fastcall StringBuffer_SetLengthMoveInline(void* self, void* /*edx*/, char a2, uint32_t a3)
{
    if (a2 && CSF_AT(uint32_t, self, 0x18) >= kHeapThreshold)
    {
        void* heapBuffer = CSF_AT(void*, self, 0x04);
        if (a3)
            memcpy(reinterpret_cast<uint8_t*>(self) + 4, heapBuffer, a3);
        Mem_Free(heapBuffer);
    }

    CSF_AT(uint32_t, self, 0x18) = kInlineCapacity;
    CSF_AT(uint32_t, self, 0x14) = a3;
    reinterpret_cast<uint8_t*>(self)[a3 + 4] = 0;
    return a3;
}

HOOK(0x00403820, StringBuffer_SetLengthMoveInline, "auto");
