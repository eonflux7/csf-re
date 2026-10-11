#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, SetSignedInputMagnitude_4117B0, 0x004117B0, (void* self, void* edx, float a2));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_753A30, 0x00753A30, ());

static void __fastcall TaskState_SelectSpeedAndSetInput(void* self, void* /*edx*/, float magnitude)
{
    uint8_t* owner = CSF_AT(uint8_t*, self, 0x14);
    uint8_t* motion = CSF_AT(uint8_t*, owner, 0x4C);

    if (CSF_AT(int, owner, 0x50) == 0) {
        int flags = CSF_AT(int, owner, 0x2E0);
        int offAlt = -1;
        int offNormal = -1;

        if ((flags & 0x40) != 0) {
            offAlt = 0x1AC;
            offNormal = 0x180;
        } else if ((flags & 0x10000) != 0) {
            offAlt = 0x1A8;
            offNormal = 0x17C;
        } else if ((flags & 2) != 0) {
            offAlt = 0x1A0;
            offNormal = 0x174;
        } else if (CSF_AT(int, owner, 0x1C4) == 1) {
            offAlt = 0x1A4;
            offNormal = 0x178;
        }

        if (offAlt >= 0) {
            uint8_t* state = reinterpret_cast<uint8_t*>(GetGlobalState_50D190());
            if (CSF_AT(uint8_t, state, 0xD0) != 0) {
                uint8_t* gv = reinterpret_cast<uint8_t*>(GetGlobalValue_753A30());
                CSF_AT(float, motion, 0x110) = CSF_AT(float, gv, offAlt);
            } else {
                uint8_t* gv = reinterpret_cast<uint8_t*>(GetGlobalValue_753A30());
                CSF_AT(float, motion, 0x110) = CSF_AT(float, gv, offNormal);
            }
        } else {
            CSF_AT(float, motion, 0x110) = 1.0f;
        }
    }

    SetSignedInputMagnitude_4117B0(motion, nullptr, magnitude);
    CSF_AT(int, self, 0x24) = 2;
}

HOOK(0x0041FEC0, TaskState_SelectSpeedAndSetInput, "auto");
