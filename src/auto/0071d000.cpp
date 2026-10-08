#include <cstdint>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "globals.h"

CSF_FUNC(std::int32_t*, __fastcall, CopyReferencedObjectList,
         0x0071C0A0, (std::int32_t* self, void* edx, void* source));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

static unsigned char __fastcall C_00864508_NotifyReferencedObjectsIfEnabled(
    C_00864508* self, void* /*edx*/, int argument, unsigned char enabled)
{
    if (!enabled)
        return enabled;

    std::int32_t snapshot[4];
    std::int32_t* copyResult = CopyReferencedObjectList(
        snapshot, nullptr, reinterpret_cast<std::uint8_t*>(self) + 0x44);
    unsigned char result = static_cast<unsigned char>(
        reinterpret_cast<std::uintptr_t>(copyResult));

    void** objects = reinterpret_cast<void**>(
        static_cast<std::uint32_t>(snapshot[1]));
    const std::int32_t count = objects
        ? static_cast<std::int32_t>(
              static_cast<std::uint32_t>(snapshot[2]) -
              reinterpret_cast<std::uintptr_t>(objects)) >> 2
        : 0;

    for (std::int32_t index = 0; index < count; ++index)
    {
        void* object = objects[index];
        if (object)
        {
            result = static_cast<unsigned char>(
                ((int (__fastcall*)(void*, void*, int))
                     CSF_AT(void**, object, 0)[3])(object, nullptr, argument));
        }
    }

    if (objects)
        return static_cast<unsigned char>(Mem_FreeDebug(objects));
    return result;
}

HOOK(0x0071D000, C_00864508_NotifyReferencedObjectsIfEnabled, "auto");
