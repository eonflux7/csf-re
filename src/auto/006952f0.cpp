#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, __snprintf, 0x007B02F5, (void* Buffer, uint32_t BufferCount, void* Format, ...));
CSF_FUNC(unsigned int, __fastcall, sub_694FB0, 0x00694FB0, (void* self, void* edx, char* a2, char* a3));

static unsigned int __fastcall FormatAndWriteInteger(S_00694cd0* self, void* /*edx*/, int value, const char* text)
{
    char buffer[0x34];

    __snprintf(buffer, 0x32u, const_cast<char*>(CSF_STR(0x0083090C, "%d")), value);
    return sub_694FB0(self, nullptr, buffer, const_cast<char*>(text));
}

HOOK(0x6952F0, FormatAndWriteInteger, "auto");
