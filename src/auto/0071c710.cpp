#include <cstdint>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "globals.h"

CSF_FUNC(char, __fastcall, C_00864508_DoAllReferencedObjectsPassCheck,
         0x0071A940, (C_00864508* self, void* edx));
CSF_FUNC(std::int32_t*, __fastcall, CopyReferencedObjectList,
         0x0071C0A0, (std::int32_t* self, void* edx, void* source));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

static char __fastcall C_00864508_DispatchReferencedSingleArgumentOperation(
    C_00864508* self, void* /*edx*/, int argument)
{
    char result = C_00864508_DoAllReferencedObjectsPassCheck(self, nullptr);
    if (!result)
        return result;

    std::int32_t snapshot[4];
    CopyReferencedObjectList(
        snapshot, nullptr, reinterpret_cast<std::uint8_t*>(self) + 0x44);

    void** objects = reinterpret_cast<void**>(
        static_cast<std::uint32_t>(snapshot[1]));
    const std::int32_t count = objects
        ? static_cast<std::int32_t>(
              static_cast<std::uint32_t>(snapshot[2]) -
              reinterpret_cast<std::uintptr_t>(objects)) >> 2
        : 0;

    for (std::int32_t index = count - 1; index >= 0; --index)
    {
        const std::uint32_t exclusionsBegin = self->field_0034;
        const std::uint32_t exclusionCount = exclusionsBegin
            ? static_cast<std::uint32_t>(
                  static_cast<std::int32_t>(
                      self->field_0038 - exclusionsBegin) >> 3)
            : 0;

        bool excluded = false;
        if (exclusionCount)
        {
            auto* exclusion = reinterpret_cast<std::uint8_t*>(self->field_0034);
            for (std::uint32_t position = 0; position < exclusionCount;
                 ++position, exclusion += 8)
            {
                if (CSF_AT(std::int32_t, exclusion, 0) == index)
                {
                    excluded = true;
                    break;
                }
            }
        }

        if (!excluded)
        {
            void* object = objects[index];
            void* primary = reinterpret_cast<void*>(
                ((int (__fastcall*)(void*, void*))
                     CSF_AT(void**, object, 0)[8])(object, nullptr));

            ((void (__fastcall*)(void*, void*, int))
                 CSF_AT(void**, primary, 0)[6])(primary, nullptr, argument);

            void* secondary =
                reinterpret_cast<std::uint8_t*>(objects[index]) + 4;
            ((void (__fastcall*)(void*, void*, int))
                 CSF_AT(void**, secondary, 0)[6])(secondary, nullptr, argument);
        }

        void* object = objects[index];
        if (((unsigned char (__fastcall*)(void*, void*))
                 CSF_AT(void**, object, 0)[10])(object, nullptr))
            break;
    }

    result = static_cast<char>(
        ((int (__fastcall*)(void*, void*))
             CSF_AT(void**, self, 0)[42])(self, nullptr));

    if (objects)
        return static_cast<char>(Mem_FreeDebug(objects));
    return result;
}

HOOK(0x0071C710, C_00864508_DispatchReferencedSingleArgumentOperation, "auto");
