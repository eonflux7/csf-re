#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static uint32_t ReadPointerBits(const float& field)
{
    uint32_t bits;
    std::memcpy(&bits, &field, sizeof(bits));
    return bits;
}

static void WritePointerBits(float& field, uint32_t bits)
{
    std::memcpy(&field, &bits, sizeof(bits));
}

static int __fastcall S_006e48a0_RegisterInstance(
    S_006e48a0* self, void* /*edx*/, int instance)
{
    const uint32_t freeBegin = self->field_0018;
    if (freeBegin)
    {
        const int32_t freeCount =
            static_cast<int32_t>(ReadPointerBits(self->field_001c) - freeBegin) >> 2;
        if (freeCount != 0)
        {
            const uint32_t recordsBegin = ReadPointerBits(self->field_0008);
            const uint32_t lastFreeAddress =
                freeBegin + static_cast<uint32_t>(freeCount) * 4 - 4;
            const int index = CSF_AT(
                int, reinterpret_cast<void*>(lastFreeAddress), 0x0);
            void* record = reinterpret_cast<void*>(
                recordsBegin + static_cast<uint32_t>(index) * 16);

            const uint32_t freeEnd = ReadPointerBits(self->field_001c);
            if ((static_cast<int32_t>(freeEnd - freeBegin) >> 2) != 0)
                WritePointerBits(self->field_001c, freeEnd - 4);

            CSF_AT(int, record, 0x4) = -1;
            CSF_AT(uint32_t, record, 0x8) = self->field_0034;
            CSF_AT(int, record, 0x0) = instance;
            return index;
        }
    }

    if (self->field_003c)
    {
        const uint32_t recordsBegin = ReadPointerBits(self->field_0008);
        const int32_t recordCount = recordsBegin
            ? static_cast<int32_t>(self->field_000c - recordsBegin) >> 4
            : 0;

        int index = 0;
        uint32_t byteOffset = 0;
        do
        {
            void* record = reinterpret_cast<void*>(
                ReadPointerBits(self->field_0008) + byteOffset);
            if (CSF_AT(int, record, 0x0) != instance)
            {
                CSF_AT(uint32_t, record, 0x8) = self->field_0034;
                CSF_AT(int, record, 0x0) = instance;
                return index;
            }
            ++index;
            byteOffset += 16;
        } while (index < recordCount);
    }

    return -1;
}

HOOK(0x007012E0, S_006e48a0_RegisterInstance, "auto");
