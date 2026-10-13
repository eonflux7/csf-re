#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, CoreMemory_AllocUntagged, 0x006910F0, (std::size_t size));
CSF_FUNC(char, __fastcall, S_00693740_OpenPackedBuffer, 0x00693740, (void* self, void* edx));
CSF_FUNC(uint8_t*, __fastcall, S_00693ad0_Construct, 0x00693AD0, (void* self, void* edx, char a2));
CSF_FUNC(char, __fastcall, S_00693740_Reset, 0x00693BD0, (void* self, void* edx));
CSF_FUNC(uint8_t*, __fastcall, S_00693740_AttachReader, 0x006943C0, (void* self, void* edx, uint8_t* a2));
CSF_FUNC(uint32_t*, __fastcall, S_0069e460_Construct, 0x0069E460, (void* self, void* edx));
CSF_FUNC(int, __fastcall, S_0069e480_SetBuffer, 0x0069E740, (void* self, void* edx, void* a2, void* a3, int a4));

static char __fastcall S_00693740_OpenFileDescriptor(
    S_00693740* self, void* /*edx*/, void* descriptor,
    std::size_t size, char* textOutput, uint32_t* bytesRead)
{
    // Preserve the packet's additional vtable bindings without writing them.
    (void)CSF_VTBL(0x0069E460);
    (void)CSF_VTBL(0x00693AD0);
    (void)CSF_VTBL(0x00693740);

    if (!descriptor)
        return 0;

    S_00693740_Reset(self, nullptr);

    const uint32_t allocation =
        static_cast<uint32_t>(CoreMemory_AllocUntagged(size));
    self->field_0004 = allocation;
    CSF_AT(uint32_t, self, 0x08) = static_cast<uint32_t>(size);

    *bytesRead = static_cast<uint32_t>(
        reinterpret_cast<int (__fastcall*)(void*, void*, uint32_t, std::size_t)>(
            CSF_AT(void**, descriptor, 0)[8 / 4])(
                descriptor, nullptr, allocation, size));

    self->field_0014 = 1;
    if (*bytesRead > 6u)
    {
        const auto* buffer = reinterpret_cast<const uint8_t*>(self->field_0004);
        if (buffer[0] == 67 && buffer[1] == 83 &&
            buffer[2] == 70 && buffer[3] == 70 &&
            buffer[4] == 66 && buffer[5] == 83)
        {
            self->field_0014 = 0;
        }
    }

    if (textOutput)
    {
        const char* source = reinterpret_cast<const char*>(self->field_0004);
        char character;
        do
        {
            character = *source++;
            *textOutput++ = character;
        } while (character != 0);
    }

    if (self->field_0014)
    {
        void* streamStorage =
            reinterpret_cast<void*>(CoreMemory_AllocUntagged(44));
        uint32_t* stream = streamStorage
            ? S_0069e460_Construct(streamStorage, nullptr)
            : nullptr;

        void* buffer = reinterpret_cast<void*>(self->field_0004);
        self->field_000c = reinterpret_cast<uint32_t>(stream);
        S_0069e480_SetBuffer(
            stream, nullptr, buffer, reinterpret_cast<void*>(size), 1);

        void* readerStorage =
            reinterpret_cast<void*>(CoreMemory_AllocUntagged(12));
        uint8_t* reader = readerStorage
            ? S_00693ad0_Construct(readerStorage, nullptr, 1)
            : nullptr;

        self->field_0000 = reinterpret_cast<uint32_t>(reader);
        self->field_0010 = 0;
        S_00693740_AttachReader(self, nullptr, reader);
        return 1;
    }

    self->field_0000 = 0;
    return S_00693740_OpenPackedBuffer(self, nullptr);
}

HOOK(0x00694660, S_00693740_OpenFileDescriptor, "auto");
