#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());

static int __fastcall C_008342e8_GetDrawState(C_008342e8* self, void* /*edx*/)
{
    int valueBase = GetValue8B0554();
    uint8_t* stateOwnerBase = reinterpret_cast<uint8_t*>(static_cast<intptr_t>(valueBase));
    void* stateObject = CSF_AT(void*, stateOwnerBase, 0x1FC0);
    int stateArg = CSF_AT(int, self, 0x194);
    int result = ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, stateObject, 0)[2])(stateObject, nullptr, stateArg);
    return result + 4;
}

HOOK(0x0049CF10, C_008342e8_GetDrawState, "auto");
