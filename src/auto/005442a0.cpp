#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Callees bound by address.
CSF_FUNC(int, __fastcall, CMenuProfiles_ResetCampaignControls, 0x0053B970, (void* self, void* edx));
CSF_FUNC(void, __fastcall, MenuProfiles_UpdateTrucoFlag, 0x0053C070, (void* self, void* edx));

// Original: int __thiscall CMenuSaveGame::sub_5442A0(void)
// Notifies the sub-object at +0x28 (vtbl slot 1) with the value at +0x30,
// resets the save session and the campaign controls, then calls the
// object at +0x24 (vtbl slot 42, byte offset 0xA8) with 0 and returns its result.
// Finally clears the field at +0x38 to -1.
static int __fastcall CMenuSaveGame_ShutdownAndReturnResult(void* self, void* /*edx*/)
{
    // Argument reads happen in the same order as the original.
    void* notifyTarget = CSF_AT(void*, self, 0x28);
    uint32_t notifyValue = CSF_AT(uint32_t, self, 0x30);
    ((void(__fastcall*)(void*, void*, uint32_t))CSF_AT(void**, notifyTarget, 0)[1])(
        notifyTarget, nullptr, notifyValue);

    MenuProfiles_UpdateTrucoFlag(self, nullptr);
    CMenuProfiles_ResetCampaignControls(self, nullptr);

    void* resultTarget = CSF_AT(void*, self, 0x24);
    int result = ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, resultTarget, 0)[168 / 4])(
        resultTarget, nullptr, 0);

    CSF_AT(int32_t, self, 0x38) = -1;
    return result;
}

HOOK(0x005442A0, CMenuSaveGame_ShutdownAndReturnResult, "auto");
