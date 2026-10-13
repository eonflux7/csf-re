#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(bool, __fastcall, sub_4CDDD0, 0x004CDDD0, (void* self, void* edx));
CSF_FUNC(char, __fastcall, sub_4CDF30, 0x004CDF30, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_0083ac40_BuildMovementPath, 0x004D5940, (C_0083ac40* self, void*));

// Leaves the previous state's resource (if any) and switches the task to newState.
static char __fastcall C_0083ac40_SetTaskState(C_0083ac40* self, void* /*edx*/, int newState)
{
    uint8_t* parent = reinterpret_cast<uint8_t*>(self->field_0014);
    uint32_t previousState = self->field_0070;

    if (previousState == 0)
    {
        // parent->field_0044 object: vtbl slot 188 / 4 = 47, argument 14
        void* controller = reinterpret_cast<void*>(CSF_AT(uint32_t, parent, 0x44));
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, controller, 0)[47])(controller, nullptr, 14);
    }
    else if (previousState == 1)
    {
        // parent->field_0030 object: vtbl slot 160 / 4 = 40
        void* mover = reinterpret_cast<void*>(CSF_AT(uint32_t, parent, 0x30));
        ((void(__fastcall*)(void*, void*))CSF_AT(void**, mover, 0)[40])(mover, nullptr);
    }

    self->field_0070 = static_cast<uint32_t>(newState);
    char result = static_cast<char>(newState - 1);

    switch (newState)
    {
    case 1:
    {
        // parent->field_0030 object: vtbl slot 172 / 4 = 43, args (&field_0078, 1)
        void* mover = reinterpret_cast<void*>(CSF_AT(uint32_t, reinterpret_cast<uint8_t*>(self->field_0014), 0x30));
        result = ((char(__fastcall*)(void*, void*, uint32_t*, int))CSF_AT(void**, mover, 0)[43])(
            mover, nullptr, &CSF_AT(uint32_t, self, 0x78), 1);
        break;
    }
    case 2:
        result = sub_4CDDD0(self, nullptr);
        break;
    case 3:
        result = static_cast<char>(C_0083ac40_BuildMovementPath(self, nullptr));
        break;
    case 4:
        result = sub_4CDF30(self, nullptr);
        break;
    case 6:
        self->field_0018 &= ~1u;
        break;
    default:
        return result;
    }
    return result;
}

HOOK(0x004CE350, C_0083ac40_SetTaskState, "auto");
