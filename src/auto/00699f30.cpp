#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, ProcessBufferedState, 0x006A0570, (int a1, unsigned int a2));

static int __fastcall C_0085e748_ProcessInput(
    C_0085e748* self, void* /*edx*/, int input, int inputSize)
{
    if (!inputSize || self->field_0008)
        return 0;

    CSF_AT(int, reinterpret_cast<void*>(self->field_0010), 0x04) = inputSize;
    CSF_AT(int, reinterpret_cast<void*>(self->field_0010), 0x00) = input;

    if (ProcessBufferedState(static_cast<int>(self->field_0010), 0))
        return 0;

    if (CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0010), 0x10))
        return inputSize;

    for (;;)
    {
        void* source = reinterpret_cast<void*>(self->field_0004);
        int count = reinterpret_cast<int (__fastcall*)(void*, void*, uint32_t, int)>(
            CSF_AT(void**, source, 0)[0x0C / 4])(
                source, nullptr, self->field_000c, 0x4000);
        if (count != 0x4000)
            return 0;

        CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0010), 0x0C) =
            self->field_000c;
        CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0010), 0x10) =
            0x4000;

        if (ProcessBufferedState(static_cast<int>(self->field_0010), 0))
            return 0;

        if (CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0010), 0x10))
            return inputSize;
    }
}

HOOK(0x00699F30, C_0085e748_ProcessInput, "auto");
