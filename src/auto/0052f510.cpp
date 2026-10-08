#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// int __thiscall sub_52F140(int this): no stack args, returns int.
CSF_FUNC(int, __fastcall, CMenuMpLoadout_ClearActiveSelection, 0x0052F140, (CMenuMpLoadout* self, void* edx));

// Handles a key event for CMenuMpLoadout. Only VK_RETURN (13) is handled:
// if the stored selection id (field_0034) is nonzero and equals the event's
// second dword, the selection is cleared; then the virtual slot at +0x48 is
// invoked with the event regardless.
static void __fastcall CMenuMpLoadout_HandleEnterKey(CMenuMpLoadout* self, void* /*edx*/, uint32_t* event)
{
    if (event[0] == 13)
    {
        uint32_t selectedId = self->field_0034;
        if (selectedId != 0)
        {
            if (selectedId == event[1])
                CMenuMpLoadout_ClearActiveSelection(self, nullptr);
        }
        ((void(__fastcall*)(void*, void*, uint32_t*))CSF_AT(void**, self, 0)[0x48 / 4])(self, nullptr, event);
    }
}

HOOK(0x0052F510, CMenuMpLoadout_HandleEnterKey, "auto");
