#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(void, __fastcall, WeaponFirearms_Update, 0x00475B90, (void* self, void* edx, float a2));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static int __fastcall C_008317c8_UpdateAndCheckHandle(
    C_008317c8* self, void* /*edx*/, float deltaTime)
{
    WeaponFirearms_Update(self, nullptr, deltaTime);

    int result = static_cast<int>(self->field_006c);
    if (result >= 0)
    {
        result = CHandleTable_Resolve(
            g_pCurrentScene, nullptr,
            reinterpret_cast<int>(reinterpret_cast<std::uint8_t*>(self) + 0x64));

        if (!result ||
            !CSF_AT(std::uint8_t, reinterpret_cast<void*>(result), 0x198))
        {
            return self->vSlotE4();
        }
    }
    return result;
}

HOOK(0x004789D0, C_008317c8_UpdateAndCheckHandle, "auto");
