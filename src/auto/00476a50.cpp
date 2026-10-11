#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
static CSF_FUNC(float, __fastcall, MenuSystem_GetFadeScale, 0x0050D9A0, (void* self, void*));

static std::uint64_t __fastcall C_008316d8_GetItemCapacity(
    C_008316d8* self, void* /*edx*/)
{
    const int weaponType = CSF_AT(int, reinterpret_cast<void*>(self->field_0040), 0xD4);
    float fadeScale = 1.0f;

    if ((weaponType == 2 || weaponType == 1) && self->vSlot9C())
    {
        const int globalState = GetGlobalState_50D190();
        fadeScale = MenuSystem_GetFadeScale(reinterpret_cast<void*>(globalState), nullptr);
    }

    const int capacity = CSF_AT(int, reinterpret_cast<void*>(self->field_0040), 0x114);
    return static_cast<std::uint64_t>(
        static_cast<std::int64_t>(static_cast<double>(capacity) * fadeScale));
}

HOOK(0x00476A50, C_008316d8_GetItemCapacity, "auto");
