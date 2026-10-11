#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, GetValue8BBB28, 0x006F7820, ());
CSF_FUNC(char, __fastcall, GfxHAnim_ReturnAnimationToPool, 0x006F7A70,
         (void* self, void* edx, void** a2));
CSF_FUNC(int, __fastcall, GfxHAnim_ClearBoneMatrix, 0x007075C0,
         (void* self, void* edx));

static int __fastcall GfxHAnim_ResetAnimationState(void* self, void* /*edx*/)
{
    std::uint32_t entryIndex = 0;
    std::uint32_t entryOffset = 0;

    if (CSF_AT(std::uint32_t, self, 0x0C) != 0)
    {
        do
        {
            auto* entry = CSF_AT(std::uint8_t*, self, 0x04) + entryOffset;
            auto** animation = reinterpret_cast<void**>(entry + 0x04);

            if (*animation != nullptr)
            {
                const int pool = GetValue8BBB28();
                GfxHAnim_ReturnAnimationToPool(
                    reinterpret_cast<void*>(pool), nullptr, animation);
                *animation = nullptr;
            }

            CSF_AT(std::uint32_t, entry, 0x08) = 0;
            CSF_AT(std::uint32_t, entry, 0x0C) = 0;
            ++entryIndex;
            entryOffset += 36;
        }
        while (entryIndex < CSF_AT(std::uint32_t, self, 0x0C));
    }

    const std::uint32_t flags = CSF_AT(std::uint32_t, self, 0x58);
    void* hierarchy = CSF_AT(void*, self, 0x14);
    CSF_AT(std::uint32_t, self, 0x58) = flags | 4u;

    for (int boneIndex = 0;
         boneIndex < CSF_AT(int, hierarchy, 0x04);
         ++boneIndex)
    {
        void* bone = CSF_AT(void**, self, 0x4C)[boneIndex];
        if (bone != nullptr)
            GfxHAnim_ClearBoneMatrix(bone, nullptr);

        hierarchy = CSF_AT(void*, self, 0x14);
    }

    return reinterpret_cast<int>(hierarchy);
}

HOOK(0x006E9ED0, GfxHAnim_ResetAnimationState, "auto");
