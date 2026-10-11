#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, sub_515080, 0x00515080, (int a1, int a2));
CSF_FUNC(void*, __cdecl, sub_515160, 0x00515160, (void* a2, void* arg4, void* a3));
CSF_FUNC(int, __fastcall, ComputeChildHandleFromSlot, 0x00515BD0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, sub_5167D0, 0x005167D0, (void* self, void* edx, void* a2));
CSF_FUNC(uint32_t*, __fastcall, sub_5170E0, 0x005170E0, (void* self, void* edx, uint32_t* a2, int a3, int a4));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

static int __fastcall C_00840af8_MoveActiveRecordToFront(
    C_00840af8* self, void* /*edx*/)
{
    // The packet also classifies this direct call target as a vtable reference.
    (void)CSF_VTBL(0x005170E0);

    int result = static_cast<int>(self->field_0008);
    if (result == 0)
        return result;

    result = static_cast<int32_t>(
        self->field_000c - self->field_0008) / 80;
    uint32_t recordCount = static_cast<uint32_t>(result);
    if (result == 0)
        return result;

    alignas(8) uint8_t savedRecordBytes[0x50];
    uint8_t* recordRange = reinterpret_cast<uint8_t*>(self) + 4;

    uint32_t selectedOffset =
        80u * static_cast<uint32_t>(ComputeChildHandleFromSlot(self, nullptr));

    sub_5167D0(savedRecordBytes, nullptr,
              reinterpret_cast<void*>(self->field_0008 + selectedOffset));

    uint32_t selectedAddress = self->field_0008 + selectedOffset;
    uint32_t endAddress = self->field_000c;
    sub_515160(reinterpret_cast<void*>(selectedAddress + 80),
               reinterpret_cast<void*>(endAddress),
               reinterpret_cast<void*>(selectedAddress));

    endAddress = self->field_000c;
    sub_515080(static_cast<int>(endAddress - 80),
              static_cast<int>(endAddress));

    self->field_000c -= 80;

    sub_5170E0(recordRange, nullptr, &recordCount,
               static_cast<int>(self->field_0008),
               static_cast<int>(reinterpret_cast<uintptr_t>(savedRecordBytes)));

    void* block = CSF_AT(void*, savedRecordBytes, 0x38);
    result = static_cast<int>(reinterpret_cast<uintptr_t>(block));
    if (block)
        result = Mem_FreeDebug(block);

    CSF_AT(void*, savedRecordBytes, 0x38) = nullptr;
    CSF_AT(uint32_t, savedRecordBytes, 0x3C) = 0;
    CSF_AT(uint32_t, savedRecordBytes, 0x40) = 0;

    if (CSF_AT(uint32_t, savedRecordBytes, 0x18) >= 8) {
        void* stringStorage = CSF_AT(void*, savedRecordBytes, 0x04);
        result = static_cast<int>(reinterpret_cast<uintptr_t>(stringStorage));
        if (stringStorage)
            result = Mem_FreeDebug(stringStorage);
    }

    return result;
}

HOOK(0x00517150, C_00840af8_MoveActiveRecordToFront, "auto");
