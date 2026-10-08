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

// Callees (bound by address)
CSF_FUNC(int, __fastcall, ForwardToStoredObjectVirtualSlot40, 0x722DF0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CGuiListBoxBase_SetItemHeight, 0x714750, (void* self, void* edx, int a2));

static inline void* IntToPtr(int value)
{
    return reinterpret_cast<void*>(static_cast<uintptr_t>(static_cast<uint32_t>(value)));
}

// Looks up a child control by name through the vtable slot at +0xFC of the owner object.
static int LookupChildControlByName(void* owner, const char* name)
{
    void** vtbl = *reinterpret_cast<void***>(owner);
    return reinterpret_cast<int(__fastcall*)(void*, void*, const char*)>(vtbl[0xFC / 4])(owner, nullptr, name);
}

// __thiscall int(int a2, int a3), ret 8
static int __fastcall MenuMpSessions_BindListboxes(void* self, void* /*edx*/, int a2, int a3)
{
    (void)a2;
    (void)a3;

    // Partidas listbox: stored at this+0x70
    int partidasListbox = LookupChildControlByName(CSF_AT(void*, self, 0x40), CSF_STR(0x8457D8, "ListboxPartidas"));
    CSF_AT(int, self, 0x70) = partidasListbox;

    // Mapas listbox: stored at this+0x34
    int mapasListbox = LookupChildControlByName(CSF_AT(void*, self, 0x40), CSF_STR(0x8458E8, "ListboxMapas"));
    CSF_AT(int, self, 0x34) = mapasListbox;

    if (mapasListbox != 0)
    {
        int storedObject = ForwardToStoredObjectVirtualSlot40(IntToPtr(mapasListbox), nullptr);
        if (storedObject != 0)
        {
            void** vtbl = *reinterpret_cast<void***>(IntToPtr(storedObject));
            int itemHeight = reinterpret_cast<int(__fastcall*)(void*, void*, int)>(vtbl[0x18 / 4])(
                IntToPtr(storedObject), nullptr, 1);
            CGuiListBoxBase_SetItemHeight(CSF_AT(void*, self, 0x34), nullptr, itemHeight);
        }
    }

    // Re-query Partidas listbox and store the result at this+0x70
    int result = LookupChildControlByName(CSF_AT(void*, self, 0x40), CSF_STR(0x8457D8, "ListboxPartidas"));
    CSF_AT(int, self, 0x70) = result;
    return result;
}

HOOK(0x5615E0, MenuMpSessions_BindListboxes, "auto");
