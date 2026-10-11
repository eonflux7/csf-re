#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, WeaponDefs_IsFireModeSpecial, 0x007612C0, (int a1));

static int __fastcall C_008316d8_ResolveCachedEntry(C_008316d8* self, void* /*edx*/)
{
    int result = static_cast<int>(self->field_002c);
    if (result)
        return result;

    void* lookup = reinterpret_cast<void*>(self->vResolveEntryLookupValue());
    int entryKey = 100;
    if (!lookup)
    {
        void* owner = reinterpret_cast<void*>(self->field_0004);
        lookup = reinterpret_cast<void*>(
            ((int(__fastcall*)(void*, void*))CSF_AT(void**, owner, 0)[0x28 / 4])
            (owner, nullptr));
        entryKey = 2100;
    }

    if (lookup)
    {
        int index =
            ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, lookup, 0)[0x98 / 4])
            (lookup, nullptr, entryKey);
        if (index != -1)
        {
            result =
                ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, lookup, 0)[0x8C / 4])
                (lookup, nullptr, index);
            self->field_002c = result;
            return result;
        }
    }

    result = WeaponDefs_IsFireModeSpecial(
        CSF_AT(int, reinterpret_cast<void*>(self->field_0040), 0x74));
    self->field_002c = 0;
    return result;
}

HOOK(0x004771D0, C_008316d8_ResolveCachedEntry, "auto");
