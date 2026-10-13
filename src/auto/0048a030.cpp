#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_00833708_ResolveEligibleControl, 0x00489ED0, (C_00833708* self, void* /*edx*/, int index));

// Starts the secondary order for the entry at `index` in the table at this->field_000c (96-byte entries).
static int __fastcall AutoTask_StartSecondaryOrder(C_00833708* self, void* /*edx*/, int a2)
{
    uint8_t* entry = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(self->field_000c) + 96u * static_cast<uint32_t>(a2));
    void* control = reinterpret_cast<void*>(C_00833708_ResolveEligibleControl(self, nullptr, a2));

    if (control) {
        if (CSF_AT(uint32_t, control, 0x50) != 0) {
            // Virtual slot 35 (+0x8C): thiscall (ecx = control), four stack args (8, 0, -1, 0).
            ((void(__fastcall*)(void*, void*, int, int, int, int))CSF_AT(void**, control, 0)[35])(
                control, nullptr, 8, 0, -1, 0);
        }
        CSF_AT(uint32_t, entry, 0x54) = (CSF_AT(uint32_t, control, 0x78) != 1) ? 1u : 0u;
    }

    // Entry's object at +0: virtual slot 16 (+0x40). The original loads the object into ecx
    // and pushes one argument (entry + 0x38), so this is a thiscall with one stack arg.
    void* obj = CSF_AT(void*, entry, 0);
    return ((int(__fastcall*)(void*, void*, void*))CSF_AT(void**, obj, 0)[16])(
        obj, nullptr, entry + 0x38);
}

HOOK(0x0048A030, AutoTask_StartSecondaryOrder, "auto");
