#include <cstdint>
#include "globals.h"
#include "hook.h"

// Hands two 4-dword parameter blocks taken from a2 to the object stored at this+0x08
// (vtable slot 7 first, then slot 6 twice), then records a2[6] at this+0x14 and
// clears this+0x10. Returns the result of the second slot-6 call.
static int __fastcall C_73B650_SubmitParamBlocks(void* self, void* /*edx*/, int* a2)
{
    // Slot 7 on the target object, no arguments.
    void* obj = CSF_AT(void*, self, 0x08);
    ((void(__fastcall*)(void*, void*))CSF_AT(void**, obj, 0)[7])(obj, nullptr);

    // First parameter block: a2[9..12] -> buf, then slot 6 with (buf, 0).
    int buf[4];
    buf[0] = a2[9];
    buf[1] = a2[10];
    buf[2] = a2[11];
    buf[3] = a2[12];
    obj = CSF_AT(void*, self, 0x08);
    ((void(__fastcall*)(void*, void*, int*, int))CSF_AT(void**, obj, 0)[6])(obj, nullptr, buf, 0);

    // Second parameter block: a2[13..16] -> buf, then slot 6 with (buf, a2[6]).
    buf[0] = a2[13];
    buf[1] = a2[14];
    buf[2] = a2[15];
    buf[3] = a2[16];
    obj = CSF_AT(void*, self, 0x08);
    int result = ((int(__fastcall*)(void*, void*, int*, int))CSF_AT(void**, obj, 0)[6])(obj, nullptr, buf, a2[6]);

    CSF_AT(int, self, 0x10) = 0;
    CSF_AT(int, self, 0x14) = a2[6];
    return result;
}

HOOK(0x73B650, C_73B650_SubmitParamBlocks, "auto");
