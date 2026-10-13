#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, CoreZlibLog_Inflate, 0x006A1A30, (uint8_t** stream, int flush));

static int __fastcall C_0085e748_ReadDecompressedBytes(
    C_0085e748* self, void* /*edx*/, int output, int requestedBytes)
{
    if (!requestedBytes || !self->field_0008)
        return 0;

    CSF_AT(int, reinterpret_cast<void*>(self->field_0010), 0x10) = requestedBytes;
    CSF_AT(int, reinterpret_cast<void*>(self->field_0010), 0x0C) = output;

    if (!CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0010), 0x04))
    {
        void* source = reinterpret_cast<void*>(self->field_0004);
        int bytesRead =
            reinterpret_cast<int (__fastcall*)(void*, void*, uint32_t, int)>(
                CSF_AT(void**, source, 0)[2])(
                    source, nullptr, self->field_000c, 0x4000);
        if (!bytesRead)
            return 0;

        CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0010), 0x00) =
            self->field_000c;
        CSF_AT(int, reinterpret_cast<void*>(self->field_0010), 0x04) = bytesRead;
    }

    if (CoreZlibLog_Inflate(reinterpret_cast<uint8_t**>(self->field_0010), 2))
        return 0;

    if (!CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0010), 0x10))
        return requestedBytes;

    for (;;)
    {
        void* source = reinterpret_cast<void*>(self->field_0004);
        int bytesRead =
            reinterpret_cast<int (__fastcall*)(void*, void*, uint32_t, int)>(
                CSF_AT(void**, source, 0)[2])(
                    source, nullptr, self->field_000c, 0x4000);
        void* stream = reinterpret_cast<void*>(self->field_0010);

        if (!bytesRead)
        {
            return static_cast<int>(
                static_cast<uint32_t>(requestedBytes) -
                CSF_AT(uint32_t, stream, 0x10));
        }

        CSF_AT(uint32_t, stream, 0x00) = self->field_000c;
        CSF_AT(int, reinterpret_cast<void*>(self->field_0010), 0x04) = bytesRead;

        if (CoreZlibLog_Inflate(reinterpret_cast<uint8_t**>(self->field_0010), 2))
        {
            return static_cast<int>(
                static_cast<uint32_t>(requestedBytes) -
                CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0010), 0x10));
        }

        if (!CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0010), 0x10))
            return requestedBytes;
    }
}

HOOK(0x00699E40, C_0085e748_ReadDecompressedBytes, "auto");
