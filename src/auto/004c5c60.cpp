#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, CoverAttack_SetMode_47B210, 0x0047B210, (void* self, void* edx, int a2));
CSF_FUNC(unsigned int, __fastcall, CoverAttack_Sub_4B9BA0, 0x004B9BA0, (void* self, void* edx));
CSF_FUNC(bool, __fastcall, CoverAttack_Sub_4C4890, 0x004C4890, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CoverAttack_SetState_4DC150, 0x004DC150, (void* self, void* edx, int a2));

static int __fastcall CoverAttack_EnterState(void* self, void* /*edx*/, void* aiState)
{
    uint8_t* base = reinterpret_cast<uint8_t*>(self);

    int linked = GetLinkedObjectValue_4DB5C0(aiState, nullptr);
    uint8_t* linkedObj = linked ? reinterpret_cast<uint8_t*>(linked) - 4 : nullptr;
    void* modeTarget = CSF_AT(void*, linkedObj, 0x718);
    CoverAttack_SetMode_47B210(modeTarget, nullptr, 2);

    CoverAttack_Sub_4C4890(base + 0x5D8, nullptr);

    uint8_t* owner = CSF_AT(uint8_t*, base, 0x28);
    CSF_AT(int, base, 0x684) = CSF_AT(int, owner, 0x1D8);

    CoverAttack_Sub_4B9BA0(base - 4, nullptr);

    unsigned int mode = static_cast<unsigned int>(CSF_AT(int, base, 0x684));
    uint8_t* owner2 = CSF_AT(uint8_t*, base, 0x28);
    CSF_AT(uint8_t, base, 0x6C8) = 0;
    CSF_AT(uint8_t, base, 0x69C) = (mode != 0) ? 1 : 0;
    CSF_AT(int, base, 0x6D8) = 0;

    uint8_t* sub = CSF_AT(uint8_t*, owner2, 0x34);
    if (!CSF_AT(uint8_t, sub, 0x39)) {
        return CoverAttack_SetState_4DC150(aiState, nullptr, reinterpret_cast<int>(base + 0x34));
    }

    switch (mode) {
    case 0:
        return CoverAttack_SetState_4DC150(aiState, nullptr, reinterpret_cast<int>(base + 0x38));
    case 1:
        return CoverAttack_SetState_4DC150(aiState, nullptr, reinterpret_cast<int>(base + 0x4C));
    case 2:
        return CoverAttack_SetState_4DC150(aiState, nullptr, reinterpret_cast<int>(base + 0x80));
    case 3:
        return CoverAttack_SetState_4DC150(aiState, nullptr, reinterpret_cast<int>(base + 0x108));
    default:
        return static_cast<int>(mode);
    }
}

HOOK(0x004C5C60, CoverAttack_EnterState, "auto");
