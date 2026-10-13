#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Forwards to the vtable slot 0x38 of the embedded sub-object at +0x04,
// passing the address of the +0x08 member and the caller's argument.
static int __fastcall C_0083dfa0_ForwardToEmbeddedObject(C_0083dfa0* self, void* /*edx*/, int a2) {
    void* embedded = self ? reinterpret_cast<uint8_t*>(self) + 4 : nullptr;
    void* memberAddr = reinterpret_cast<uint8_t*>(self) + 8;
    return ((int(__fastcall*)(void*, void*, void*, int))CSF_AT(void**, embedded, 0)[0x38 / 4])(
        embedded, nullptr, memberAddr, a2);
}

HOOK(0x4DB920, C_0083dfa0_ForwardToEmbeddedObject, "auto");
