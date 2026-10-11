#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, sub_4E1F40, 0x004E1F40, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_558150, 0x00558150, ());

static int __fastcall C_0083e8c8_CaptureGlobalValueIfEnabled(C_0083e8c8* self, void* /*edx*/) {
    sub_4E1F40(self, nullptr);

    int result = GetGlobalState_50D190();
    if (CSF_AT(uint8_t, reinterpret_cast<uint8_t*>(result), 0xD0)) {
        int globalValueObject = GetGlobalValue_558150();
        result = CSF_AT(int, reinterpret_cast<uint8_t*>(globalValueObject), 0x1A0);

        // The original stores the raw dword into the float field (bit copy).
        uint32_t valueBits = static_cast<uint32_t>(result);
        std::memcpy(&self->field_0034, &valueBits, sizeof valueBits);
        self->field_0038 = 0;
        self->field_003c = 0xFFFFFFFFu;
    }
    return result;
}

HOOK(0x004E26D0, C_0083e8c8_CaptureGlobalValueIfEnabled, "auto");
