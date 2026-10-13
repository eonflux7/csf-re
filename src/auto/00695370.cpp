#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(unsigned int, __fastcall, CoreZlibLog_AppendText, 0x00694FB0, (void* self, void* edx, char* a2, char* a3));
CSF_FUNC(int, __cdecl, __snprintf, 0x007B02F5, (void* Buffer, uint32_t BufferCount, void* Format, ...));

static unsigned int __fastcall CoreZlibLog_WriteBoolean(S_00694cd0* self, void* /*edx*/, uint8_t a2, const char* a3)
{
    char buffer[0x34];
    __snprintf(buffer, 0x32u, const_cast<char*>(CSF_STR(0x83090C, "%d")), static_cast<int>(a2));
    return CoreZlibLog_AppendText(self, nullptr, buffer, const_cast<char*>(a3));
}

HOOK(0x00695370, CoreZlibLog_WriteBoolean, "auto");
