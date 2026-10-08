#include <cstdint>
#include "types/auto_classes.h"
#include "hook.h"
#include "globals.h"

// 0x00540030 - __thiscall(void* this, uint32_t* a4), ret 4 - processes the incoming tab command
CSF_FUNC(void, __fastcall, CMenuMissionInfo_DispatchCommand, 0x540030, (CMenuMissionInfo* self, void* /*edx*/, uint32_t* a4));

// 0x00540380 - __thiscall(CMenuMissionInfo* this, uint32_t* a4), ret 4
// Inspects the command id at *a4; only id 13 (tab switch) is forwarded to the handler.
static void __fastcall CMenuMissionInfo_HandleCommand(CMenuMissionInfo* self, void* /*edx*/, uint32_t* a4)
{
    if (*a4 == 13)
        CMenuMissionInfo_DispatchCommand(self, nullptr, a4);
}

HOOK(0x540380, CMenuMissionInfo_HandleCommand, "auto");
