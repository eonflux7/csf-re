#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, GlobalInt_ResolveAccessor, 0x006B13A0, (void* self, void* edx));
CSF_FUNC(char, __cdecl, GfxSkin_ProbeEntryHandle, 0x006E2F10, (int a1));

// Returns 1 if any entry in the list at a1+0x18 has its flag byte (+7) set to 0xFF and the
// probe on its first dword does not accept it; otherwise 0. Entries whose flag byte is below 0xFF
// are reported immediately.
static char __cdecl GfxSkin_ListHasUnresolvedEntry(int a1)
{
    if (!a1)
        return 0;

    int globalInt = GetGlobalInt();
    int accessor = GlobalInt_ResolveAccessor(reinterpret_cast<void*>(globalInt), nullptr);
    void* accessorObj = reinterpret_cast<void*>(accessor);
    reinterpret_cast<void (__fastcall*)(void*, void*)>(CSF_AT(void**, accessorObj, 0)[4])(accessorObj, nullptr);

    int list = CSF_AT(int, a1, 0x18);
    if (list)
    {
        int count = CSF_AT(int, list, 0x24);
        int index = 0;
        if (count > 0)
        {
            do
            {
                int entry = CSF_AT(int*, CSF_AT(int, list, 0x20), 0)[index];
                if (CSF_AT(uint8_t, entry, 7) != 0xFF)
                    return 1;
                if (GfxSkin_ProbeEntryHandle(CSF_AT(int, entry, 0)))
                    return 1;
                ++index;
            } while (index < count);
            return 0;
        }
    }
    return 0;
}

HOOK(0x006C4D50, GfxSkin_ListHasUnresolvedEntry, "auto");
