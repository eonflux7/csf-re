#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_00833908_ForwardEnabledState, 0x0048E240,
         (void* self, void* edx, int enabled));

static int __fastcall C_00833b88_SetLinkedObjectEnabled(
    C_00833b88* self, void* /*edx*/, int enabled)
{
    int result = C_00833908_ForwardEnabledState(self, nullptr, enabled);
    void* linkedObject = reinterpret_cast<void*>(self->field_0044);
    if (!linkedObject)
        return result;

    result = reinterpret_cast<int(__fastcall*)(void*, void*)>(
        CSF_AT(void**, linkedObject, 0)[0xFC / 4])(linkedObject, nullptr);
    if (result == -1)
        return result;

    void* owner = reinterpret_cast<void*(__fastcall*)(void*, void*)>(
        CSF_AT(void**, linkedObject, 0)[0xD8 / 4])(linkedObject, nullptr);
    void* target = reinterpret_cast<void*(__fastcall*)(void*, void*)>(
        CSF_AT(void**, owner, 0)[0xA8 / 4])(owner, nullptr);
    if (!target)
        return 0;

    if (static_cast<std::uint8_t>(enabled))
    {
        return reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, target, 0)[0x3C / 4])(target, nullptr, 1);
    }

    owner = reinterpret_cast<void*(__fastcall*)(void*, void*)>(
        CSF_AT(void**, linkedObject, 0)[0xD8 / 4])(linkedObject, nullptr);
    target = reinterpret_cast<void*(__fastcall*)(void*, void*)>(
        CSF_AT(void**, owner, 0)[0xA8 / 4])(owner, nullptr);
    return reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, target, 0)[0x3C / 4])(target, nullptr, 0);
}

HOOK(0x00492A10, C_00833b88_SetLinkedObjectEnabled, "auto");
