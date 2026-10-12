#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static int __fastcall C_00868b04_TransferNextBlock(
    C_00868b04* self, void* /*edx*/, int destination, int /*unused*/)
{
    if (!self->field_001c)
        return 1;

    const uint32_t blockIndex = self->field_0020;
    const int32_t cycleIndex =
        static_cast<int32_t>(blockIndex) %
        static_cast<int32_t>(self->field_0024);

    // The original reads all four bytes beginning at the byte-sized field.
    const uint32_t blockSize =
        (CSF_AT(uint32_t, self, 0x18) >> 3) *
        CSF_AT(uint32_t, self, 0x10) *
        CSF_AT(uint32_t, self, 0x0C);
    const uint32_t blockOffset = blockSize * static_cast<uint32_t>(cycleIndex);

    if (!blockIndex && !self->field_0030)
    {
        void* stream = reinterpret_cast<void*>(self->field_001c);
        ((void(__fastcall*)(void*, void*))
            CSF_AT(void**, stream, 0)[1])(stream, nullptr);
        self->field_0030 = 1;
    }

    void* stream = reinterpret_cast<void*>(self->field_001c);
    const int position =
        ((int(__fastcall*)(void*, void*))
            CSF_AT(void**, stream, 0)[3])(stream, nullptr);

    if (position < static_cast<int32_t>(self->field_0028))
        self->field_002c += blockSize * self->field_0024;

    const uint32_t requiredPosition = blockSize * (self->field_0020 + 1);
    const uint32_t availablePosition =
        static_cast<uint32_t>(position) + self->field_002c;
    self->field_0028 = static_cast<uint32_t>(position);

    if (static_cast<int32_t>(availablePosition) <
        static_cast<int32_t>(requiredPosition))
        return 2;

    stream = reinterpret_cast<void*>(self->field_001c);
    if (!((bool(__fastcall*)(void*, void*, int, int, int))
              CSF_AT(void**, stream, 0)[4])(
            stream, nullptr, destination,
            static_cast<int32_t>(blockSize),
            static_cast<int32_t>(blockOffset)))
        return 2;

    ++self->field_0020;
    return 0;
}

HOOK(0x00752330, C_00868b04_TransferNextBlock, "auto");
