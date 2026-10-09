#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Callee at 0x497FB0: __thiscall with no stack args, ret 0.
CSF_FUNC(void, __fastcall, sub_497FB0, 0x00497FB0, (void* self, void* edx));

// Runs the object's 0x497FB0 routine, then returns the address of the
// embedded sub-object at offset 0x1C8 (lea eax, [esi+0x1c8]).
static char* __fastcall C_00833ee8_RefreshAndGetSubobject1C8(C_00833ee8* self, void* /*edx*/)
{
    sub_497FB0(self, nullptr);
    return reinterpret_cast<char*>(reinterpret_cast<uint8_t*>(self) + 0x1C8);
}

HOOK(0x498A30, C_00833ee8_RefreshAndGetSubobject1C8, "auto");
