#include <cstdint>
#include "types/auto_classes.h"
#include "hook.h"

// Returns a pointer to the character data of the MSVC std::string stored at +0x54.
// Small-string optimization: if the capacity at +0x68 is below 0x10 the internal
// buffer (the string object itself at +0x54) is used, otherwise the heap pointer
// stored at +0x54 is dereferenced.
static char* __fastcall C_00864288_GetStringData(C_00864288* self, void* /*edx*/)
{
    if (self->field_0068 < 0x10)
        return reinterpret_cast<char*>(reinterpret_cast<uint8_t*>(self) + 0x54);
    return reinterpret_cast<char*>(self->field_0054);
}

HOOK(0x4379D0, C_00864288_GetStringData, "auto");
