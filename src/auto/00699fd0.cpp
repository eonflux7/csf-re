#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, CoreZlibLog_Deflate, 0x006A0570, (int a1, unsigned int a2));
CSF_FUNC(std::uint32_t, __cdecl, CoreZlibLog_EndDeflate, 0x006A07F0, (void* stream));

static char __fastcall C_0085e748_FinalizeCompression(
    C_0085e748* self, void* /*edx*/)
{
    if (self->field_0008)
        return 1;

    alignas(8) std::uint8_t finalInput[0x1];
    finalInput[0] = 0;
    CSF_AT(std::uint32_t, reinterpret_cast<void*>(self->field_0010), 0x4) = 1;
    CSF_AT(void*, reinterpret_cast<void*>(self->field_0010), 0x0) = finalInput;

    int result = CoreZlibLog_Deflate(static_cast<int>(self->field_0010), 4);
    while (result == 0)
    {
        void* destination = reinterpret_cast<void*>(self->field_0004);
        int written = reinterpret_cast<int(__fastcall*)(void*, void*, void*, int)>(
            CSF_AT(void**, destination, 0)[0xC / 4])(
                destination, nullptr, reinterpret_cast<void*>(self->field_000c), 0x4000);
        if (written != 0x4000)
            return 0;

        CSF_AT(std::uint32_t, reinterpret_cast<void*>(self->field_0010), 0xC) =
            self->field_000c;
        CSF_AT(std::uint32_t, reinterpret_cast<void*>(self->field_0010), 0x10) =
            0x4000;
        result = CoreZlibLog_Deflate(static_cast<int>(self->field_0010), 4);
    }

    if (result != 1)
        return 0;

    int remaining = static_cast<int>(
        0x4000 - CSF_AT(std::uint32_t,
                       reinterpret_cast<void*>(self->field_0010), 0x10));
    if (remaining)
    {
        void* destination = reinterpret_cast<void*>(self->field_0004);
        int written = reinterpret_cast<int(__fastcall*)(void*, void*, void*, int)>(
            CSF_AT(void**, destination, 0)[0xC / 4])(
                destination, nullptr, reinterpret_cast<void*>(self->field_000c), remaining);
        if (written != remaining)
            return 0;
    }

    if (CoreZlibLog_EndDeflate(reinterpret_cast<void*>(self->field_0010)))
        return 0;

    return 1;
}

HOOK(0x00699FD0, C_0085e748_FinalizeCompression, "auto");
