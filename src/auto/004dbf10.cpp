#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));

// Destructor of C_0083def8 (TAiState). Frees the owned block at +0x14, releases the
// child object at +0x0C through its slot 0, restores the base vtable and optionally
// frees the object itself (a2 & 1). Returns the object address.
static uint32_t* __fastcall C_0083def8_Destroy(C_0083def8* self, void* /*edx*/, char a2) {
    if (self->field_0014) {
        CoreMemory_Free(reinterpret_cast<void*>(self->field_0014));
        self->field_0014 = 0;
    }

    void* ownedObj = reinterpret_cast<void*>(self->field_000c);
    self->vtbl = CSF_VTBL(0x0083DEF8);
    if (ownedObj) {
        using SlotZeroFn = void(__fastcall*)(void*, void*, int);
        SlotZeroFn slotZero = reinterpret_cast<SlotZeroFn>(CSF_AT(void**, ownedObj, 0)[0]);
        slotZero(ownedObj, nullptr, 1);
        self->field_000c = 0;
    }

    self->vtbl = CSF_VTBL(0x0082D410);
    if ((a2 & 1) != 0) {
        CoreMemory_Free(reinterpret_cast<void*>(self));
    }
    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x004DBF10, C_0083def8_Destroy, "auto");
