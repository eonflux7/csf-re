#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Queries the selection provider stored at this+0x894 (vtable slot 0x104) for the
// current button id, then raises the pressed/selected flag of the first button
// slot whose stored id matches. Returns the queried id unchanged.
static int __fastcall CMenuMessageBox_MarkCurrentButtonFlag(CMenuMessageBox* self, void* /*edx*/)
{
    void* selectionProvider = reinterpret_cast<void*>(self->field_0894);
    using CurrentButtonQueryFn = int(__fastcall*)(void* thisPtr, void* edx);
    CurrentButtonQueryFn queryCurrentButtonId =
        reinterpret_cast<CurrentButtonQueryFn>(CSF_AT(void**, selectionProvider, 0)[65]);

    int result = queryCurrentButtonId(selectionProvider, nullptr);
    const uint32_t buttonId = static_cast<uint32_t>(result);

    if (buttonId == self->field_002c) {
        self->field_0082 = 1;
    } else if (buttonId == self->field_0030) {
        self->field_0086 = 1;
    } else if (buttonId == self->field_0034) {
        self->field_0087 = 1;
    } else if (buttonId == self->field_0038) {
        self->field_0088 = 1;
    } else if (buttonId == self->field_003c) {
        self->field_0089 = 1;
    } else if (buttonId == self->field_0040) {
        self->field_008a = 1;
    } else if (buttonId == self->field_0044) {
        self->field_008b = 1;
    } else if (buttonId == self->field_0048) {
        self->field_008c = 1;
    } else if (buttonId == self->field_004c) {
        self->field_008d = 1;
    } else if (buttonId == self->field_0050) {
        self->field_008e = 1;
    } else if (buttonId == self->field_0054) {
        self->field_008f = 1;
    }

    return result;
}

HOOK(0x00528E70, CMenuMessageBox_MarkCurrentButtonFlag, "auto");
