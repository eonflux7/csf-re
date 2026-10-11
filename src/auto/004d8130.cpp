#include <cstdint>
#include "globals.h"
#include "hook.h"

// Original: 0x004DB110, __thiscall-style helper with no stack args.
CSF_FUNC(int, __fastcall, CharTaskBase_Prepare, 0x004DB110, (void* self, void* edx));

// Clears bit 0 of the flags word at inner->+0x18, then calls virtual slot 45
// (byte offset 0xB4) of the object at inner->+0x14 with argument 1.
static int __fastcall TCharTask_PrepareAndActivateInner(void* self, void* /*edx*/) {
    CharTaskBase_Prepare(self, nullptr);

    void* inner = CSF_AT(void*, self, 0x8);
    uint32_t flags = CSF_AT(uint32_t, inner, 0x18);
    CSF_AT(uint32_t, inner, 0x18) = flags & 0xFFFFFFFEu;

    void* activeObj = CSF_AT(void*, CSF_AT(void*, self, 0x8), 0x14);
    using SlotFn = int(__fastcall*)(void*, void*, int);
    SlotFn fn = (SlotFn)CSF_AT(void**, activeObj, 0)[0xB4 / 4];
    return fn(activeObj, nullptr, 1);
}

HOOK(0x004D8130, TCharTask_PrepareAndActivateInner, "auto");
