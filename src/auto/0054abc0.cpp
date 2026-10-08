#include <cstdint>

#include "hook.h"
#include "types/auto_classes.h"
#include "globals.h"

// CMenuMpGameMode::BotonDMatch handler (vtable 0x00845CA8).
//
// If the widget currently linked at +0x10 exists and its +0x04 "id/owner" field
// matches the one carried by the argument object, that widget is deselected
// (virtual slot 46 with argument 0) and then this screen's own virtual slot 19
// is invoked, whose result is returned. Otherwise the linked pointer itself is
// returned untouched (0 when nothing is linked).
static int __fastcall CMenuMpGameMode_HandleLinkedItemChange(
    CMenuMpGameMode* self, void* /*edx*/, int a2)
{
    uint32_t linkedItem = self->field_0010;
    if (linkedItem)
    {
        if (linkedItem == *reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(a2) + 4))
        {
            void** linkedVtable = CSF_AT(void**, reinterpret_cast<void*>(linkedItem), 0);
            (reinterpret_cast<void(__fastcall*)(void*, void*, int)>(linkedVtable[46]))(
                reinterpret_cast<void*>(linkedItem), nullptr, 0);

            void** selfVtable = CSF_AT(void**, self, 0);
            return (reinterpret_cast<int(__fastcall*)(void*, void*)>(selfVtable[19]))(self, nullptr);
        }
    }
    return static_cast<int>(linkedItem);
}

HOOK(0x0054ABC0, CMenuMpGameMode_HandleLinkedItemChange, "auto");
