#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// The stream object stored at archive+4 has thiscall-style virtual methods:
// `this` goes in ecx, (buffer, length) are stack arguments and the callee pops them.
// Slot 24 (+0x60) is the write path, slot 25 (+0x64) is the read path.
using StreamTransferMethodFn = uint8_t(__fastcall*)(void* self, void* edx, int buffer, int length);

static bool __cdecl Archive_SerializeBytes(bool* archive, int buffer, int length, int mode)
{
    // Store the mode argument at archive+0x1c (dword index 7).
    CSF_AT(int, archive, 0x1c) = mode;

    bool result = false;

    // Only runs while the archive's done/error flag at +0 is clear.
    if (!CSF_AT(bool, archive, 0))
    {
        bool useWrite = CSF_AT(bool, archive, 0x14);
        CSF_AT(bool, archive, 0x15) = false;
        CSF_AT(bool, archive, 0x16) = false;

        void* stream = reinterpret_cast<void*>(CSF_AT(uint32_t, archive, 4));

        uint8_t status;
        if (useWrite)
        {
            auto transfer = reinterpret_cast<StreamTransferMethodFn>(CSF_AT(void**, stream, 0)[24]);
            status = transfer(stream, nullptr, buffer, length);
        }
        else
        {
            auto transfer = reinterpret_cast<StreamTransferMethodFn>(CSF_AT(void**, stream, 0)[25]);
            status = transfer(stream, nullptr, buffer, length);
        }

        result = (status == 0);
        CSF_AT(bool, archive, 0) = result;
    }

    return result;
}

HOOK(0x691580, Archive_SerializeBytes, "auto");
