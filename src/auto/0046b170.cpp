#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __cdecl, GetGlobalObjectField184, 0x00475470, ());
CSF_FUNC(unsigned int, __fastcall, sub_475800, 0x00475800, (void* self, void* edx, int a2));
CSF_FUNC(void*, __fastcall, GrowButtonArray, 0x0041E5F0, (void* self, void* edx, int growth));
CSF_FUNC(int, __fastcall, sub_46A190, 0x0046A190, (void* self, void* edx, unsigned int a2, int a3, int a4, int a5, int a6));
CSF_FUNC(char*, __fastcall, sub_470E80, 0x00470E80, (void* self, void* edx, int a2));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));

// Vtable of the embedded array object (S_0041e5f0) at +0x28; kept for the address binding.
[[maybe_unused]] static const auto kActorArrayVtbl = CSF_VTBL(0x0041E5F0);

// Body of the tail-jump target (0x46AEE0 is reached by `jmp`, so its code is inlined here
// instead of being bound as a separate function). Returns eax as the original does.
static int __fastcall SceneLoader_CreateActorTracker(S_0044d360* self, void* /*edx*/)
{
    uint32_t trackerOwner = CSF_AT(uint32_t, self, 0x3C);
    if (trackerOwner == 0) {
        return 0;
    }

    void* tracker = Mem_Alloc(0x14);
    if (tracker) {
        CSF_AT(void*, tracker, 0x00) = CSF_VTBL(0x831434);
        CSF_AT(uint32_t, tracker, 0x08) = 0;
        CSF_AT(uint32_t, tracker, 0x0C) = 0;
        CSF_AT(uint32_t, tracker, 0x10) = 0;
        CSF_AT(void*, self, 0x38) = tracker;
        return reinterpret_cast<int>(sub_470E80(reinterpret_cast<void*>(trackerOwner), nullptr,
                                                reinterpret_cast<int>(tracker)));
    }

    CSF_AT(void*, self, 0x38) = nullptr;
    return reinterpret_cast<int>(sub_470E80(reinterpret_cast<void*>(trackerOwner), nullptr, 0));
}

static int __fastcall SceneLoader_PrepareSceneActors(S_0044d360* self, void* /*edx*/)
{
    self->field_00ac = 0;
    self->field_00d0 = 0;

    void* globalObject = reinterpret_cast<void*>(GetGlobalObjectField184());
    sub_475800(globalObject, nullptr, reinterpret_cast<int>(self));

    uint32_t arrayCapacity = self->field_002c;
    if (arrayCapacity < 0x12C) {
        GrowButtonArray(reinterpret_cast<void*>(&self->field_0028), nullptr,
                        static_cast<int>(0x12Cu - arrayCapacity));
    }

    sub_46A190(reinterpret_cast<void*>(reinterpret_cast<uint8_t*>(self) + 0x64), nullptr,
               0x2Cu, 0, 0, 0, 10);
    sub_46A190(reinterpret_cast<void*>(reinterpret_cast<uint8_t*>(self) + 0x74), nullptr,
               0x2Cu, 0, 0, 0, 10);

    // Original tail-jumps to 0x46AEE0 with ecx = this and returns its result.
    return SceneLoader_CreateActorTracker(self, nullptr);
}

HOOK(0x0046B170, SceneLoader_PrepareSceneActors, "auto");
