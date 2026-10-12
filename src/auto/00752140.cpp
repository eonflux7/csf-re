#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static int __fastcall C_00868af0_WriteNextBlock(
    C_00868af0* self, void* /*edx*/, int source, int /*unused*/)
{
    void* stream = CSF_AT(void*, self, 0x08);
    if (!stream)
        return 2;

    const uint32_t blockIndex = self->field_0024 % self->field_0020;
    const int32_t blockSize = static_cast<int32_t>(
        (self->field_001c >> 3) * self->field_0014 *
        CSF_AT(uint32_t, self, 0x10));
    const uint32_t offset = blockIndex * static_cast<uint32_t>(blockSize);

    const int position =
        ((int(__fastcall*)(void*, void*))
            CSF_AT(void**, stream, 0)[0x1C / 4])(stream, nullptr);
    const int currentBlock = position / blockSize;

    if (static_cast<int32_t>(self->field_0024) > 0 &&
        static_cast<uint32_t>(currentBlock) ==
            self->field_0024 % self->field_0020)
        return 2;

    stream = CSF_AT(void*, self, 0x08);
    if (!((bool(__fastcall*)(void*, void*, int, int32_t, uint32_t))
              CSF_AT(void**, stream, 0)[0x2C / 4])(
            stream, nullptr, source, blockSize, offset))
        return 2;

    if (self->field_0024 == 2)
    {
        stream = CSF_AT(void*, self, 0x08);
        ((void(__fastcall*)(void*, void*, int))
            CSF_AT(void**, stream, 0)[0x04 / 4])(stream, nullptr, 1);
    }

    ++self->field_0024;
    return 0;
}

HOOK(0x00752140, C_00868af0_WriteNextBlock, "auto");
