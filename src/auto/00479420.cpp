#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);
static CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));

static int __fastcall C_008317c8_ClearLinkedWeaponState(C_008317c8* self, void* /*edx*/)
{
    if ((self->field_006c & 0x80000000) == 0)
    {
        void* linkedObject = reinterpret_cast<void*>(CHandleTable_Resolve(
            g_pCurrentScene, nullptr,
            reinterpret_cast<int>(reinterpret_cast<std::uint8_t*>(self) + 0x64)));

        if (linkedObject)
        {
            void* state = reinterpret_cast<void* (__fastcall*)(void*, void*)>(
                CSF_AT(void**, linkedObject, 0)[0x28 / 4])(linkedObject, nullptr);

            if (reinterpret_cast<bool (__fastcall*)(void*, void*)>(
                    CSF_AT(void**, state, 0)[0xA8 / 4])(state, nullptr))
            {
                state = reinterpret_cast<void* (__fastcall*)(void*, void*)>(
                    CSF_AT(void**, linkedObject, 0)[0x28 / 4])(linkedObject, nullptr);
                reinterpret_cast<void (__fastcall*)(void*, void*)>(
                    CSF_AT(void**, state, 0)[0xA4 / 4])(state, nullptr);
            }
        }
    }

    self->field_0068 = -1;
    self->field_006c = -1;
    return -1;
}

HOOK(0x00479420, C_008317c8_ClearLinkedWeaponState, "auto");
