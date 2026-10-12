#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Game heap free, called when the string storage was heap-allocated (capacity >= 16).
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// std::exception destructor (__thiscall), reached by tail jump.
CSF_FUNC(void, __fastcall, sub_7AF6FB, 0x007AF6FB, (void* self, void* edx));

// Releases the heap buffer of C_008769d4 (if any), resets it to the inline/empty state,
// then tail-calls the base std::exception destructor.
static void __fastcall C_008769d4_ReleaseBase(C_008769d4* self, void* /*edx*/)
{
    self->vtbl = CSF_VTBL(0x008769D4);

    if (self->field_0024 >= 0x10)
    {
        Mem_Free(reinterpret_cast<void*>(self->field_0010));
    }

    self->field_0024 = 15;
    self->field_0020 = 0;
    *reinterpret_cast<uint8_t*>(&self->field_0010) = 0;

    sub_7AF6FB(self, nullptr);
}

HOOK(0x00403E10, C_008769d4_ReleaseBase, "auto");
