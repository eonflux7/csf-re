#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "types/auto_classes.h"

// Original: 0x0042D6D0, __cdecl, no args, returns the HUD global pointer.
CSF_FUNC(int32_t, __cdecl, GetGlobal8afc78, 0x0042D6D0, ());

// Original: 0x00431F10, __thiscall, one stack arg (key), ret 4.
CSF_FUNC(int, __fastcall, CHud_LookupValueByKey, 0x00431F10, (CHud* self, void* /*edx*/, int key));

// Original: 0x00458840, __thiscall, two stack args, ret 8.
CSF_FUNC(char, __fastcall, sub_458840, 0x00458840, (void* self, void* /*edx*/, int a2, uint32_t* a3));

// Returns the HUD lookup result for key a2 when the status icon table is non-empty
// and the entry check at 0x458840 passes; otherwise returns 0.
static int __fastcall HudStatusIcons_TryLookupStatusValue(S_00458840* self, void* /*edx*/, int a2, uint32_t* a3)
{
    uint32_t entriesBegin = self->field_002c;
    if (entriesBegin == 0)
        return 0;

    // Element count: signed (end - begin) / 20, as in the original.
    int entryCount = static_cast<int>(self->field_0030 - entriesBegin) / 20;
    if (entryCount == 0)
        return 0;

    if (!sub_458840(self, nullptr, a2, a3))
        return 0;

    auto* hud = reinterpret_cast<CHud*>(GetGlobal8afc78());
    return CHud_LookupValueByKey(hud, nullptr, a2);
}

HOOK(0x004588D0, HudStatusIcons_TryLookupStatusValue, "auto");
