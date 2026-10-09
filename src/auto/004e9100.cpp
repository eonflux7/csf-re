#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, Handle_GetValuePointer, 0x00476060, (void* self, void* edx));
CSF_FUNC(int, __cdecl, IsResolvedEntryActive, 0x00755880, (int a1));

// Returns the state/type value (+0x78) of the object selected from the entry's container,
// choosing the slot by the entry kind when that kind is active.
static int __stdcall SceneMissionSetup_GetEntryLinkedObjectState(void* entry)
{
    int result = 0;

    if (!entry)
        return 0;

    int state = CSF_AT(int, entry, 0x78);
    if (state == 0 || state == 2)
        return 3;

    void* container = CSF_AT(void*, entry, 0x44);
    if (!container)
        return result;

    unsigned int count =
        ((unsigned int(__fastcall*)(void*, void*))CSF_AT(void**, container, 0)[0xA0 / 4])(container, nullptr);
    if (count == 0)
        return result;

    void* handle;
    if ((uint8_t)IsResolvedEntryActive(CSF_AT(int, entry, 0x50)))
    {
        int slotKind;
        switch (CSF_AT(int, entry, 0x50))
        {
        case 4:
            slotKind = 30;
            break;
        case 5:
            slotKind = 13;
            break;
        case 6:
        case 7:
            slotKind = 14;
            break;
        case 0x13:
            slotKind = 20;
            break;
        default:
            return result;
        }

        int index =
            ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, container, 0)[0xBC / 4])(container, nullptr, slotKind);
        if (index == -1)
            return result;

        handle =
            ((void*(__fastcall*)(void*, void*, int))CSF_AT(void**, container, 0)[0xC8 / 4])(container, nullptr, index);
    }
    else
    {
        handle =
            ((void*(__fastcall*)(void*, void*, int))CSF_AT(void**, container, 0)[0xC8 / 4])(container, nullptr, 0);
    }

    if (handle)
    {
        int value = Handle_GetValuePointer(handle, nullptr);
        result = CSF_AT(int, value, 0x78);
    }

    return result;
}

HOOK(0x004E9100, SceneMissionSetup_GetEntryLinkedObjectState, "auto");
