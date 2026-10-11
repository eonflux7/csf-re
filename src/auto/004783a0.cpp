#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static int __fastcall C_008317c8_ResetSubObjectTarget(
    C_008317c8* self, void* /*edx*/)
{
    int result = static_cast<int>(self->field_006c);
    if (result < 0)
        return result;

    result = CHandleTable_Resolve(
        g_pCurrentScene, nullptr,
        reinterpret_cast<int>(reinterpret_cast<std::uint8_t*>(self) + 0x64));
    if (!result)
        return result;

    void* resolved = reinterpret_cast<void*>(result);
    result = ((int(__fastcall*)(void*, void*))
        CSF_AT(void**, resolved, 0)[0x28 / 4])(resolved, nullptr);
    if (!result)
        return result;

    void* target = reinterpret_cast<void*>(result);
    if (((bool(__fastcall*)(void*, void*))
        CSF_AT(void**, target, 0)[0xA8 / 4])(target, nullptr))
    {
        ((void(__fastcall*)(void*, void*))
            CSF_AT(void**, target, 0)[0xA4 / 4])(target, nullptr);
    }

    ((void(__fastcall*)(void*, void*, int))
        CSF_AT(void**, target, 0)[0x3C / 4])(target, nullptr, 1);

    if (self->field_0070)
    {
        void* owner = reinterpret_cast<void*>(self->field_0070);
        result = ((int(__fastcall*)(void*, void*))
            CSF_AT(void**, owner, 0)[0xA8 / 4])(owner, nullptr);
    }
    else
    {
        result = self->vResolveEntryLookupValue();
    }

    if (result)
    {
        void* receiver = reinterpret_cast<void*>(result);
        return ((int(__fastcall*)(void*, void*, void*, int, int))
            CSF_AT(void**, receiver, 0)[0xA0 / 4])(
                receiver, nullptr, target, 100, 0);
    }
    return result;
}

HOOK(0x004783A0, C_008317c8_ResetSubObjectTarget, "auto");
