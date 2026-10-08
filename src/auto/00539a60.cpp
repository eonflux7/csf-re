#include <cstdint>
#include "types/auto_classes.h"
#include "hook.h"
#include "globals.h"

// 0x005399A0 - __thiscall(CMenuLoadGame* this, uint32_t* a2), ret 4 - handler for command id 13
CSF_FUNC(uint32_t*, __fastcall, CMenuLoadGame_SelectSaveRecordAndRefresh, 0x5399A0, (CMenuLoadGame* self, void* /*edx*/, uint32_t* a2));

// 0x00539A60 - __thiscall(CMenuLoadGame* this, uint32_t* a2), ret 4
// Inspects the first dword of the incoming command; only id 13 is forwarded to the handler,
// otherwise the command pointer itself is returned untouched.
static uint32_t* __fastcall CMenuLoadGame_HandleCommand(CMenuLoadGame* self, void* /*edx*/, uint32_t* a2)
{
    if (*a2 == 13)
        return CMenuLoadGame_SelectSaveRecordAndRefresh(self, nullptr, a2);
    return a2;
}

HOOK(0x539A60, CMenuLoadGame_HandleCommand, "auto");
