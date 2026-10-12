#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static CSF_FUNC(void*, __cdecl, ReleaseGenericBlock, 0x00471B50, (void* Block));
static CSF_FUNC(int, __cdecl, ReleaseTypedBlock, 0x004727E0, (void* Block));
static CSF_FUNC(int, __cdecl, ReleaseTypeOneBlock, 0x00472970, (void* Block));

static char* __fastcall C_008313f8_ReleaseBlock(C_008313f8* self, void* /*edx*/)
{
    char* block = reinterpret_cast<char*>(self->field_0004);
    if (!block)
        return block;

    const char type = *block;
    if (type == 23)
        return reinterpret_cast<char*>(ReleaseTypedBlock(block));
    if (type == 1)
        return reinterpret_cast<char*>(ReleaseTypeOneBlock(block));
    if (type == 37 || type == 18 || type == 11)
        return reinterpret_cast<char*>(ReleaseTypedBlock(block));

    char* result = static_cast<char*>(ReleaseGenericBlock(block));
    self->field_0004 = 0;
    return result;
}

HOOK(0x00472AB0, C_008313f8_ReleaseBlock, "auto");
