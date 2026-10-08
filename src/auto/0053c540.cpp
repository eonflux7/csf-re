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

// Global state accessor (cdecl, no arguments; the caller's pushed argument is not popped).
CSF_FUNC(void*, __cdecl, GetGlobalState, 0x00521BB0, ());
// Global settings accessor whose byte at +0xD3 is the demo flag (cdecl, no arguments).
CSF_FUNC(void*, __cdecl, GetGlobalSettings, 0x0075FDF0, ());
// Receives the raw dword of the countdown timer (ret 4).
CSF_FUNC(void, __fastcall, GlobalState_StoreRawDword, 0x005231D0, (void* self, void* edx, uint32_t rawBits));
// Switches the demo-exit screen to the campaign menu (thiscall, ret 0).
CSF_FUNC(int, __fastcall, CMenuDemoExit_SwitchToCampaign, 0x0053C4A0, (void* self, void* edx));

static const uint32_t kNegativeOneBits = 0xBF800000u;  // bit pattern of -1.0f

static uint32_t FloatBitsOf(float value)
{
    uint32_t bits;
    std::memcpy(&bits, &value, sizeof bits);
    return bits;
}

// Counts the demo-exit countdown down by deltaSeconds; once it reaches 15 s, leaves to the campaign menu
// (or the pending menu, if one is set) and disarms the timer by setting it to -1.0f.
static void __fastcall CMenuDemoExit_UpdateCountdown(CMenuDemoExit* self, void* /*edx*/, float deltaSeconds)
{
    // The original pushes the raw timer dword before calling GetGlobalState, then passes it to the callee.
    uint32_t timerBitsBeforeCall = FloatBitsOf(self->m_timer);
    void* globalState = GetGlobalState();
    GlobalState_StoreRawDword(globalState, nullptr, timerBitsBeforeCall);

    void* settings = GetGlobalSettings();
    if (CSF_AT(uint8_t, settings, 0xD3) != 0)
        return;

    if (FloatBitsOf(self->m_timer) == kNegativeOneBits)
        return;

    // Accumulate in double (equivalent to the x87 comparison against 15.0 for these operands).
    double elapsed = static_cast<double>(deltaSeconds) + static_cast<double>(self->m_timer);
    self->m_timer = static_cast<float>(elapsed);

    // Ordered comparison: NaN does not pass, matching the fcomp/fnstsw test.
    if (!(elapsed >= 15.0))
        return;

    void* pendingMenu = reinterpret_cast<void*>(self->m_pPendingMenu);
    if (pendingMenu != nullptr && CSF_AT(uint8_t, pendingMenu, 0) != 0)
    {
        CMenuDemoExit_SwitchToCampaign(self, nullptr);
        self->m_timer = -1.0f;
    }
    else
    {
        void* manager = reinterpret_cast<void*>(self->m_pManager);
        self->m_pPendingMenu = 0;
        // Virtual slot at +0x64 -> index 25; called with two stack arguments (both 0).
        using ManagerVirtualFn = void(__fastcall*)(void*, void*, int, int);
        reinterpret_cast<ManagerVirtualFn>(CSF_AT(void**, manager, 0)[0x64 / 4])(manager, nullptr, 0, 0);
        self->m_timer = -1.0f;
    }
}

HOOK(0x0053C540, CMenuDemoExit_UpdateCountdown, "auto");
