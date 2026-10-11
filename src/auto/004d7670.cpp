#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, SceneObj_IsInactive, 0x0048E100, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneObject_PassesFirstCheck, 0x0048E120, (void* self, void* edx));
CSF_FUNC(int, __fastcall, HasNonzeroLinkedValueDifferentFrom, 0x0048E680, (void* self, void* , int comparisonValue));
CSF_FUNC(int, __fastcall, IsField1BCNonNegative, 0x00492B10, (void* self, void*));
CSF_FUNC(int, __fastcall, CharActionFsm_IsWeaponActionBlocked, 0x00492BD0, (void* self, void* edx));

// Returns true when the autonomous task's owner character is in a state that allows it to proceed.
static char __fastcall AutonomousTask_CanProceed(void* self, void* /*edx*/)
{
    char result = 0;

    void* owner = CSF_AT(void*, self, 8);
    void* character = CSF_AT(void*, owner, 0x14);
    if ((uint8_t)HasNonzeroLinkedValueDifferentFrom(character, nullptr, (int)(intptr_t)owner))
        return result;

    character = CSF_AT(void*, CSF_AT(void*, self, 8), 0x14);
    if ((uint8_t)CharActionFsm_IsWeaponActionBlocked(character, nullptr))
        return result;

    character = CSF_AT(void*, CSF_AT(void*, self, 8), 0x14);
    if ((uint8_t)IsField1BCNonNegative(character, nullptr))
        return result;

    character = CSF_AT(void*, CSF_AT(void*, self, 8), 0x14);
    void* sub = ((void*(__fastcall*)(void*, void*))CSF_AT(void**, character, 0)[10])(character, nullptr);
    if ((uint8_t)((int(__fastcall*)(void*, void*))CSF_AT(void**, sub, 0)[42])(sub, nullptr))
        return result;

    character = CSF_AT(void*, CSF_AT(void*, self, 8), 0x14);
    if ((uint8_t)SceneObject_PassesFirstCheck(character, nullptr))
        return result;

    character = CSF_AT(void*, CSF_AT(void*, self, 8), 0x14);
    if ((uint8_t)SceneObj_IsInactive(character, nullptr))
        return result;

    if (CSF_AT(uint8_t, self, 0x3A))
        return result;

    character = CSF_AT(void*, CSF_AT(void*, self, 8), 0x14);
    void* child = CSF_AT(void*, character, 0x44);
    if (!child)
        return result;

    uint32_t value = ((uint32_t(__fastcall*)(void*, void*))CSF_AT(void**, child, 0)[40])(child, nullptr);
    if (value == 0)
        return result;

    if (CSF_AT(uint8_t, self, 0x39))
        return 1;

    if (CSF_AT(int, self, 0x2F4)) {
        int state = CSF_AT(int, CSF_AT(void*, self, 0x14), 0x1D8);
        if (state == 2 || state == 3)
            return 1;
    }
    return result;
}

HOOK(0x004D7670, AutonomousTask_CanProceed, "auto");
