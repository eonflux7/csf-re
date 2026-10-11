#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, sub_4DB0F0, 0x004DB0F0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, sub_4DB440, 0x004DB440, (void* self, void* edx, int a2));

static char __fastcall TNavegando_UpdateStateFlag(TNavegando* self, void* /*edx*/, int a2) {
    sub_4DB0F0(self, nullptr, a2);

    char flagValue = static_cast<char>(CSF_AT(uint8_t, self, 0x7C));
    if (!flagValue) {
        uint32_t ownerOffset = CSF_AT(uint32_t, self, 0x0C);
        int adjustedValue = static_cast<int>(ownerOffset + 32);
        return static_cast<char>(
            sub_4DB440(reinterpret_cast<C_00834fa8*>(self), nullptr, adjustedValue));
    }
    return flagValue;
}

HOOK(0x420EF0, TNavegando_UpdateStateFlag, "auto");
