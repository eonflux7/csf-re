#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, TaskState_Finish, 0x00421B30, (void* self, void* edx, float a2, char a3));
CSF_FUNC(char, __fastcall, Owner_CanEnterState, 0x0048E5D0, (void* self, void* edx, int a2, int a3, int a4));
CSF_FUNC(int, __fastcall, Task_GetSubObject, 0x004DB140, (void* self, void* edx, int a2));
CSF_FUNC(void*, __fastcall, SubObject_Resolve, 0x004DB190, (void* self, void* edx, int a2));

static int __fastcall C_0082f9f8_Start(C_0082f9f8* self, void* /*edx*/, float a2, char a3)
{
    int result;

    CSF_AT(uint8_t, self, 0x2B8) = 0;
    self->field_0018 &= 0xFFFFFFF9;

    void* child = reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x10));
    if (child) {
        result = ((int(__fastcall*)(void*, void*))CSF_AT(void**, child, 0)[0x80 / 4])(child, nullptr);
        if (result == 7)
            return result;
    }

    child = reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x10));
    if (child) {
        result = ((int(__fastcall*)(void*, void*))CSF_AT(void**, child, 0)[0x80 / 4])(child, nullptr);
        if (result != 0)
            return result;
    }

    if (a3) {
        char ok = Owner_CanEnterState(reinterpret_cast<void*>(self->field_0014), nullptr, (int)self, 2, 1);
        if (!ok)
            return 0;
    }

    uint8_t* owner = reinterpret_cast<uint8_t*>(self->field_0014);
    CSF_AT(int, self, 0x1FC) = 2;
    CSF_AT(uint8_t, self, 0x230) = 0;
    CSF_AT(float, self, 0x200) = 0.0f;

    switch (CSF_AT(int, owner, 0x1D8)) {
    case 0:
    case 1:
        CSF_AT(float, self, 0x200) = 200.0f;
        break;
    case 2:
    case 3:
        CSF_AT(float, self, 0x200) = 600.0f;
        break;
    default:
        break;
    }

    int sub = Task_GetSubObject(self, nullptr, (int)((uint8_t*)self + 0x60));
    uint8_t* subPtr = reinterpret_cast<uint8_t*>(sub);
    void* resolved = SubObject_Resolve(subPtr, nullptr, CSF_AT(int, subPtr, 8) + 424);
    result = TaskState_Finish(resolved, nullptr, a2, a3);
    self->field_0018 |= 2u;
    return result;
}

HOOK(0x004226C0, C_0082f9f8_Start, "auto");
