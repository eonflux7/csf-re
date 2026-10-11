#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Stores the facing pair in the FSM object, then forwards both values to the
// provider's facing interface. The provider is obtained from this+4 via vtable slot 4.
static int __fastcall CharActionFsm_SetProviderFacing(void* self, void* /*edx*/, int a2, int a3, int a4, int a5)
{
    CSF_AT(int, self, 0x84) = a2;
    CSF_AT(int, self, 0x88) = a3;
    CSF_AT(int, self, 0xA4) = a4;
    CSF_AT(int, self, 0xA8) = a5;

    void* provider = CSF_AT(void*, self, 0x4);
    void* facing = ((void* (__fastcall*)(void*, void*))CSF_AT(void**, provider, 0)[4])(provider, nullptr);

    ((void (__fastcall*)(void*, void*, int))CSF_AT(void**, facing, 0)[12])(facing, nullptr, CSF_AT(int, self, 0x84));
    return ((int (__fastcall*)(void*, void*, int))CSF_AT(void**, facing, 0)[13])(facing, nullptr, CSF_AT(int, self, 0x88));
}

HOOK(0x4F5040, CharActionFsm_SetProviderFacing, "auto");
