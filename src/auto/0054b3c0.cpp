#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static CSF_FUNC(bool, __stdcall, TypeRegistry_IsNameBefore, 0x0054ABF0,
                (char* a1, char* a2));
static CSF_FUNC(int*, __fastcall, TypeRegistry_FindLowerBound, 0x0054AFB0,
                (void* self, void* edx, void** a2));

static int** __fastcall TypeRegistry_FindByName(
    S_0054afb0* self, void* /*edx*/, int** result, const char** name)
{
    int* candidate = TypeRegistry_FindLowerBound(self, nullptr, (void**)name);
    int* end = reinterpret_cast<int*>(self->field_0004);

    if (candidate == end ||
        TypeRegistry_IsNameBefore(
            const_cast<char*>(*name), CSF_AT(char*, candidate, 0x0C)))
    {
        *result = end;
    }
    else
    {
        *result = candidate;
    }
    return result;
}

HOOK(0x0054B3C0, TypeRegistry_FindByName, "auto");
