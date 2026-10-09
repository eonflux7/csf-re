#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

namespace {

// Stack argument block passed by address to the virtual slot: {3, 0}.
struct SceneCommandTag {
    int kind;
    uint8_t flag;
};

}  // namespace

static int __stdcall ForwardSceneHandleCommand(int a1, int a2, int a3) {
    // a3 -> node; node->[4] -> owner; owner->[12] -> ownerValue; handle = owner + 4.
    void* argPtr = reinterpret_cast<void*>(static_cast<uintptr_t>(a3));
    uint8_t* node = CSF_AT(uint8_t*, argPtr, 0);
    uint8_t* owner = CSF_AT(uint8_t*, node, 4);
    int32_t ownerValue = CSF_AT(int32_t, owner, 12);
    int result = static_cast<int>(reinterpret_cast<uintptr_t>(owner + 4));

    if (ownerValue >= 0) {
        result = CHandleTable_Resolve(g_pCurrentScene, nullptr, result);
        if (result != 0) {
            void* target = reinterpret_cast<void*>(static_cast<uintptr_t>(
                CSF_AT(uint32_t, reinterpret_cast<void*>(static_cast<uintptr_t>(result)), 52)));
            if (target != nullptr) {
                SceneCommandTag tag;
                tag.kind = 3;
                tag.flag = 0;
                using SlotFn = int(__fastcall*)(void* self, void* edx, SceneCommandTag* tag);
                SlotFn slot = reinterpret_cast<SlotFn>(CSF_AT(void**, target, 0)[42]);
                return slot(target, nullptr, &tag);
            }
        }
    }
    return result;
}

HOOK(0x45ECA0, ForwardSceneHandleCommand, "auto");
