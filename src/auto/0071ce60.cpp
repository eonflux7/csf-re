#include <cstdint>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "globals.h"

CSF_FUNC(char, __fastcall, C_00864508_DoAllReferencedObjectsPassCheck,
         0x0071A940, (C_00864508* self, void* edx));
CSF_FUNC(std::int32_t*, __fastcall, ConstructPointerVectorCopy_71C0A0,
         0x0071C0A0, (std::int32_t* self, void* edx, void* source));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

// Iterates the snapshot of referenced objects in reverse order; for each object
// not present in the exclusion list, invokes vtable slot 15 (a two-argument
// operation) on the object's primary interface and on the sub-object embedded at
// +4, then polls vtable slot 10 to decide whether to stop. Finally calls vtable
// slot 42 on the owning object and frees the snapshot buffer.
static char __fastcall C_00864508_DispatchReferencedTwoArgumentOperation(
    C_00864508* self, void* /*edx*/, int firstArgument, int secondArgument)
{
    char result = C_00864508_DoAllReferencedObjectsPassCheck(self, nullptr);
    if (!result)
        return result;

    std::int32_t snapshot[4];
    ConstructPointerVectorCopy_71C0A0(
        snapshot, nullptr, reinterpret_cast<std::uint8_t*>(self) + 0x44);

    void** objects =
        reinterpret_cast<void**>(static_cast<std::uint32_t>(snapshot[1]));
    const std::int32_t objectCount = objects
        ? static_cast<std::int32_t>(
              static_cast<std::uint32_t>(snapshot[2]) -
              reinterpret_cast<std::uintptr_t>(objects)) >> 2
        : 0;

    for (std::int32_t index = objectCount - 1; index >= 0; --index)
    {
        // Re-read the exclusion list every outer iteration: the operation
        // dispatched below may mutate it.
        const std::uint32_t exclusionBegin = self->field_0034;
        const std::uint32_t exclusionCount = exclusionBegin
            ? static_cast<std::uint32_t>(
                  static_cast<std::int32_t>(
                      self->field_0038 - exclusionBegin) >> 3)
            : 0;

        bool excluded = false;
        if (exclusionCount != 0)
        {
            const std::uint8_t* exclusion =
                reinterpret_cast<const std::uint8_t*>(self->field_0034);
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

            ((void (__fastcall*)(void*, void*, int, int))
                 CSF_AT(void**, primary, 0)[15])(
                     primary, nullptr, firstArgument, secondArgument);

            void* secondary =
                reinterpret_cast<std::uint8_t*>(objects[index]) + 4;
            ((void (__fastcall*)(void*, void*, int, int))
                 CSF_AT(void**, secondary, 0)[15])(
                     secondary, nullptr, firstArgument, secondArgument);
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

HOOK(0x0071CE60, C_00864508_DispatchReferencedTwoArgumentOperation, "auto");
