#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Callee bound by address; signature copied from the packet's bind line.
CSF_FUNC(int, __fastcall, sub_4DB140, 0x004DB140, (void* self, void* edx, int a2));

// Forwards to 0x4DB140 with the address of the embedded sub-object at +0x180 (pad_0020[352]).
static int __fastcall C_0082f9f8_ForwardEmbeddedSubobjectCall(C_0082f9f8* self, void* /*edx*/) {
    return sub_4DB140(self, nullptr, reinterpret_cast<int>(reinterpret_cast<uint8_t*>(self) + 0x180));
}

HOOK(0x004212E0, C_0082f9f8_ForwardEmbeddedSubobjectCall, "auto");
