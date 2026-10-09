#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(uint32_t*, __fastcall, AudioSoundManager_ApplySoundDefinition, 0x00521FA0, (void* self, void* edx, int a2));
CSF_FUNC(uint32_t*, __fastcall, C_00840ff0_Construct, 0x005237C0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, AudioSoundManager_UpdateSpecialSound, 0x00527970, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(uint32_t*, __fastcall, CSoundInstance_Construct, 0x00527DC0, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, C_00841128_Construct, 0x00527FE0, (void* self, void* edx));

static uint32_t* __fastcall AudioSoundManager_CreateSoundInstance(
    void* self, void* /*edx*/, uint32_t* definition)
{
    uint32_t* instance = nullptr;

    switch (*definition)
    {
    case 0:
    case 1:
    case 2:
    case 4:
    {
        const uint32_t poolCount = CSF_AT(uint32_t, self, 0x0C);
        const uint32_t startIndex = CSF_AT(uint32_t, self, 0x10);
        uint32_t index = startIndex;

        if (poolCount != 0)
        {
            auto* pool = CSF_AT(uint8_t*, self, 0x00);
            for (;;)
            {
                if (CSF_AT(uint8_t, pool + 112 * index, 0x3C) == 0)
                {
                    instance = reinterpret_cast<uint32_t*>(pool + 112 * index);
                    CSF_AT(uint32_t, self, 0x10) = index;
                    break;
                }

                ++index;
                if (index == poolCount)
                {
                    if (startIndex == 0)
                        break;
                    index = 0;
                }

                if (index == startIndex)
                    break;
                if (CSF_AT(uint32_t, self, 0x0C) == 0)
                    break;
            }
        }

        if (!instance)
        {
            void* storage = Mem_Alloc(112);
            if (!storage)
                return nullptr;

            instance = CSoundInstance_Construct(storage, nullptr);
            if (!instance)
                return nullptr;
        }

        AudioSoundManager_ApplySoundDefinition(
            instance + 1, nullptr, reinterpret_cast<int>(definition));
        return instance;
    }

    case 3:
    case 5:
    {
        void* storage = Mem_Alloc(96);
        if (!storage)
        {
            AudioSoundManager_ApplySoundDefinition(
                reinterpret_cast<void*>(4), nullptr,
                reinterpret_cast<int>(definition));
            return nullptr;
        }

        instance = C_00840ff0_Construct(storage, nullptr);
        AudioSoundManager_ApplySoundDefinition(
            reinterpret_cast<void*>(reinterpret_cast<std::uintptr_t>(instance) + 4),
            nullptr, reinterpret_cast<int>(definition));
        return instance;
    }

    case 6:
        if (!CSF_AT(uint32_t*, self, 0x08))
        {
            void* storage = Mem_Alloc(128);
            if (storage)
                instance = C_00841128_Construct(storage, nullptr);
            CSF_AT(uint32_t*, self, 0x08) = instance;
        }

        AudioSoundManager_UpdateSpecialSound(
            CSF_AT(uint32_t*, self, 0x08), nullptr, definition + 6);

        if (instance)
        {
            AudioSoundManager_ApplySoundDefinition(
                instance + 1, nullptr, reinterpret_cast<int>(definition));
        }
        return instance;

    default:
        return nullptr;
    }
}

HOOK(0x00528430, AudioSoundManager_CreateSoundInstance, "auto");
