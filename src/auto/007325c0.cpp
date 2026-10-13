#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(unsigned int, __fastcall, sub_732610, 0x00732610, (void* self, void* edx, char* a2, unsigned int arg4, int* arg8));

// Computes the index of pointer a3 inside the element array at self+4 (16-byte elements),
// calls sub_732610 with (a3, 1, a4), then writes the element address for that index into *a2.
static int* __fastcall InsertAtPointerAndWriteIndex(void* self, void* /*edx*/, int* a2, void* a3, int a4)
{
    int32_t index = 0;
    int32_t base = CSF_AT(int32_t, self, 0x4);
    if (base != 0 && ((CSF_AT(int32_t, self, 0x8) - base) >> 4) != 0)
        index = (static_cast<int32_t>(reinterpret_cast<intptr_t>(a3)) - base) >> 4;

    sub_732610(self, nullptr, reinterpret_cast<char*>(a3), 1u,
               reinterpret_cast<int*>(static_cast<intptr_t>(a4)));

    *a2 = CSF_AT(int32_t, self, 0x4) + index * 16;
    return a2;
}

HOOK(0x7325C0, InsertAtPointerAndWriteIndex, "auto");
