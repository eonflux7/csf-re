#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void, __stdcall, NetGameDdl_FinishFieldProcessing, 0x0065E710, (int a1, int a2));

static void __stdcall NetGameDdl_ProcessFields(int objectAddress, int mode)
{
    void* object = reinterpret_cast<void*>(objectAddress);

    if (mode == 0 || mode == 1)
    {
        reinterpret_cast<void (__fastcall*)(void*, void*, int)>(CSF_AT(void**, object, 0)[0x1C / 4])(object, nullptr, 1);
        reinterpret_cast<void (__fastcall*)(void*, void*, int)>(CSF_AT(void**, object, 0)[0x1C / 4])(object, nullptr, 2);
        reinterpret_cast<void (__fastcall*)(void*, void*, int)>(CSF_AT(void**, object, 0)[0x1C / 4])(object, nullptr, 3);
        reinterpret_cast<void (__fastcall*)(void*, void*, int)>(CSF_AT(void**, object, 0)[0x1C / 4])(object, nullptr, 4);
        reinterpret_cast<void (__fastcall*)(void*, void*, int)>(CSF_AT(void**, object, 0)[0x1C / 4])(object, nullptr, 5);
        reinterpret_cast<void (__fastcall*)(void*, void*, int)>(CSF_AT(void**, object, 0)[0x1C / 4])(object, nullptr, 6);
        reinterpret_cast<void (__fastcall*)(void*, void*, int)>(CSF_AT(void**, object, 0)[0x1C / 4])(object, nullptr, 7);
        reinterpret_cast<void (__fastcall*)(void*, void*, int)>(CSF_AT(void**, object, 0)[0x1C / 4])(object, nullptr, 8);
        reinterpret_cast<void (__fastcall*)(void*, void*, int)>(CSF_AT(void**, object, 0)[0x1C / 4])(object, nullptr, 9);
        reinterpret_cast<void (__fastcall*)(void*, void*, int)>(CSF_AT(void**, object, 0)[0x1C / 4])(object, nullptr, 10);
        reinterpret_cast<void (__fastcall*)(void*, void*, int)>(CSF_AT(void**, object, 0)[0x1C / 4])(object, nullptr, 11);
        reinterpret_cast<void (__fastcall*)(void*, void*, int)>(CSF_AT(void**, object, 0)[0x1C / 4])(object, nullptr, 12);
        reinterpret_cast<void (__fastcall*)(void*, void*, int)>(CSF_AT(void**, object, 0)[0x1C / 4])(object, nullptr, 13);
    }

    NetGameDdl_FinishFieldProcessing(objectAddress, mode);
}

HOOK(0x0074CA20, NetGameDdl_ProcessFields, "auto");
