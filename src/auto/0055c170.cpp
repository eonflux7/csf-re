#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Handles key events for the account edit screen. On Enter (key code 13), marks the
// screen as changed when the attached object's query (vtable slot 0x104) matches the
// stored expected value, then forwards the event through this object's vtable slot 0x48.
static void __fastcall CMenuAccountEdit_HandleKeyEvent(CMenuAccountEdit* self, void* /*edx*/, uint32_t* event)
{
    const uint32_t kKeyEnter = 13;

    if (*event != kKeyEnter)
        return;

    void* statusObject = reinterpret_cast<void*>(self->field_0284);
    uint32_t statusValue =
        ((uint32_t(__fastcall*)(void*, void*))CSF_AT(void**, statusObject, 0)[0x104 / 4])(statusObject, nullptr);

    if (statusValue == self->field_0018)
        CSF_AT(uint8_t*, self, 0x030)[0] = 1;

    ((void(__fastcall*)(void*, void*, uint32_t*))CSF_AT(void**, self, 0)[0x48 / 4])(self, nullptr, event);
}

HOOK(0x0055C170, CMenuAccountEdit_HandleKeyEvent, "auto");
