#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, SceneObj_IsInactive, 0x0048E100, (void* self, void* edx));
CSF_FUNC(int, __cdecl, IsResolvedEntryActive, 0x00755880, (int a1));

static char __stdcall SceneObj_MatchesFilter(C_00833b88* object, int* filter)
{
    const std::uint32_t excludedCount = static_cast<std::uint32_t>(filter[6]);
    for (std::uint32_t index = 0; index < excludedCount; ++index)
    {
        auto excludedObjects = reinterpret_cast<C_00833b88**>(filter[4]);
        if (excludedObjects[index] == object)
            return 0;
    }

    int flags = filter[0];
    if ((flags & 4) != 0 && (object->field_0170 & 8) == 0)
        return 0;
    if ((flags & 8) != 0 && (object->field_0170 & 0x200) == 0)
        return 0;
    if ((flags & 0x400) != 0 &&
        !reinterpret_cast<bool (__fastcall*)(void*, void*)>(
            object->vtbl[0x48 / 4])(object, nullptr))
        return 0;

    if ((filter[0] & 0x10) == 0 &&
        static_cast<std::uint8_t>(SceneObj_IsInactive(object, nullptr)) == 0)
        return 0;
    if ((filter[0] & 0x20) == 0 &&
        static_cast<std::uint8_t>(SceneObj_IsInactive(object, nullptr)) != 0)
        return 0;

    if (filter[1] != -1)
    {
        auto relatedObject = reinterpret_cast<C_008672e4*>(object->field_0034);
        if (!relatedObject)
            return 0;

        char* identifier =
            reinterpret_cast<char* (__fastcall*)(void*, void*)>(
                relatedObject->vtbl[0x98 / 4])(relatedObject, nullptr);
        if (identifier != reinterpret_cast<char*>(filter[1]))
            return 0;
    }

    const int excludedType = filter[3];
    if (excludedType != -1 && excludedType == object->field_0050)
        return 0;

    const int requiredType = filter[2];
    if (requiredType == -1)
    {
        flags = filter[0];
        if ((flags & 0xC0) == 0 && (flags & 0x1B00) == 0)
            return 1;
    }
    else if (requiredType == object->field_0050)
    {
        return 1;
    }

    flags = filter[0];
    if ((flags & 0x40) != 0 && object->field_0050 == 0)
        return 1;
    if ((flags & 0x80) != 0 && object->field_0050 == 1)
        return 1;
    if ((flags & 0x100) != 0 && (object->field_0170 & 0x2000) != 0)
        return 1;
    if ((flags & 0x200) != 0 &&
        static_cast<std::uint8_t>(IsResolvedEntryActive(object->field_0050)) != 0)
        return 1;

    flags = filter[0];
    if ((flags & 0x800) != 0 && object->field_0050 == 12)
        return 1;
    if ((flags & 0x1000) != 0 && object->field_0050 == 17)
        return 1;

    return 0;
}

HOOK(0x0044FFF0, SceneObj_MatchesFilter, "auto");
