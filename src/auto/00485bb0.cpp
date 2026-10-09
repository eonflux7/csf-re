#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, GetMemberValueD4, 0x00476090, (void* self, void*));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));

static char __fastcall CharActionFsm_CheckActionObjects(void* self, void* /*edx*/)
{
    const uint32_t begin = CSF_AT(uint32_t, self, 0x18);
    const uint32_t count = begin
        ? static_cast<uint32_t>(
              static_cast<int32_t>(CSF_AT(uint32_t, self, 0x1C) - begin) >> 2)
        : 0;

    char blocked = 0;
    for (uint32_t index = 0; index < count; ++index)
    {
        if (blocked)
            break;

        auto* object = reinterpret_cast<C_0085e7b8**>(
            CSF_AT(uint32_t, self, 0x18))[index];
        if (!object)
            continue;

        if (((bool(__fastcall*)(void*, void*))object->vtbl[0x9C / 4])(
                object, nullptr)
            && (GetMemberValueD4(object, nullptr) == 2
                || GetMemberValueD4(object, nullptr) == 1))
        {
            char sameProvider = 0;
            auto* current = CSF_AT(C_0085e7b8*, self, 0x9C);
            if (current
                && ((int(__fastcall*)(void*, void*))current->vtbl[0x4C / 4])(
                       current, nullptr) == 10)
            {
                void* provider = GetCachedObjectProvider(object, nullptr);
                sameProvider = provider == GetCachedObjectProvider(
                    CSF_AT(C_0085e7b8*, self, 0x9C), nullptr);
            }

            if (((bool(__fastcall*)(void*, void*))object->vtbl[0x58 / 4])(
                    object, nullptr)
                || !((bool(__fastcall*)(void*, void*, char))object->vtbl[0x54 / 4])(
                       object, nullptr, sameProvider))
            {
                blocked = 1;
            }
        }
    }
    return blocked;
}

HOOK(0x00485BB0, CharActionFsm_CheckActionObjects, "auto");
