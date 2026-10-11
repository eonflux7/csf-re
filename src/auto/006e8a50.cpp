#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RpHAnimHierarchyGetMatrixArray, 0x0056CF80, (int a1));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __fastcall, C_008633a0_ActivateBone, 0x007075C0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_008633a0_InitializeBone, 0x007075F0,
         (void* self, void* edx, int a2, int a3, int a4, int a5, int a6, int a7));

static int __fastcall C_00861e98_GetOrCreateBone(
    C_00861e98* self, void* /*edx*/, int index)
{
    if (CSF_AT(int*, self, 0x4C)[index] != 0)
        return CSF_AT(int*, self, 0x4C)[index];

    auto* hierarchy = CSF_AT(std::uint8_t*, self, 0x14);
    auto* node = CSF_AT(std::uint8_t*, hierarchy, 0x10)
               + static_cast<std::uint32_t>(index) * 16;
    auto* animation = CSF_AT(std::uint8_t*, hierarchy, 0x20);
    auto* frame = animation
                + static_cast<std::uint32_t>(index)
                    * CSF_AT(std::uint32_t, animation, 0x24)
                + 76;

    const std::uint32_t matrixAddress =
        static_cast<std::uint32_t>(
            RpHAnimHierarchyGetMatrixArray(reinterpret_cast<int>(hierarchy)))
        + (static_cast<std::uint32_t>(index) << 6);

    void* bone = Mem_Alloc(40);
    if (bone)
    {
        C_008633a0_InitializeBone(
            bone, nullptr, CSF_AT(int, node, 0), 0,
            reinterpret_cast<int>(frame + 8),
            reinterpret_cast<int>(frame + 24),
            static_cast<int>(matrixAddress),
            reinterpret_cast<int>(self));
        CSF_AT(int, bone, 0x24) = 0;
    }

    CSF_AT(int*, self, 0x4C)[index] = reinterpret_cast<int>(bone);

    if ((CSF_AT(std::uint8_t, self, 0x58) & 4) != 0)
    {
        C_008633a0_ActivateBone(
            reinterpret_cast<void*>(CSF_AT(int*, self, 0x4C)[index]),
            nullptr);
    }

    return CSF_AT(int*, self, 0x4C)[index];
}

HOOK(0x006E8A50, C_00861e98_GetOrCreateBone, "auto");
