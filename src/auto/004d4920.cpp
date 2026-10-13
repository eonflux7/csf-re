#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(void, __fastcall, CharTaskBase_Init_4DB3C0, 0x004DB3C0, (void* self, void* edx));

static void __fastcall C_0082d410_ResetTaskSlots(C_0082d410* self, void* /*edx*/) {
    CharTaskBase_Init_4DB3C0(reinterpret_cast<uint8_t*>(self) + 100, nullptr);
    CharTaskBase_Init_4DB3C0(reinterpret_cast<uint8_t*>(self) + 80, nullptr);
    CharTaskBase_Init_4DB3C0(reinterpret_cast<uint8_t*>(self) + 60, nullptr);
    CharTaskBase_Init_4DB3C0(reinterpret_cast<uint8_t*>(self) + 40, nullptr);
    CharTaskBase_Init_4DB3C0(reinterpret_cast<uint8_t*>(self) + 20, nullptr);
    CharTaskBase_Init_4DB3C0(self, nullptr);
}

HOOK(0x004D4920, C_0082d410_ResetTaskSlots, "auto");
