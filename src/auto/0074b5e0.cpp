#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __stdcall, NetGameDdl_FinishFieldProcessing, 0x0065E710, (int a1, int a2));

static void __stdcall NetGhostDdl_ProcessFields(int objectAddress, int mode)
{
    void* object = reinterpret_cast<void*>(objectAddress);

    if (mode == 0 || mode == 1)
    {
        using ProcessField = void (__fastcall*)(void*, void*, int);

        reinterpret_cast<ProcessField>(CSF_AT(void**, object, 0)[7])(object, nullptr, 1);
        reinterpret_cast<ProcessField>(CSF_AT(void**, object, 0)[7])(object, nullptr, 2);
        reinterpret_cast<ProcessField>(CSF_AT(void**, object, 0)[7])(object, nullptr, 3);
        reinterpret_cast<ProcessField>(CSF_AT(void**, object, 0)[7])(object, nullptr, 4);
        reinterpret_cast<ProcessField>(CSF_AT(void**, object, 0)[7])(object, nullptr, 5);
        reinterpret_cast<ProcessField>(CSF_AT(void**, object, 0)[7])(object, nullptr, 6);
    }

    NetGameDdl_FinishFieldProcessing(objectAddress, mode);
}

HOOK(0x0074B5E0, NetGhostDdl_ProcessFields, "auto");
