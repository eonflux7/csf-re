#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, S_00427bf0_PrepareSubmission, 0x00427BF0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, S_00427bf0_UpdateSubmissionDisplay, 0x00427CC0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, S_00427bf0_UpdateSubmissionLayout, 0x00427DA0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, S_00427bf0_UpdateSubmissionAppearance, 0x00427E10, (void* self, void* edx));
CSF_FUNC(int, __fastcall, S_00427bf0_FinishSubmission, 0x00427EE0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, HudObjectives_ResetOverlayState, 0x00428350, ());
CSF_FUNC(int32_t, __cdecl, GetGlobal8afc78, 0x0042D6D0, ());

static char __fastcall S_00427bf0_SubmitFourDwords(
    S_00427bf0* self, void* /*edx*/, const uint32_t* values)
{
    const char enabled = static_cast<char>(self->field_0008);
    if (!enabled)
        return enabled;

    self->field_0024 = values[0];
    self->field_0028 = values[1];
    self->field_002c = values[2];
    CSF_AT(uint32_t, self, 0x30) = values[3];
    self->field_0000 = 0xffffffff;
    self->field_0044 = 0;
    self->field_0009 = 0;

    CSF_AT(uint8_t, reinterpret_cast<void*>(GetGlobal8afc78()), 0x2a2) = 1;
    CSF_AT(uint8_t, reinterpret_cast<void*>(GetGlobal8afc78()), 0x2a1) = 1;
    CSF_AT(uint8_t, reinterpret_cast<void*>(GetGlobal8afc78()), 0x2a2) = 0;
    CSF_AT(uint8_t, reinterpret_cast<void*>(GetGlobal8afc78()), 0x2a1) = 0;

    self->field_0009 = 1;
    HudObjectives_ResetOverlayState();

    // Packet-listed binding; the disassembly performs a direct call, not a vtable store.
    (void)CSF_VTBL(0x00427BF0);
    S_00427bf0_PrepareSubmission(self, nullptr);
    S_00427bf0_UpdateSubmissionDisplay(self, nullptr);
    S_00427bf0_UpdateSubmissionLayout(self, nullptr);
    S_00427bf0_UpdateSubmissionAppearance(self, nullptr);
    return static_cast<char>(S_00427bf0_FinishSubmission(self, nullptr));
}

HOOK(0x004283E0, S_00427bf0_SubmitFourDwords, "auto");
