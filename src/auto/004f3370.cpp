#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, HudStringShow_UpdateSubtitleEntries, 0x004F1F00,
         (void* self, void* edx, float a2));
CSF_FUNC(int, __fastcall, HudStringShow_RefreshSubtitlePanel, 0x004F3290,
         (void* self, void* edx));
CSF_FUNC(int, __fastcall, HudStringShow_UpdateSubtitleLayout, 0x004F1FF0,
         (void* self, void* edx));
CSF_FUNC(int, __fastcall, HudStringShow_FinalizeSubtitlePanel, 0x004F22A0,
         (void* self, void* edx));

static int __fastcall S_004f1f00_UpdateSubtitlePanel(
    S_004f1f00* self, void* /*edx*/, float elapsedTime)
{
    const uint32_t entriesBegin = self->field_0008;
    if (!entriesBegin)
        return 0;

    const int32_t byteCount =
        static_cast<int32_t>(self->field_000c - entriesBegin);
    const int entryCount = byteCount / 124;
    if (!entryCount)
        return entryCount;

    HudStringShow_UpdateSubtitleEntries(self, nullptr, elapsedTime);
    HudStringShow_RefreshSubtitlePanel(self, nullptr);
    HudStringShow_UpdateSubtitleLayout(self, nullptr);
    return HudStringShow_FinalizeSubtitlePanel(self, nullptr);
}

HOOK(0x004F3370, S_004f1f00_UpdateSubtitlePanel, "auto");
