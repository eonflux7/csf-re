#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// char *__cdecl sub_631950(int a1, char *a2)
CSF_FUNC(char*, __cdecl, RwTexDictionaryFindNamedTexture, 0x631950, (int dict, const char* name));

// Looks up a named texture in the object's texture dictionary; returns 0 if the name is null/empty.
static int __fastcall C_008655e8_FindNamedTexture(C_008655e8* self, void* /*edx*/, const char* name)
{
    if (name && *name)
        return (int)RwTexDictionaryFindNamedTexture((int)self->field_002c, name);
    return 0;
}

HOOK(0x00726AD0, C_008655e8_FindNamedTexture, "auto");
