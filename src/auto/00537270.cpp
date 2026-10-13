#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __fastcall, C_00843720_ReleaseSubobjects, 0x00537130, (void* self, void* edx));
CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));

static char* __fastcall C_00843720_DeleteFromSecondaryBase(
    char* self, void* /*edx*/, char flags)
{
    char* object = self - 4;
    C_00843720_ReleaseSubobjects(object, nullptr);
    if ((flags & 1) != 0)
        CoreMemory_Free(object);
    return object;
}

HOOK(0x00537270, C_00843720_DeleteFromSecondaryBase, "auto");
