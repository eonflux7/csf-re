#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// int __thiscall sub_54ABA0(std::uint32_t *this, int a2)   -> ret 4
CSF_FUNC(int, __fastcall, CMenuMpGameMode_SetBackButton, 0x54ABA0, (void* self, void* edx, int a2));
// int __thiscall sub_53BF50(int this)               -> ret 0
CSF_FUNC(int, __fastcall, CMenuProfiles_SyncButtonsToProfileCount, 0x53BF50, (void* self, void* edx));

// 0x0053C170: forwards the incoming action argument to 0x54ABA0 (with this as ecx)
// and then runs the menu re-evaluation at 0x53BF50, whose result is returned.
static int __fastcall CMenuProfiles_ProcessActionAndRefresh(CMenuProfiles* self, void* /*edx*/, int a2)
{
    CMenuMpGameMode_SetBackButton(self, nullptr, a2);
    return CMenuProfiles_SyncButtonsToProfileCount(self, nullptr);
}

HOOK(0x53C170, CMenuProfiles_ProcessActionAndRefresh, "auto");
