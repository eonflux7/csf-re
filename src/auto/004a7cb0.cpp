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

CSF_FUNC(int, __fastcall, CPlayerCharacter_PreparePlacement, 0x00493250, (void* self, void* edx));
CSF_FUNC(char, __fastcall, C_0082dd78_StoreByteAndSetFlag, 0x0040FD20, (C_0082dd78* self, void* edx, char a2));
CSF_FUNC(void, __fastcall, TFncGirar_OnRotationComplete, 0x004DB1D0, (TFncGirar* self, void* edx, int a2));

// Owner character stored at this+0x08; its player object lives at owner+0x118.
static void* TFncGirar_GetOwnerPlayer(TFncGirar* self)
{
    void* owner = reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x08));
    return reinterpret_cast<void*>(CSF_AT(uint32_t, owner, 0x118));
}

static void __fastcall TFncGirar_BeginRotation(TFncGirar* self, void* /*edx*/)
{
    CPlayerCharacter_PreparePlacement(TFncGirar_GetOwnerPlayer(self), nullptr);

    void* player = TFncGirar_GetOwnerPlayer(self);
    C_0082dd78_StoreByteAndSetFlag(
        reinterpret_cast<C_0082dd78*>(CSF_AT(uint32_t, player, 0x4C)),
        nullptr,
        1);

    // Virtual slot 0x8C / 4 = 35 on the player object.
    player = TFncGirar_GetOwnerPlayer(self);
    ((void(__fastcall*)(void*, void*, int, int, int, int))CSF_AT(void**, player, 0)[0x8C / 4])(
        player, nullptr, 0, 0, -1, 0);

    TFncGirar_OnRotationComplete(self, nullptr, 0);
}

HOOK(0x4A7CB0, TFncGirar_BeginRotation, "auto");
