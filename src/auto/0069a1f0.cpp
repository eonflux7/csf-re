#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Writes one byte through the stream object held in field_0004 (vtable slot 3, count 1).
// Returns 1 when the stream reports exactly one item written, 0 otherwise.
static int __fastcall C_0085e760_WriteOneByteAndCheckSuccess(C_0085e760* self, void* /*edx*/, char a2) {
    ++self->field_0008;

    void* streamObject = reinterpret_cast<void*>(self->field_0004);
    int itemsWritten = ((int(__fastcall*)(void*, void*, char*, int))CSF_AT(void**, streamObject, 0)[3])(
        streamObject, nullptr, &a2, 1);

    return (itemsWritten == 1) ? 1 : 0;
}

HOOK(0x69A1F0, C_0085e760_WriteOneByteAndCheckSuccess, "auto");
