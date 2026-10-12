#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, ForwardTarget_7A3310, 0x007A3310, (void* self, void* edx, int a1, int a2));

static int __fastcall C_008678a8_ProcessPlayerFields(
    C_008678a8* self, void* /*edx*/, int streamAddress, int mode)
{
    if (mode == 0 || mode == 1)
    {
        void* stream = reinterpret_cast<void*>(streamAddress);
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, stream, 0)[7])(stream, nullptr, 4);
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, stream, 0)[7])(stream, nullptr, 5);
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, stream, 0)[7])(stream, nullptr, 6);
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, stream, 0)[7])(stream, nullptr, 7);
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, stream, 0)[7])(stream, nullptr, 8);
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, stream, 0)[7])(stream, nullptr, 9);
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, stream, 0)[7])(stream, nullptr, 10);
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, stream, 0)[7])(stream, nullptr, 11);
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, stream, 0)[7])(stream, nullptr, 12);
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, stream, 0)[7])(stream, nullptr, 13);
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, stream, 0)[7])(stream, nullptr, 14);
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, stream, 0)[7])(stream, nullptr, 15);
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, stream, 0)[7])(stream, nullptr, 16);
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, stream, 0)[7])(stream, nullptr, 17);
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, stream, 0)[7])(stream, nullptr, 18);
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, stream, 0)[7])(stream, nullptr, 19);
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, stream, 0)[7])(stream, nullptr, 20);
    }

    return ForwardTarget_7A3310(self, nullptr, streamAddress, mode);
}

HOOK(0x00744C80, C_008678a8_ProcessPlayerFields, "auto");
