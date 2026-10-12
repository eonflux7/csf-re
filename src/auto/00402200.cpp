#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char*, __fastcall, S_00492b20_GetPtr0x1B4, 0x00492B20, (void* self, void*));
CSF_FUNC(int, __cdecl, GameConfig_IsResolvedEntryActive, 0x00755880, (int entry));
CSF_FUNC(int, __cdecl, GameConfig_IsSceneObjectRemovable, 0x007558C0, (int objectType));

typedef void* (__fastcall* GetOwnerFn)(void* self, void* edx);
typedef unsigned char (__fastcall* BoolMethodFn)(void* self, void* edx);

static bool IsEntityExcludedOrIgnorable(S_00492b20* self, uint32_t* other)
{
    void* selfPtr = self;

    if (selfPtr == static_cast<void*>(other))
        return true;

    if (!((GetOwnerFn)CSF_AT(void**, selfPtr, 0)[0x28 / 4])(selfPtr, nullptr))
        return true;

    void* owner = ((GetOwnerFn)CSF_AT(void**, selfPtr, 0)[0x28 / 4])(selfPtr, nullptr);
    if (!((BoolMethodFn)CSF_AT(void**, owner, 0)[0x40 / 4])(owner, nullptr))
        return true;

    if ((CSF_AT(uint8_t, selfPtr, 0x170) & 8) == 0)
        return true;

    if (!other)
        return false;

    if (GameConfig_IsSceneObjectRemovable(CSF_AT(int, other, 0x50)) &&
        CSF_AT(int, other, 0x18C) == CSF_AT(int, selfPtr, 0x20))
        return true;

    if (!GameConfig_IsResolvedEntryActive(CSF_AT(int, other, 0x50)))
        return false;

    if ((CSF_AT(uint32_t, selfPtr, 0x170) & 0x2000) == 0)
        return false;

    char* ptr = S_00492b20_GetPtr0x1B4(selfPtr, nullptr);
    if (CSF_AT(int, ptr, 4) == CSF_AT(int, other, 0x20))
        return true;

    void* child = CSF_AT(void*, selfPtr, 0x4C);
    return ((BoolMethodFn)CSF_AT(void**, child, 0)[0x48 / 4])(child, nullptr) != 0;
}

HOOK(0x00402200, IsEntityExcludedOrIgnorable, "auto");
