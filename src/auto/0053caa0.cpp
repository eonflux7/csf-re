#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_75FDF0, 0x0075FDF0, ());
CSF_GLOBAL(const char*, g_MenuOptionsPage, 0x00899ACC);
CSF_GLOBAL(const char*, g_InGameOptionsPage, 0x00899AD4);

static int __fastcall CMenuSoundOptions_ReturnToOptions(
    CMenuSoundOptions* self, void* /*edx*/)
{
    void* configuration = reinterpret_cast<void*>(GetGlobalValue_75FDF0());
    if (CSF_AT(std::uint8_t, configuration, 0xCC))
    {
        void* pendingObject = reinterpret_cast<void*>(self->field_0028);
        if (pendingObject)
        {
            ((void(__fastcall*)(void*, void*))
                CSF_AT(void**, pendingObject, 0)[0x08 / 4])(
                    pendingObject, nullptr);
        }
    }

    void* menu = reinterpret_cast<void*>(self->field_0014);
    ((void(__fastcall*)(void*, void*, CMenuSoundOptions*, int))
        CSF_AT(void**, menu, 0)[0x60 / 4])(menu, nullptr, self, 0);

    const std::uint32_t result = self->field_0018;
    if (result)
        return static_cast<std::int32_t>(result);

    const unsigned char inGame =
        ((unsigned char(__fastcall*)(void*, void*))
            CSF_AT(void**, self, 0)[0x30 / 4])(self, nullptr);

    const char* destination =
        *reinterpret_cast<const char* volatile*>(&g_InGameOptionsPage);
    if (!inGame)
        destination = g_MenuOptionsPage;

    menu = reinterpret_cast<void*>(self->field_0014);
    return ((int(__fastcall*)(void*, void*, const char*))
        CSF_AT(void**, menu, 0)[0x58 / 4])(menu, nullptr, destination);
}

HOOK(0x0053CAA0, CMenuSoundOptions_ReturnToOptions, "auto");
