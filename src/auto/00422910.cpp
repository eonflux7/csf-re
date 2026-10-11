#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, CharTask_FinishStep, 0x00421CA0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CharTask_ArriveAtTarget, 0x004DB440, (void* self, void* edx, int a2));
CSF_FUNC(float*, __fastcall, Vec3_NormalizeHelper, 0x006AB830, (void* self, void* edx, float a2));

CSF_GLOBAL(float, g_ArrivalEpsilon, 0x0082E89C);

// The original's 0x1C-byte frame block, whose address is handed to the virtual call:
// position copy at +0x00, direction vector at +0x0C, distance to the target at +0x18.
struct MoveStepFrame {
    uint32_t position[3];
    float direction[3];
    float distance;
};

static uint32_t* __fastcall C_00834fa8_MoveTowardTarget(C_00834fa8* self, void* /*edx*/, float deltaTime)
{
    MoveStepFrame frame;
    frame.position[0] = 0;
    frame.position[1] = 0;
    frame.position[2] = 0;

    uint8_t* obj = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, self, 0x8));

    frame.direction[0] = CSF_AT(float, obj, 0x224) - CSF_AT(float, obj, 0x218);
    frame.direction[1] = CSF_AT(float, obj, 0x228) - CSF_AT(float, obj, 0x21C);
    frame.direction[2] = CSF_AT(float, obj, 0x22C) - CSF_AT(float, obj, 0x220);
    frame.distance = sqrtf(frame.direction[0] * frame.direction[0] +
                           frame.direction[1] * frame.direction[1] +
                           frame.direction[2] * frame.direction[2]);

    Vec3_NormalizeHelper(frame.direction, nullptr, frame.distance);

    obj = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, self, 0x8));
    float step = deltaTime * CSF_AT(float, obj, 0x200);

    CSF_AT(float, obj, 0x218) = frame.direction[0] * step + CSF_AT(float, obj, 0x218);
    CSF_AT(float, obj, 0x21C) = frame.direction[1] * step + CSF_AT(float, obj, 0x21C);
    CSF_AT(float, obj, 0x220) = frame.direction[2] * step + CSF_AT(float, obj, 0x220);

    if (step >= frame.distance - g_ArrivalEpsilon) {
        obj = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, self, 0x8));
        CSF_AT(uint32_t, obj, 0x218) = CSF_AT(uint32_t, obj, 0x224);
        CSF_AT(uint32_t, obj, 0x21C) = CSF_AT(uint32_t, obj, 0x228);
        CSF_AT(uint32_t, obj, 0x220) = CSF_AT(uint32_t, obj, 0x22C);
        CharTask_ArriveAtTarget(self, nullptr, (int)(CSF_AT(uint32_t, self, 0xC) + 0x3C));
    }

    obj = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, self, 0x8));
    frame.position[0] = CSF_AT(uint32_t, obj, 0x218);
    frame.position[1] = CSF_AT(uint32_t, obj, 0x21C);
    frame.position[2] = CSF_AT(uint32_t, obj, 0x220);

    uint8_t* owner = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, obj, 0x14));
    uint8_t* inner = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, owner, 0x4C));
    void** vtbl = CSF_AT(void**, inner, 0);
    ((void(__fastcall*)(void*, void*, void*))vtbl[0x50 / 4])(inner, nullptr, &frame);

    return CharTask_FinishStep(obj, nullptr);
}

HOOK(0x00422910, C_00834fa8_MoveTowardTarget, "auto");
