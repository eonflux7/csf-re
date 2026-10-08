#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Forwards the event to the virtual slot 0x48 of the menu (default handler path).
static void __fastcall CMenuAccounts_ForwardKeyEventVirtual(CMenuAccounts* self, void* /*edx*/, int* event)
{
    using Fn = void(__fastcall*)(void*, void*, int*);
    Fn handler = reinterpret_cast<Fn>(CSF_AT(void**, self, 0)[0x48 / 4]);
    handler(self, nullptr, event);
}

// Handles a key event (type 13) on the accounts screen: maps the currently
// focused child control (returned by the object at +0x3C, vtable slot 0x104)
// to a pending-action flag, then forwards the event to the virtual handler.
static void __fastcall CMenuAccounts_HandleKeyEvent(CMenuAccounts* self, void* /*edx*/, int* event)
{
    if (*event != 13)
        return;

    void* focusedObj = reinterpret_cast<void*>(self->field_003c);
    using GetFocusFn = uint32_t(__fastcall*)(void*, void*);
    GetFocusFn getFocus = reinterpret_cast<GetFocusFn>(CSF_AT(void**, focusedObj, 0)[0x104 / 4]);
    uint32_t focused = getFocus(focusedObj, nullptr);

    if (focused == self->field_0018)
    {
        self->field_002c = 1;
        CMenuAccounts_ForwardKeyEventVirtual(self, nullptr, event);
        return;
    }

    if (focused == self->field_0020 ||
        (self->field_0014 != 0 && self->field_0014 == static_cast<uint32_t>(event[1])))
    {
        CSF_AT(uint8_t*, self, 0x2E)[0] = 1;
    }
    else
    {
        if (focused == self->field_0024)
        {
            self->field_002f = 1;
            CMenuAccounts_ForwardKeyEventVirtual(self, nullptr, event);
            return;
        }
        if (focused == self->field_001c)
        {
            self->field_002d = 1;
            CMenuAccounts_ForwardKeyEventVirtual(self, nullptr, event);
            return;
        }
    }

    CMenuAccounts_ForwardKeyEventVirtual(self, nullptr, event);
}

HOOK(0x564DF0, CMenuAccounts_HandleKeyEvent, "auto");
