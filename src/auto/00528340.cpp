#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, InitializeSoundInstance, 0x00521FA0,
         (void* self, void* edx, int a2));
CSF_FUNC(uint32_t*, __fastcall, ConstructSpecialSoundInstance, 0x005237C0,
         (void* self, void* edx));
CSF_FUNC(int, __fastcall, AllocateManagedSoundInstance, 0x00526D40,
         (void* self, void* edx, float a2));
CSF_FUNC(int, __fastcall, ConfigureSharedSoundInstance, 0x00527970,
         (void* self, void* edx, uint32_t* a2));
CSF_FUNC(uint32_t*, __fastcall, ConstructSharedSoundInstance, 0x00527FE0,
         (void* self, void* edx));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));

// The packet also classifies the allocator method address as a vtable.
[[maybe_unused]] static auto const g_PacketSoundManagerVtable =
    CSF_VTBL(0x00526D40);

static uint32_t* __fastcall S_00526d40_CreateSoundInstance(
    S_00526d40* self, void* /*edx*/, uint32_t* definition)
{
    uint32_t* instance = nullptr;

    switch (*definition)
    {
    case 0:
    case 1:
    case 2:
    case 4:
    {
        // The bound signature is float, but the original pushes pointer bits.
        float encodedDefinition;
        std::memcpy(&encodedDefinition, &definition, sizeof(encodedDefinition));

        instance = reinterpret_cast<uint32_t*>(
            AllocateManagedSoundInstance(self, nullptr, encodedDefinition));
        if (instance)
        {
            InitializeSoundInstance(
                instance + 1, nullptr,
                reinterpret_cast<std::intptr_t>(definition));
        }
        return instance;
    }

    case 3:
    {
        void* storage = Mem_Alloc(96);
        if (storage)
            instance = ConstructSpecialSoundInstance(storage, nullptr);

        // Allocation failure still calls the initializer with this == 4.
        InitializeSoundInstance(
            reinterpret_cast<void*>(
                reinterpret_cast<std::uintptr_t>(instance) + 4),
            nullptr, reinterpret_cast<std::intptr_t>(definition));
        return instance;
    }

    case 6:
        if (!CSF_AT(uint32_t*, self, 0x08))
        {
            void* storage = Mem_Alloc(128);
            if (storage)
                instance = ConstructSharedSoundInstance(storage, nullptr);

            CSF_AT(uint32_t*, self, 0x08) = instance;
        }

        ConfigureSharedSoundInstance(
            CSF_AT(uint32_t*, self, 0x08), nullptr, definition + 6);

        if (instance)
        {
            InitializeSoundInstance(
                instance + 1, nullptr,
                reinterpret_cast<std::intptr_t>(definition));
        }
        return instance;

    default:
        return nullptr;
    }
}

HOOK(0x00528340, S_00526d40_CreateSoundInstance, "auto");
