#include <cstdint>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "globals.h"

// Original function addresses (bound by address, file-local bindings).
CSF_FUNC(char, __fastcall, C_00864508_DoAllReferencedObjectsPassCheck,
         0x0071A940, (C_00864508* self, void* edx));
CSF_FUNC(std::int32_t*, __fastcall, ConstructPointerVectorCopy_71C0A0,
         0x0071C0A0, (std::int32_t* self, void* edx, void* source));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

// Takes a snapshot of this object's referenced-object list, then (unless the
// whole referenced set passes the precondition check) walks the snapshot from
// the back, skipping indices listed in the exclusion vector, and for every
// remaining entry forwards `argument` to the entry's primary object (vtable
// slot 8) and to its embedded secondary object at +4 (slot 8 as well). The walk
// stops early when an entry's slot 10 query returns a non-zero byte. Finally
// the object's own vtable slot 42 hook is invoked and the snapshot is freed.
static char __fastcall C_00864508_DispatchOperationToReferencedObjects(
    C_00864508* self, void* /*edx*/, int argument)
{
    char result = C_00864508_DoAllReferencedObjectsPassCheck(self, nullptr);
    if (!result)
        return result;

    std::int32_t snapshot[4];
    ConstructPointerVectorCopy_71C0A0(
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
            auto* exclusion =
                reinterpret_cast<std::uint8_t*>(self->field_0034);
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
                 CSF_AT(void**, primary, 0)[8])(primary, nullptr, argument);

            void* secondary =
                reinterpret_cast<std::uint8_t*>(objects[index]) + 4;
            ((void (__fastcall*)(void*, void*, int))
                 CSF_AT(void**, secondary, 0)[8])(secondary, nullptr, argument);
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

HOOK(0x0071C8B0, C_00864508_DispatchOperationToReferencedObjects, "auto");
