#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, SetAllocationMode, 0x006910B0, (int mode));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(void*, __cdecl, _malloc, 0x007B1EC2, (uint32_t Size));

static unsigned int __fastcall S_00471340_InitializePool(
    S_00471340* self, void* /*edx*/,
    int elementCount, unsigned int requestedBits, char useCrtHeap)
{
    if (self->field_0000)
        return 0;

    const uint32_t headerBytes = 4 * (((requestedBits >> 3) + 1) >> 2) + 4;
    CSF_AT(uint32_t, self, 0x14) = headerBytes;

    const uint32_t elementStride = headerBytes * 8;
    const uint32_t allocationBytes =
        CSF_AT(uint32_t, self, 0x14) +
        elementStride * static_cast<uint32_t>(elementCount);

    self->field_0028 = useCrtHeap;

    if (useCrtHeap)
    {
        CSF_AT(uint32_t, self, 0x04) =
            reinterpret_cast<std::uintptr_t>(_malloc(allocationBytes));
    }
    else
    {
        SetAllocationMode(1);
        CSF_AT(uint32_t, self, 0x04) =
            reinterpret_cast<std::uintptr_t>(Mem_Alloc(allocationBytes));
        SetAllocationMode(0);
    }

    void* buffer = reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x04));
    if (!buffer)
        return 0;

    std::memset(buffer, 0, CSF_AT(uint32_t, self, 0x14));

    const uint32_t bufferAddress = CSF_AT(uint32_t, self, 0x04);
    const uint32_t elementsAddress =
        bufferAddress + CSF_AT(uint32_t, self, 0x14);
    const uint32_t endAddress = bufferAddress + allocationBytes;

    CSF_AT(uint32_t, self, 0x08) = elementsAddress;
    CSF_AT(uint32_t, self, 0x0C) = endAddress;
    CSF_AT(uint32_t, self, 0x18) = 0;
    CSF_AT(int, self, 0x10) = elementCount;
    CSF_AT(uint32_t, self, 0x1C) = 0;
    CSF_AT(uint32_t, self, 0x24) = elementStride;
    CSF_AT(uint32_t, self, 0x20) = 0;
    self->field_0000 = 1;

    return elementStride;
}

HOOK(0x0069B100, S_00471340_InitializePool, "auto");
