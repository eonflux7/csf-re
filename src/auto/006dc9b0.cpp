#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_6C5760, 0x006C5760, ());

// Forwards a call to virtual slot 29 (+0x74) of the global render-device object
// (GlobalValue + 0xF0), passing this + 0x20 and two zero arguments. Returns 0 when
// the self flag at +0x94 is clear.
static char __fastcall C_00861508_ForwardToGlobalDeviceIfEnabled(C_00861508* self, void* /*edx*/) {
    char result = 0;
    if (self->field_0094) {
        uint8_t* globalBase = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(GetGlobalValue_6C5760()));
        void* renderDevice = CSF_AT(void*, globalBase, 0xF0);
        auto slotFn = reinterpret_cast<char (__fastcall*)(void*, void*, uint8_t*, int, int)>(
            CSF_AT(void**, renderDevice, 0)[0x74 / 4]);
        return slotFn(renderDevice, nullptr, reinterpret_cast<uint8_t*>(self) + 0x20, 0, 0);
    }
    return result;
}

HOOK(0x6DC9B0, C_00861508_ForwardToGlobalDeviceIfEnabled, "auto");
