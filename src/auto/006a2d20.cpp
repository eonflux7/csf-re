#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_0085ee28_LoadBlock, 0x006A2BB0, (void* self, void* edx));

static char __fastcall C_0085ee28_Seek(
    C_0085ee28* self, void* /*edx*/, int origin, int offset)
{
    void* stream = reinterpret_cast<void*>(
        CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0004), 0x4));

    if (origin == 1)
        offset = static_cast<int>(static_cast<uint32_t>(offset) +
                                  self->field_0010 + self->field_001c);
    else if (origin == 2)
        offset = static_cast<int>(self->field_000c -
                                  static_cast<uint32_t>(offset));

    int remaining = offset;
    if (remaining > static_cast<int>(self->field_000c) || remaining < 0)
        return 0;

    const int bufferOffset = static_cast<int>(
        static_cast<uint32_t>(remaining) - self->field_001c);
    if (bufferOffset >= 0 &&
        bufferOffset < static_cast<int>(self->field_0018))
    {
        self->field_0010 = bufferOffset;
        return 1;
    }

    ((void(__fastcall*)(void*, void*, int, uint32_t))
        CSF_AT(void**, stream, 0)[0x14 / 4])(
            stream, nullptr, 0, self->field_0008);

    int blockSize = 0;
    for (;;)
    {
        const uint32_t blockPosition =
            ((int(__fastcall*)(void*, void*))
                CSF_AT(void**, stream, 0)[0x10 / 4])(stream, nullptr);

        ((void(__fastcall*)(void*, void*, int*, int))
            CSF_AT(void**, stream, 0)[0x8 / 4])(
                stream, nullptr, &origin, 4);
        if (origin == 0)
            return 0;

        ((void(__fastcall*)(void*, void*, int*, int))
            CSF_AT(void**, stream, 0)[0x8 / 4])(
                stream, nullptr, &blockSize, 4);

        ((void(__fastcall*)(void*, void*, int, int))
            CSF_AT(void**, stream, 0)[0x14 / 4])(
                stream, nullptr, 1, origin);

        if (remaining < blockSize)
        {
            self->field_0014 = blockPosition;
            C_0085ee28_LoadBlock(self, nullptr);
            self->field_0010 = remaining;
            self->field_001c = static_cast<uint32_t>(offset) -
                               static_cast<uint32_t>(remaining);
        }

        remaining = static_cast<int>(
            static_cast<uint32_t>(remaining) -
            static_cast<uint32_t>(blockSize));
        if (remaining < 0)
            return 1;
    }
}

HOOK(0x006A2D20, C_0085ee28_Seek, "auto");
