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

// Gate query (cdecl, one stack arg, result tested via al).
CSF_FUNC(uint8_t, __cdecl, Sub_7A8ED0, 0x007A8ED0, (int eventCode));

// Menu sub-object handler (thiscall, no stack args), invoked on the enclosing object.
CSF_FUNC(void, __fastcall, MenuAccounts_ResetOnSelect, 0x0055D130, (void* self, void* edx));

// Handles a menu event code. `self` points at the sub-object embedded 4 bytes past
// the owning menu object, so the owner is reached via self - 4.
static void __fastcall MenuAccounts_HandleEventCode(uint32_t* self, void* /*edx*/, int eventCode, int /*unusedArg*/)
{
    uint8_t* owner = reinterpret_cast<uint8_t*>(self) - 4;

    if (Sub_7A8ED0(eventCode)) {
        // Virtual slot at byte offset 0x4C (slot 19) on the owner's vtable.
        void** vtbl = *reinterpret_cast<void***>(owner);
        ((void(__fastcall*)(void*, void*))vtbl[0x4C / 4])(owner, nullptr);
    }

    if (eventCode == 6) {
        MenuAccounts_ResetOnSelect(owner, nullptr);
    }
}

HOOK(0x0055D2A0, MenuAccounts_HandleEventCode, "auto");
