#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// snprintf from the C runtime (cdecl, variadic).
CSF_FUNC(int, __cdecl, __snprintf, 0x007B02F5, (void* Buffer, uint32_t BufferCount, void* Format, ...));

// Appends the formatted text to the log sink (__thiscall, 2 stack args, ret 8).
CSF_FUNC(unsigned int, __fastcall, CoreZlibLog_AppendText, 0x00694FB0, (void* self, void* edx, char* a2, char* a3));

static unsigned int __fastcall CoreZlibLog_WriteFloat(S_00694cd0* self, void* /*edx*/, float value, const char* suffix)
{
    alignas(8) char buffer[0x34];
    __snprintf(buffer, 0x32u, const_cast<char*>(CSF_STR(0x840C3C, "%f")), value);
    return CoreZlibLog_AppendText(self, nullptr, buffer, const_cast<char*>(suffix));
}

HOOK(0x695330, CoreZlibLog_WriteFloat, "auto");
