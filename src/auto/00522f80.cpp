#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(unsigned int*, __fastcall, ReleaseSoundEntry, 0x00526CF0,
         (void* self, void* edx, unsigned int* a2));
CSF_FUNC(void*, __cdecl, _memcpy, 0x007AEE60,
         (void* destination, const void* source, std::size_t byteCount));

static void* __fastcall S_00522010_UpdateSoundEntriesAndRemoveFinished(
    S_00522010* self, void* /*edx*/, int updateValue)
{
    const uint32_t initialCount = self->field_0028;
    void* result = reinterpret_cast<void*>(initialCount);

    if (initialCount != 0)
    {
        uint32_t entryOffset = 0;
        uint32_t remaining = initialCount;
        do
        {
            void* sound = CSF_AT(
                void*, reinterpret_cast<uint8_t*>(self->field_0020) + entryOffset, 0x4);

            if (sound && CSF_AT(uint8_t, sound, 0x3C))
            {
                if (((uint32_t(__fastcall*)(void*, void*))
                         CSF_AT(void**, sound, 0)[0x5C / 4])(sound, nullptr) & 0xFF)
                {
                    ((void(__fastcall*)(void*, void*, int))
                         CSF_AT(void**, sound, 0)[0x50 / 4])(
                        sound, nullptr, updateValue);
                }
                else
                {
                    void* globalState =
                        reinterpret_cast<void*>(GetGlobalState_50D190());
                    int globalUpdateValue = CSF_AT(int, globalState, 0xA0);
                    ((void(__fastcall*)(void*, void*, int))
                         CSF_AT(void**, sound, 0)[0x50 / 4])(
                        sound, nullptr, globalUpdateValue);
                }
            }

            entryOffset += 24;
            --remaining;
        } while (remaining != 0);
    }

    int32_t index = static_cast<int32_t>(initialCount - 1);
    if (index >= 0)
    {
        uint32_t entryOffset = 24 * static_cast<uint32_t>(index);
        do
        {
            void* sound = CSF_AT(
                void*, reinterpret_cast<uint8_t*>(self->field_0020) + entryOffset, 0x4);

            bool removeEntry = sound == nullptr;
            if (sound)
            {
                // Full EAX from slot +0x48 is kept as the running result;
                // only AL decides removal.
                uint32_t slotReturn =
                    ((uint32_t(__fastcall*)(void*, void*))
                         CSF_AT(void**, sound, 0)[0x48 / 4])(sound, nullptr);
                result = reinterpret_cast<void*>(static_cast<uintptr_t>(slotReturn));
                removeEntry = (slotReturn & 0xFF) != 0;
            }

            if (removeEntry)
            {
                unsigned int* entrySound = reinterpret_cast<unsigned int*>(
                    reinterpret_cast<uint8_t*>(self->field_0020) + entryOffset + 4);
                ReleaseSoundEntry(
                    reinterpret_cast<void*>(self->field_0040), nullptr, entrySound);

                uint32_t newCount = self->field_0028 - 1;
                self->field_0028 = newCount;
                result = reinterpret_cast<void*>(newCount);

                if (static_cast<uint32_t>(index) != newCount)
                {
                    uint8_t* destination =
                        reinterpret_cast<uint8_t*>(self->field_0020) + entryOffset;
                    result = _memcpy(
                        destination, destination + 24,
                        24 * (newCount - static_cast<uint32_t>(index)));
                }
            }

            --index;
            entryOffset -= 24;
        } while (index >= 0);
    }

    return result;
}

HOOK(0x00522F80, S_00522010_UpdateSoundEntriesAndRemoveFinished, "auto");
