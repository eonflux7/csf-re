#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* , int additionalCapacity));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __fastcall, C_00840f48_Construct, 0x0051E810, (C_00840f48* self, void*));
CSF_FUNC(int, __fastcall, C_00840f6c_Construct, 0x0051F0D0, (C_00840f6c* self, void*));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));

static int __fastcall FxParticleManager_RefreshRenderState(void* self, void* /*edx*/)
{
    // The packet also classifies the growth routine's address as a vtable reference.
    (void)CSF_VTBL(0x0041E5F0);

    if (CSF_AT(std::uint8_t, reinterpret_cast<void*>(GetValue8B0554()), 0x970))
    {
        auto* storage = static_cast<C_00840f6c*>(Mem_Alloc(76));
        C_00840f6c* state = nullptr;
        if (storage)
            state = reinterpret_cast<C_00840f6c*>(
                C_00840f6c_Construct(storage, nullptr));

        ((void(__fastcall*)(void*, void*))state->vtbl[3])(state, nullptr);

        if (CSF_AT(std::uint32_t, self, 0x24) >=
            CSF_AT(std::uint32_t, self, 0x20))
        {
            S_0041e5f0_GrowArrayCapacity(
                reinterpret_cast<S_0041e5f0*>(
                    static_cast<std::uint8_t*>(self) + 0x1C),
                nullptr, CSF_AT(int, self, 0x28));
        }

        const std::uint32_t count = CSF_AT(std::uint32_t, self, 0x24);
        auto* entry = reinterpret_cast<std::uint32_t*>(
            CSF_AT(std::uint32_t, self, 0x1C)) + count;
        CSF_AT(std::uint32_t, self, 0x24) = count + 1;
        *entry = reinterpret_cast<std::uint32_t>(state);
    }

    auto* storage = static_cast<C_00840f48*>(Mem_Alloc(44));
    C_00840f48* state = nullptr;
    if (storage)
        state = reinterpret_cast<C_00840f48*>(
            C_00840f48_Construct(storage, nullptr));

    ((void(__fastcall*)(void*, void*))state->vtbl[3])(state, nullptr);

    if (CSF_AT(std::uint32_t, self, 0x24) >=
        CSF_AT(std::uint32_t, self, 0x20))
    {
        S_0041e5f0_GrowArrayCapacity(
            reinterpret_cast<S_0041e5f0*>(
                static_cast<std::uint8_t*>(self) + 0x1C),
            nullptr, CSF_AT(int, self, 0x28));
    }

    const std::uint32_t count = CSF_AT(std::uint32_t, self, 0x24);
    auto* entry = reinterpret_cast<std::uint32_t*>(
        CSF_AT(std::uint32_t, self, 0x1C)) + count;
    const std::uint32_t newCount = count + 1;
    CSF_AT(std::uint32_t, self, 0x24) = newCount;
    *entry = reinterpret_cast<std::uint32_t>(state);
    return static_cast<int>(newCount);
}

HOOK(0x00520020, FxParticleManager_RefreshRenderState, "auto");
