#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_008319c0_GetSubObjectField74, 0x004760A0, (C_008319c0* self, void*));
CSF_FUNC(bool, __fastcall, ActionTargetIsUsable, 0x004781E0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CCharActionFsm_ResolveTargetFromSlot, 0x00485960, (CCharActionFsm* self, void*));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

CSF_GLOBAL(float, g_ProgressScale, 0x0082E03C);      // 0.01f
CSF_GLOBAL(float, g_MaxPercent, 0x0082E120);         // 100.0f

// Refreshes the availability flags of the character's actions (field_02e4) and
// the "above threshold" status bit (field_02e0 & 0x800).
static char __fastcall C_00833ee8_UpdateAvailableActionFlags(C_00833ee8* self, void* /*edx*/)
{
    CCharActionFsm* actionFsm = reinterpret_cast<CCharActionFsm*>(self->field_0044);

    C_008319c0* indexedEntry = (C_008319c0*)actionFsm->vGetIndexedEntryOrZero();
    C_008319c0* slotTarget = (C_008319c0*)CCharActionFsm_ResolveTargetFromSlot(actionFsm, nullptr);

    char ownerFlag = CSF_AT(char, reinterpret_cast<void*>(self->field_004c), 0x88);

    uint32_t firstListBegin = self->field_0300;
    if (firstListBegin && ((int)(self->field_0304 - firstListBegin) >> 3) != 0)
        self->field_02e4 |= 0x200u;

    uint32_t secondListBegin = self->field_0310;
    if (secondListBegin && ((int)(self->field_0314 - secondListBegin) >> 3) != 0)
        self->field_02e4 |= 0x400u;

    double percent;
    if (CSF_AT(char, reinterpret_cast<void*>(GetGlobalState_50D190()), 208))
        percent = CSF_AT(float, reinterpret_cast<void*>(GetGlobalValue_8BC1E0()), 6300);
    else
        percent = CSF_AT(float, reinterpret_cast<void*>(GetGlobalValue_8BC1E0()), 6256);

    if (percent > (double)g_MaxPercent)
        percent = (double)g_MaxPercent;

    // Computed in double: the product must not overflow in single precision.
    double scaledAmount = percent * (double)self->field_02ec * (double)g_ProgressScale;

    uint32_t statusFlags = self->field_02e0;
    if (scaledAmount > (double)self->field_02f0)
        statusFlags |= 0x800u;
    else
        statusFlags &= 0xFFFFF7FFu;
    self->field_02e0 = statusFlags;

    if (self->field_03de)
        self->field_02e4 &= 0xFFF1F805u;

    uint32_t result = self->field_02e0 & 8;
    if (result && ownerFlag)
        self->field_02e4 &= ~8u;

    if (result || !ownerFlag)
    {
        result = self->field_02e4;
        if ((result & 0x100) != 0)
        {
            if (slotTarget)
            {
                result = (uint32_t)C_008319c0_GetSubObjectField74(slotTarget, nullptr);
                if (result == 28)
                    self->field_02e4 &= ~0x100u;
            }
        }
    }

    if (indexedEntry)
    {
        if ((self->field_02e4 & 0x40) != 0)
        {
            bool usable = ActionTargetIsUsable(indexedEntry, nullptr);
            result = (uint32_t)usable;
            if (!usable || (self->field_02e0 & 2) != 0)
                self->field_02e4 &= ~0x40u;
        }
    }

    return (char)result;
}

HOOK(0x00499270, C_00833ee8_UpdateAvailableActionFlags, "auto");
