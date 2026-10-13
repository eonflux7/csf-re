#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(unsigned int, __fastcall, C_0082e664_StoreAllocatedSlot, 0x0044DCE0, (void* self, void* edx, unsigned int a2));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));

// Pops a previously released 52-byte slot object from this pool's free list (field_0014 /
// field_001c). If the list is empty, allocates a new 0x34-byte object, initializes it, and
// stores it into the pool. Returns the slot pointer (0 if the allocation failed).
static unsigned int __fastcall C_008311f0_AcquireSlot(C_008311f0* self, void* /*edx*/)
{
    unsigned int freeCount = self->field_001c;
    unsigned int newSlot = 0;
    unsigned int resultSlot = 0;

    if (freeCount != 0) {
        uint32_t* freeList = reinterpret_cast<uint32_t*>(self->field_0014);
        newSlot = freeList[freeCount - 1];
        --self->field_001c;
    }
    resultSlot = newSlot;

    if (resultSlot == 0) {
        void* allocated = Mem_Alloc(52);
        unsigned int createdSlot = 0;
        if (allocated != nullptr) {
            uint8_t* obj = static_cast<uint8_t*>(allocated);
            CSF_AT(void*, obj, 0x00) = CSF_VTBL(0x0083124C);
            CSF_AT(int, obj, 0x0C) = -1;
            CSF_AT(int, obj, 0x10) = -1;
            CSF_AT(void*, obj, 0x14) = CSF_VTBL(0x0082D448);
            CSF_AT(int, obj, 0x18) = -1;
            CSF_AT(int, obj, 0x1C) = -1;
            CSF_AT(int, obj, 0x20) = 0;
            CSF_AT(int, obj, 0x24) = 0;
            CSF_AT(int, obj, 0x28) = 0;
            CSF_AT(int, obj, 0x30) = 0;
            CSF_AT(int, obj, 0x04) = -1;
            CSF_AT(int, obj, 0x08) = -1;
            CSF_AT(uint32_t, obj, 0x2C) = 0x7F7FFFFFu;
            createdSlot = static_cast<unsigned int>(reinterpret_cast<uintptr_t>(allocated));
        }
        C_0082e664_StoreAllocatedSlot(self, nullptr, createdSlot);
        return createdSlot;
    }
    return resultSlot;
}

HOOK(0x0044E580, C_008311f0_AcquireSlot, "auto");
