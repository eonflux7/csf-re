#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(uint32_t*, __cdecl, CharActionFsm_CreateAction, 0x00475A40,
         (int a1, int a2, int a3));

static char __fastcall CharActionFsm_EnsureAction(
    void* self, void* /*edx*/, int actionId, int parameter)
{
    void* context = reinterpret_cast<void*>(GetValue8B0554());
    void* registry = CSF_AT(void*, context, 0x1FC0);
    void* definition =
        ((void* (__fastcall*)(void*, void*, int))
         CSF_AT(void**, registry, 0)[2])(registry, nullptr, actionId);

    switch (CSF_AT(int, definition, 0x74))
    {
    case 23:
        if (!CSF_AT(uint32_t*, self, 0x8C))
        {
            CSF_AT(uint32_t*, self, 0x8C) =
                CharActionFsm_CreateAction(1, actionId, CSF_AT(int, self, 0x08));
        }
        return 1;

    case 27:
    {
        uint32_t* action = CSF_AT(uint32_t*, self, 0x88);
        if (!action)
        {
            CSF_AT(uint32_t*, self, 0x88) =
                CharActionFsm_CreateAction(1, actionId, CSF_AT(int, self, 0x08));
        }
        else if (parameter == -1)
        {
            ((void (__fastcall*)(void*, void*, int))
             CSF_AT(void**, action, 0)[24])(action, nullptr, 0);
        }
        else
        {
            ((void (__fastcall*)(void*, void*, int))
             CSF_AT(void**, action, 0)[23])(action, nullptr, parameter);
        }
        return 1;
    }

    case 31:
        if (!CSF_AT(uint32_t*, self, 0x84))
        {
            CSF_AT(uint32_t*, self, 0x84) =
                CharActionFsm_CreateAction(1, actionId, CSF_AT(int, self, 0x08));
        }
        return 1;

    default:
        return 0;
    }
}

HOOK(0x004828D0, CharActionFsm_EnsureAction, "auto");
