#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, ResetCollisionShapeRange, 0x006CE270, (int a1, int a2));

static int __fastcall C_00860c88_ClearCollisionShapes(
    C_00860c88* self, void* /*edx*/)
{
    auto* storage = reinterpret_cast<std::uint8_t*>(self) + 4;
    std::uint32_t index = 0;
    std::uint32_t byteOffset = 0;

    for (;;)
    {
        const std::uint32_t first = CSF_AT(std::uint32_t, storage, 4);
        if (first == 0)
            break;

        const std::int32_t byteCount = static_cast<std::int32_t>(
            CSF_AT(std::uint32_t, storage, 8) - first);
        if (index >= static_cast<std::uint32_t>(byteCount / 100))
            break;

        auto* entry = reinterpret_cast<std::uint8_t*>(
            CSF_AT(std::uint32_t, self, 0x08) + byteOffset);
        auto* object = CSF_AT(C_00860178*, entry, 96);
        if (object)
        {
            object->vSlot4(1);

            entry = reinterpret_cast<std::uint8_t*>(
                CSF_AT(std::uint32_t, self, 0x08) + byteOffset);
            CSF_AT(std::uint32_t, entry, 96) = 0;
        }

        ++index;
        byteOffset += 100;
    }

    const std::uint32_t first = CSF_AT(std::uint32_t, storage, 4);
    if (first != 0)
    {
        ResetCollisionShapeRange(
            static_cast<int>(first),
            CSF_AT(int, storage, 8));

        if (CSF_AT(std::uint32_t, storage, 4) != 0)
            Mem_FreeDebug(CSF_AT(void*, storage, 4));
    }

    CSF_AT(std::uint32_t, storage, 4) = 0;
    CSF_AT(std::uint32_t, storage, 8) = 0;
    CSF_AT(std::uint32_t, storage, 12) = 0;
    return 0;
}

HOOK(0x006CEA20, C_00860c88_ClearCollisionShapes, "auto");
