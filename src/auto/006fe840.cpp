#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, SceneLoader_GetSceneController, 0x00652010, (void* self, void* edx));
CSF_FUNC(double, __fastcall, C_00864508_GetField9c, 0x006D59F0, (C_00864508* self, void*));
CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void*, int additionalCapacity));
CSF_FUNC(const void*, __fastcall, Vec_GrowTwelveByteRecordArray, 0x004DE9B0, (void* self, void*, int extraRecords));

static uint8_t __fastcall ClumpLoader_RefreshSceneCapacityIfEnabled(void* self, void* /*edx*/) {
    // Code-address annotation: function value, not a vtable.
    [[maybe_unused]] const void* arrayVtbl = reinterpret_cast<void**>(CSF_ADDR_FN(0x0041E5F0));

    uint32_t eaxValue = CSF_AT(uint8_t, self, 0x20);
    if (CSF_AT(uint8_t, self, 0x20) != 0) {
        int globalInt = GetGlobalInt();
        int sceneController = SceneLoader_GetSceneController(reinterpret_cast<void*>(static_cast<uintptr_t>(globalInt)), nullptr);
        uint32_t sceneField = CSF_AT(uint32_t, reinterpret_cast<void*>(static_cast<uintptr_t>(sceneController)), 0xBC);
        double sceneScale = C_00864508_GetField9c(reinterpret_cast<C_00864508*>(static_cast<uintptr_t>(sceneField)), nullptr);
        CSF_AT(float, self, 0x1C) = static_cast<float>(sceneScale);

        uint8_t* base = reinterpret_cast<uint8_t*>(self);

        uint32_t firstCount = CSF_AT(uint32_t, self, 0x38);
        if (firstCount < 0x40) {
            S_0041e5f0_GrowArrayCapacity(reinterpret_cast<S_0041e5f0*>(base + 0x34), nullptr, static_cast<int>(64 - firstCount));
        }

        uint32_t secondCount = CSF_AT(uint32_t, self, 0x48);
        if (secondCount < 0x40) {
            S_0041e5f0_GrowArrayCapacity(reinterpret_cast<S_0041e5f0*>(base + 0x44), nullptr, static_cast<int>(64 - secondCount));
        }

        uint32_t thirdCount = CSF_AT(uint32_t, self, 0x58);
        eaxValue = thirdCount;
        if (thirdCount < 0x40) {
            const void* grown = Vec_GrowTwelveByteRecordArray(base + 0x54, nullptr, static_cast<int>(64 - thirdCount));
            eaxValue = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(grown));
        }

        CSF_AT(uint8_t, self, 0x21) = 1;
    }
    return static_cast<uint8_t>(eaxValue);
}

HOOK(0x006FE840, ClumpLoader_RefreshSceneCapacityIfEnabled, "auto");
