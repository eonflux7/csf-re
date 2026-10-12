#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static char __fastcall C_008443c8_ReadTerminatedString(
    C_008443c8* self, void* /*edx*/, int destination,
    unsigned int capacity, int /*unused*/)
{
    unsigned int length;
    void* reader = reinterpret_cast<void*>(self->field_0004);
    if (!reinterpret_cast<unsigned char (__fastcall*)(void*, void*, unsigned int*)>(
            CSF_AT(void**, reader, 0)[0x20 / 4])(reader, nullptr, &length)
        || length >= capacity)
    {
        return 0;
    }

    char result = static_cast<char>(
        self->vSlot64(destination, static_cast<int>(length)));
    if (result)
        *reinterpret_cast<char*>(static_cast<uint32_t>(destination) + length) = 0;
    return result;
}

HOOK(0x0054E300, C_008443c8_ReadTerminatedString, "auto");
