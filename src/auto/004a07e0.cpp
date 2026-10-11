#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_008343a4_FindMatchingEntry, 0x004A0080, (C_008343a4* self, void* , void* referenceObject));
CSF_FUNC(void, __fastcall, HudAlertIcons_ProcessMatchingEntry, 0x004A0490, (void* self, void* edx, uint32_t* a2));

[[maybe_unused]] static const void* const kEntryVtableReference = CSF_VTBL(0x004A0490);

static void __fastcall C_008343a4_ForwardType19Entry(
    C_008343a4* self, void* /*edx*/, int messageAddress)
{
    auto* message = reinterpret_cast<uint8_t*>(messageAddress);
    if (CSF_AT(int, message, 0x04) == 19)
    {
        int matchingEntry =
            C_008343a4_FindMatchingEntry(self, nullptr, message + 8);
        if (matchingEntry != 0)
        {
            HudAlertIcons_ProcessMatchingEntry(
                self, nullptr, reinterpret_cast<uint32_t*>(matchingEntry));
        }
    }
}

HOOK(0x004A07E0, C_008343a4_ForwardType19Entry, "auto");
