#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, DestroyBlockElements, 0x00782A70,
         (int* a1, int* a2, int a3, int a4));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

static char __fastcall ReleaseSubBufferHandle(S_00782f90* self, void* /*edx*/)
{
    int result = static_cast<int>(self->field_0004);
    if (result != 0)
    {
        DestroyBlockElements(reinterpret_cast<int*>(result),
                             reinterpret_cast<int*>(self->field_0008),
                             reinterpret_cast<int>(self), 0);
        result = static_cast<int>(self->field_0004);
        if (result != 0)
            result = Mem_FreeDebug(reinterpret_cast<void*>(result));
    }

    self->field_0004 = 0;
    self->field_0008 = 0;
    self->field_000c = 0;
    return static_cast<char>(result);
}

HOOK(0x00783090, ReleaseSubBufferHandle, "auto");
