#include <cstdint>
#include "globals.h"
#include <cstring>

#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(unsigned int, __fastcall, sub_408700, 0x00408700, (void* self, void* edx, void* a2));

// Tail-forwards to 0x408700 with the same `this` and the float's raw bits as the stack argument.
static unsigned int __fastcall C_0082d648_ForwardFloatArgToHandler(C_0082d648* self, void* /*edx*/, float a2) {
    uint32_t argBits = 0;
    std::memcpy(&argBits, &a2, sizeof(argBits));
    return sub_408700(self, nullptr, reinterpret_cast<void*>(static_cast<uintptr_t>(argBits)));
}

HOOK(0x0040A340, C_0082d648_ForwardFloatArgToHandler, "auto");
