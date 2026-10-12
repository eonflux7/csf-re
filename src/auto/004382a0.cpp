#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Callee bindings
CSF_FUNC(int, __cdecl, MenuMessageBox_ResolveEntryByKey, 0x00692440, (int code, int flag));

// Sets the enabled flag of the HUD child object (field_0004). The child's
// virtual slot 16 (+0x40) is called with the resolved entry; the HUD then
// marks itself disabled, clears its vtable pointer and clears field_0010.
static int __fastcall CHud_SetChildEnabled(CHud* self, void* /*edx*/, char enable)
{
    int result = static_cast<int>(self->field_0004);
    if (result != 0)
    {
        int entry = MenuMessageBox_ResolveEntryByKey((enable != 0) + 1903505457, 1);
        void* child = reinterpret_cast<void*>(self->field_0004);
        result = ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, child, 0)[0x40 / 4])(child, nullptr, entry);
        *reinterpret_cast<uint8_t*>(&self->field_0014) = 1;
        CSF_AT(uint32_t, self, 0) = 0;
        self->field_0010 = 0;
    }
    return result;
}

HOOK(0x4382A0, CHud_SetChildEnabled, "auto");
