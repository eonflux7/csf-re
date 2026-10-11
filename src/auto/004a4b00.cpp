#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, sub_492980, 0x00492980, (void* self, void* edx));
CSF_FUNC(int, __fastcall, sub_4929C0, 0x004929C0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, sub_4A4820, 0x004A4820, (void* self, void* edx));

static int __fastcall C_00834598_GetAnimationId(C_00834598* self, void* /*edx*/)
{
    uint8_t* owner = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(self->field_0008));
    uint8_t* typeInfo = CSF_AT(uint8_t*, owner, 0x38);
    int typeId = CSF_AT(int, typeInfo, 0x14);

    // The original's default paths return the uninitialized local at [ebp-4],
    // which holds the saved ECX (the this pointer).
    int uninitializedResult = static_cast<int>(reinterpret_cast<uintptr_t>(self));

    bool special = false;
    switch (typeId) {
    case 8:
    case 9:
    case 10:
    case 14:
    case 15:
    case 16:
        special = true;
        break;
    default:
        break;
    }

    if (special) {
        if (CSF_AT(int, owner, 0x1c4) != 0) {
            return 72;
        }
        switch (CSF_AT(uint32_t, owner, 0x1d8)) {
        case 0:
        case 1:
            return 8;
        case 2:
        case 3:
            return ((uint8_t)sub_4929C0(owner, nullptr) != 0) ? 59 : 35;
        default:
            return uninitializedResult;
        }
    }

    if (CSF_AT(int, owner, 0x1c4) != 0) {
        if ((uint8_t)sub_492980(owner, nullptr) != 0) {
            uint32_t state = self->field_0044;
            if (state == 1 || state == 3 || state == 23) {
                return 71;
            }
        }
        return 70;
    }

    switch (CSF_AT(uint32_t, owner, 0x1d8)) {
    case 0:
    case 1:
        return ((uint8_t)sub_492980(owner, nullptr) != 0 ? 1 : 0) + 6;
    case 2:
    case 3:
        if ((uint8_t)sub_4929C0(owner, nullptr) != 0 && (uint8_t)sub_4A4820(self, nullptr) != 0) {
            return 58;
        }
        if ((uint8_t)sub_492980(owner, nullptr) != 0 && (uint8_t)sub_4A4820(self, nullptr) != 0) {
            return 34;
        }
        return 33;
    default:
        return uninitializedResult;
    }
}

HOOK(0x004A4B00, C_00834598_GetAnimationId, "auto");
