#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Advances the byte counter in this object by 2, then forwards the 1-byte
// argument (padded to 2 bytes) to the stream object at field_0004 through its
// vtable slot 3 (+0x0C). Returns non-zero only if the stream accepted exactly 2 bytes.
static int __fastcall C_0085e760_AdvanceCountAndWrite2Bytes(C_0085e760* self, void* /*edx*/, char a2)
{
    self->field_0008 += 2;

    void* stream = reinterpret_cast<void*>(self->field_0004);
    auto writeSlot3 = reinterpret_cast<int(__fastcall*)(void*, void*, char*, int)>(
        CSF_AT(void**, stream, 0)[3]);
    int written = writeSlot3(stream, nullptr, &a2, 2);

    return written == 2 ? 1 : 0;
}

HOOK(0x0069A160, C_0085e760_AdvanceCountAndWrite2Bytes, "auto");
