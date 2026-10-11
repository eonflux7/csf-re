#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, HAnim_GetSceneChannelCount, 0x006C2080, (void* self, void* edx));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(float*, __fastcall, HAnim_ApplyChannelPose, 0x006C3320, (void* self, void* edx, int a2, float* a3, float a4));

// Referenced vtable of S_006c3320 (used by the callee's call site).
[[maybe_unused]] static const void* const kChannelPoseVtbl = CSF_VTBL(0x006C3320);

static void __stdcall HAnim_FillQuatSlotsAndApply(int a1, S_006c3320* a2, float a3)
{
    if (!a2)
        return;

    uint32_t* slots = reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(a1));
    int channelCount = HAnim_GetSceneChannelCount(a2, nullptr);

    for (int i = 0; i < channelCount; ++i) {
        if (slots[i] == 0) {
            void* block = Mem_Alloc(28);
            if (block) {
                CSF_AT(uint32_t, block, 0) = 0;
                CSF_AT(uint32_t, block, 4) = 0;
                slots[i] = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(block));
            } else {
                slots[i] = 0;
            }
        }
        HAnim_ApplyChannelPose(a2, nullptr,
                               static_cast<int>(slots[i]),
                               reinterpret_cast<float*>(static_cast<uintptr_t>(i)),
                               a3);
    }
}

HOOK(0x006E8540, HAnim_FillQuatSlotsAndApply, "auto");
