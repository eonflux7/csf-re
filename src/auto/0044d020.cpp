#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __fastcall, FindPositionSlotIndex, 0x0044BC20, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, InitCommandBuffer, 0x0044C3D0, (int a1, int a2));
CSF_FUNC(uint32_t*, __fastcall, EnqueueCommand, 0x0044CDB0, (void* self, void* edx, int a3));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

[[maybe_unused]] static const auto kSlotIndexerVtbl = CSF_VTBL(0x0044BC20);

static void* __fastcall SubmitFlaggedPositionCommand(S_0044bc20* self, void* /*edx*/, int a3, int* a4) {
    // Stack-resident command object (0x70 bytes): +0x00 array, +0x38 a3,
    // +0x44 Block, +0x58 capacity, +0x5C/+0x60 args, +0x64 vtable, +0x68/+0x6C -1.
    alignas(8) uint8_t commandBuf[0x70];

    CSF_AT(void*, commandBuf, 0x64) = reinterpret_cast<void*>(CSF_VTBL(0x0082D448));
    CSF_AT(int, commandBuf, 0x68) = -1;
    CSF_AT(int, commandBuf, 0x6C) = -1;

    if (a3 >= 0) {
        int slotIndex = FindPositionSlotIndex(self, nullptr, a3);
        if (slotIndex != -1) {
            int slotOffset = 100 * slotIndex;
            uint8_t* slotA = reinterpret_cast<uint8_t*>(self->field_0010) + slotOffset;
            CSF_AT(int, slotA, 0x38) = -1;
            uint8_t* slotB = reinterpret_cast<uint8_t*>(self->field_0010) + slotOffset;
            CSF_AT(uint8_t, slotB, 0x28) = 0;
        }
    }

    InitCommandBuffer(reinterpret_cast<int>(commandBuf), 0);

    int secondArg = a4[1];
    CSF_AT(int, commandBuf, 0x5C) = a4[0];
    CSF_AT(int, commandBuf, 0x00) = 0;
    CSF_AT(int, commandBuf, 0x60) = secondArg;
    CSF_AT(int, commandBuf, 0x38) = a3;

    void* result = EnqueueCommand(static_cast<void*>(&self->pad_0000[12]), nullptr,
                                  reinterpret_cast<int>(commandBuf));

    if (CSF_AT(uint32_t, commandBuf, 0x58) >= 0x10u) {
        result = CSF_AT(void*, commandBuf, 0x44);
        if (result) {
            return reinterpret_cast<void*>(Mem_FreeDebug(result));
        }
    }
    return result;
}

HOOK(0x0044D020, SubmitFlaggedPositionCommand, "auto");
