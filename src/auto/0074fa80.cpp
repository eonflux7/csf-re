#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __stdcall, NetGameDdl_FinishFieldProcessing, 0x0065E710, (int a1, int a2));

static void __stdcall NetPostmanDdl_ProcessFields(int processorAddress, int mode)
{
    void* processor = reinterpret_cast<void*>(processorAddress);

    if (mode == 0 || mode == 1)
    {
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, processor, 0)[0x1C / 4])(processor, nullptr, 1);
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, processor, 0)[0x1C / 4])(processor, nullptr, 2);
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, processor, 0)[0x1C / 4])(processor, nullptr, 3);
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, processor, 0)[0x1C / 4])(processor, nullptr, 4);
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, processor, 0)[0x1C / 4])(processor, nullptr, 5);
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, processor, 0)[0x1C / 4])(processor, nullptr, 6);
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, processor, 0)[0x1C / 4])(processor, nullptr, 7);
    }

    NetGameDdl_FinishFieldProcessing(processorAddress, mode);
}

HOOK(0x0074FA80, NetPostmanDdl_ProcessFields, "auto");
