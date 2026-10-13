#include <cstddef>
#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char*, __cdecl, _strncpy, 0x007AFEC0,
         (char* dest, const char* src, size_t count));
CSF_FUNC(unsigned int, __fastcall, CoreStringFormat_WriteBufferedString, 0x00694FB0,
         (void* self, void* edx, char* a2, char* a3));

CSF_GLOBAL(const char, g_StringDelimiter, 0x0083AB18);

static unsigned int __fastcall S_00694cd0_WriteDelimitedString(
    S_00694cd0* self, void* /*edx*/, char* source, char* argument)
{
    alignas(8) char buffer[0x404];

    _strncpy(buffer, &g_StringDelimiter, 0x400u);
    buffer[0x3FF] = 0;

    unsigned int length = 0;
    while (buffer[length] != 0)
        ++length;

    if (length < 0x3FF)
    {
        _strncpy(buffer + length, source, 1024 - length);
        buffer[0x3FF] = 0;
    }

    length = 0;
    while (buffer[length] != 0)
        ++length;

    if (length < 0x3FF)
    {
        _strncpy(buffer + length, &g_StringDelimiter, 1024 - length);
        buffer[0x3FF] = 0;
    }

    return CoreStringFormat_WriteBufferedString(self, nullptr, buffer, argument);
}

HOOK(0x006953B0, S_00694cd0_WriteDelimitedString, "auto");
