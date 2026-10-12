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

CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, RenderHandle_FromObject, 0x006B2D50, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GetRtDictSchema, 0x006D6360, (void* self, void*));
CSF_FUNC(int, __cdecl, RpTeamPlayerGetUserData, 0x007FF0C0, (int a1));

// Returns true when the player's index (from its RpTeam slot) lies inside the
// range reported by the render handle. Two paths depending on the selector
// returned by this->vtbl[4]: selector == 1 compares against the handle's
// count; otherwise compares the player's user data against the field at +0x4.
static bool __fastcall C_008630e0_IsPlayerInRange(C_008630e0* self, void* /*edx*/)
{
    int globalInt = GetGlobalInt();
    int renderHandle = RenderHandle_FromObject(reinterpret_cast<void*>(globalInt), nullptr);
    void* renderObj = reinterpret_cast<void*>(renderHandle);

    // renderObj->vtbl[23] (+0x5C): returns the player info object.
    int playerInfo = reinterpret_cast<int (__fastcall*)(void*, void*)>(
        CSF_AT(void**, renderObj, 0)[23])(renderObj, nullptr);
    void* info = reinterpret_cast<void*>(playerInfo);

    // self->vtbl[4] (+0x10): selector.
    int selector = reinterpret_cast<int (__fastcall*)(void*, void*)>(
        CSF_AT(void**, self, 0)[4])(self, nullptr);

    if (selector == 1)
    {
        // renderObj->vtbl[22] (+0x58): count.
        uint32_t count = reinterpret_cast<uint32_t (__fastcall*)(void*, void*)>(
            CSF_AT(void**, renderObj, 0)[22])(renderObj, nullptr);
        uint32_t infoValue = CSF_AT(uint32_t, info, 8);
        return infoValue < count;
    }

    int rtDictSchema = GetRtDictSchema(renderObj, nullptr);
    int userDataRaw = RpTeamPlayerGetUserData(rtDictSchema);
    uint32_t userData = static_cast<uint32_t>(userDataRaw);

    uint16_t slotIndex = CSF_AT(uint16_t, reinterpret_cast<void*>(self->field_0004), 0xC);

    // renderObj->vtbl[20] (+0x50): total count (no arguments).
    uint32_t total = reinterpret_cast<uint32_t (__fastcall*)(void*, void*)>(
        CSF_AT(void**, renderObj, 0)[20])(renderObj, nullptr);

    uint32_t infoIndex = CSF_AT(uint32_t, info, 4);
    if (!(infoIndex < total))
        return false;
    return userData >= static_cast<uint32_t>(slotIndex);
}

HOOK(0x006FF600, C_008630e0_IsPlayerInRange, "auto");
