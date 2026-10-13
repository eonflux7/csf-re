#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, S_0041f0a0_Serialize, 0x0041F0A0, (S_0041f0a0* self, void* , S_0041f0a0* stream));
CSF_FUNC(int, __cdecl, Archive_SerializeString, 0x00496080, (int archiveAddress, S_00691860* string));

// The packet labels the base serializer's code address as a vtable reference.
static const auto kBaseSerializerReference = CSF_VTBL(0x0041F0A0);

static bool Archive_PrepareField(S_00691860* stream)
{
    const bool ready = CSF_AT(uint8_t, stream, 0x00) == 0;
    CSF_AT(int, stream, 0x1C) = 0;
    return ready;
}

static int __fastcall S_0041f0a0_SerializeExtended(
    S_0041f0a0* self, void* /*edx*/, int streamAddress)
{
    auto* stream = reinterpret_cast<S_00691860*>(streamAddress);
    auto* fields = reinterpret_cast<uint8_t*>(self);

    bool failedToBegin = false;
    if (CSF_AT(uint8_t, stream, 0x00) == 0)
    {
        void* backend = CSF_AT(void*, stream, 0x04);
        failedToBegin =
            !((bool(__fastcall*)(void*, void*, int, S_00691860*))
                CSF_AT(void**, backend, 0)[128 / 4])(
                    backend, nullptr, 0, stream);
    }

    const int savedContext = CSF_AT(int, stream, 0x1C);
    CSF_AT(uint8_t, stream, 0x00) = failedToBegin;
    S_0041f0a0_Serialize(
        self, nullptr, reinterpret_cast<S_0041f0a0*>(stream));

    if (Archive_PrepareField(stream))
    {
        void* backend = CSF_AT(void*, stream, 0x04);
        CSF_AT(uint8_t, stream, 0x15) = 0;
        CSF_AT(uint8_t, stream, 0x16) = 0;
        CSF_AT(uint8_t, stream, 0x00) =
            ((int(__fastcall*)(void*, void*, uint8_t*, S_00691860*))
                CSF_AT(void**, backend, 0)[88 / 4])(
                    backend, nullptr, fields + 0x1C, stream) == -1;
    }

    if (Archive_PrepareField(stream))
    {
        CSF_AT(uint8_t, stream, 0x15) = 0;
        CSF_AT(uint8_t, stream, 0x16) = 0;
        Archive_SerializeString(reinterpret_cast<int>(fields + 0x20), stream);
    }

    if (Archive_PrepareField(stream))
    {
        void* backend = CSF_AT(void*, stream, 0x04);
        CSF_AT(uint8_t, stream, 0x15) = 0;
        CSF_AT(uint8_t, stream, 0x16) = 0;
        CSF_AT(uint8_t, stream, 0x00) =
            ((int(__fastcall*)(void*, void*, uint8_t*, S_00691860*))
                CSF_AT(void**, backend, 0)[84 / 4])(
                    backend, nullptr, fields + 0x4C, stream) == -1;
    }

    if (Archive_PrepareField(stream))
    {
        void* backend = CSF_AT(void*, stream, 0x04);
        CSF_AT(uint8_t, stream, 0x15) = 0;
        CSF_AT(uint8_t, stream, 0x16) = 0;
        CSF_AT(uint8_t, stream, 0x00) =
            ((int(__fastcall*)(void*, void*, uint8_t*, S_00691860*))
                CSF_AT(void**, backend, 0)[84 / 4])(
                    backend, nullptr, fields + 0x50, stream) == -1;
    }

    if (Archive_PrepareField(stream))
    {
        void* backend = CSF_AT(void*, stream, 0x04);
        CSF_AT(uint8_t, stream, 0x15) = 0;
        CSF_AT(uint8_t, stream, 0x16) = 0;
        CSF_AT(uint8_t, stream, 0x00) =
            ((int(__fastcall*)(void*, void*, uint8_t*, S_00691860*))
                CSF_AT(void**, backend, 0)[84 / 4])(
                    backend, nullptr, fields + 0x54, stream) == -1;
    }

    if (Archive_PrepareField(stream))
    {
        void* backend = CSF_AT(void*, stream, 0x04);
        CSF_AT(uint8_t, stream, 0x15) = 0;
        CSF_AT(uint8_t, stream, 0x16) = 0;
        CSF_AT(uint8_t, stream, 0x00) =
            ((int(__fastcall*)(void*, void*, uint8_t*, S_00691860*))
                CSF_AT(void**, backend, 0)[84 / 4])(
                    backend, nullptr, fields + 0x58, stream) == -1;
    }

    if (Archive_PrepareField(stream))
    {
        void* backend = CSF_AT(void*, stream, 0x04);
        CSF_AT(uint8_t, stream, 0x15) = 0;
        CSF_AT(uint8_t, stream, 0x16) = 0;
        CSF_AT(uint8_t, stream, 0x00) =
            ((int(__fastcall*)(void*, void*, uint8_t*, S_00691860*))
                CSF_AT(void**, backend, 0)[84 / 4])(
                    backend, nullptr, fields + 0x5C, stream) == -1;
    }

    if (Archive_PrepareField(stream))
    {
        void* backend = CSF_AT(void*, stream, 0x04);
        CSF_AT(uint8_t, stream, 0x15) = 0;
        CSF_AT(uint8_t, stream, 0x16) = 0;
        CSF_AT(uint8_t, stream, 0x00) =
            ((int(__fastcall*)(void*, void*, uint8_t*, S_00691860*))
                CSF_AT(void**, backend, 0)[88 / 4])(
                    backend, nullptr, fields + 0x60, stream) == -1;
    }

    if (Archive_PrepareField(stream))
    {
        void* backend = CSF_AT(void*, stream, 0x04);
        CSF_AT(uint8_t, stream, 0x15) = 0;
        CSF_AT(uint8_t, stream, 0x16) = 0;
        CSF_AT(uint8_t, stream, 0x00) =
            ((int(__fastcall*)(void*, void*, uint8_t*, S_00691860*))
                CSF_AT(void**, backend, 0)[88 / 4])(
                    backend, nullptr, fields + 0x64, stream) == -1;
    }

    if (Archive_PrepareField(stream))
    {
        void* backend = CSF_AT(void*, stream, 0x04);
        CSF_AT(uint8_t, stream, 0x15) = 0;
        CSF_AT(uint8_t, stream, 0x16) = 0;
        CSF_AT(uint8_t, stream, 0x00) =
            ((int(__fastcall*)(void*, void*, uint8_t*, S_00691860*))
                CSF_AT(void**, backend, 0)[88 / 4])(
                    backend, nullptr, fields + 0x68, stream) == -1;
    }

    if (Archive_PrepareField(stream))
    {
        void* backend = CSF_AT(void*, stream, 0x04);
        CSF_AT(uint8_t, stream, 0x15) = 0;
        CSF_AT(uint8_t, stream, 0x16) = 0;
        CSF_AT(uint8_t, stream, 0x00) =
            ((int(__fastcall*)(void*, void*, uint8_t*, S_00691860*))
                CSF_AT(void**, backend, 0)[88 / 4])(
                    backend, nullptr, fields + 0x6C, stream) == -1;
    }

    if (Archive_PrepareField(stream))
    {
        void* backend = CSF_AT(void*, stream, 0x04);
        CSF_AT(uint8_t, stream, 0x15) = 0;
        CSF_AT(uint8_t, stream, 0x16) = 0;
        CSF_AT(uint8_t, stream, 0x00) =
            ((int(__fastcall*)(void*, void*, uint8_t*, S_00691860*))
                CSF_AT(void**, backend, 0)[68 / 4])(
                    backend, nullptr, fields + 0x70, stream) == -1;
    }

    CSF_AT(int, stream, 0x1C) = savedContext;
    if (CSF_AT(uint8_t, stream, 0x00) != 0)
    {
        CSF_AT(uint8_t, stream, 0x00) = 1;
        return 1;
    }

    void* backend = CSF_AT(void*, stream, 0x04);
    const int result =
        ((int(__fastcall*)(void*, void*, S_00691860*))
            CSF_AT(void**, backend, 0)[124 / 4])(
                backend, nullptr, stream);
    if (static_cast<uint8_t>(result) == 0)
    {
        CSF_AT(uint8_t, stream, 0x00) = 1;
        return 1;
    }

    CSF_AT(uint8_t, stream, 0x00) = 0;
    return result;
}

HOOK(0x00424BE0, S_0041f0a0_SerializeExtended, "auto");
