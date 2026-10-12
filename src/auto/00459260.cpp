#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// The packet's bind-as line lists 3 parameters, but the disassembly pushes 4
// arguments and cleans up with `add esp, 0x10`. The 4th argument is the
// message pointer (a2), so the 4-argument form is used here.
CSF_FUNC(uint32_t*, __cdecl, sub_4583D0, 0x004583D0, (char* a1, char* a2, uint32_t* a3, int a4));

// Removes the entry whose key (+0x20) matches the message's key (+0x0C),
// shifting the following entries down and re-stamping the freed tail slot
// with the C_0082d410 vtable.
static void __fastcall C_008313dc_RemoveEntryMatchingId(C_008313dc* self, void* /*edx*/, int a2)
{
    char* msg = reinterpret_cast<char*>(a2);
    if (CSF_AT(uint32_t, msg, 4) != 8)
        return;

    uint32_t entryCount = 0;
    if (self->field_000c)
        entryCount = (uint32_t)((int)(self->field_0010 - self->field_000c) / 40);

    if (entryCount == 0)
        return;

    char* base = reinterpret_cast<char*>(self->field_000c);
    uint32_t idx = 0;
    while (CSF_AT(uint32_t, base + 40 * idx, 0x20) != CSF_AT(uint32_t, msg, 12))
    {
        if (++idx >= entryCount)
            return;
    }

    char* entry = base + 40 * idx;
    sub_4583D0(entry + 40, reinterpret_cast<char*>(self->field_0010), reinterpret_cast<uint32_t*>(entry), a2);

    uint32_t end = self->field_0010;
    uint32_t slotAddr = end - 40;
    do
    {
        CSF_AT(void*, reinterpret_cast<char*>(slotAddr), 0x1C) = CSF_VTBL(0x0082D410);
        slotAddr += 40;
    } while (slotAddr != end);

    self->field_0010 -= 40;
}

HOOK(0x00459260, C_008313dc_RemoveEntryMatchingId, "auto");
