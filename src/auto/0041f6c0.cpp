#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void*, __fastcall, AiTaskStates_GrowSlotStorage, 0x004DE9B0,
         (void* self, void* edx, int a2));

static std::uint32_t __stdcall AiTaskStates_ResetSoundSlotFlags(
    std::uint32_t* slots, char* state)
{
    const char savedMode = CSF_AT(char, state, 0x15);
    const bool useExistingCount = CSF_AT(char, state, 0x14) != 0;
    const char initialFailure = CSF_AT(char, state, 0);
    std::uint32_t count = 0;
    if (useExistingCount)
        count = CSF_AT(std::uint32_t, slots, 0x08);

    std::uint32_t result = 0;
    if (!initialFailure)
    {
        void* owner = CSF_AT(void*, state, 0x04);
        result = !((bool(__fastcall*)(void*, void*, std::uint32_t*, char*))
                       CSF_AT(void**, owner, 0)[0x78 / 4])(
            owner, nullptr, &count, state);
    }
    CSF_AT(char, state, 0) = static_cast<char>(result);

    char failed = 1;
    if (!result)
    {
        if (useExistingCount || savedMode)
        {
            std::uint32_t index = 0;
            std::uint32_t offset = 0;
            while (index < count)
            {
                void* entry = reinterpret_cast<void*>(
                    CSF_AT(std::uint32_t, slots, 0) + offset);
                const bool entryFailed = CSF_AT(char, state, 0) != 0;
                CSF_AT(std::uint32_t, state, 0x1C) = 0;
                if (!entryFailed)
                {
                    void* owner;
                    if (useExistingCount)
                    {
                        CSF_AT(char, state, 0x15) = savedMode;
                        owner = CSF_AT(void*, state, 0x04);
                    }
                    else
                    {
                        owner = CSF_AT(void*, state, 0x04);
                        CSF_AT(char, state, 0x15) = savedMode;
                    }
                    CSF_AT(char, state, 0x16) = 0;
                    CSF_AT(char, state, 0) =
                        ((int(__fastcall*)(void*, void*, void*, char*))
                             CSF_AT(void**, owner, 0)[0x38 / 4])(
                            owner, nullptr, entry, state) == -1;
                }
                result = ++index;
                offset += 12;
            }
        }
        else
        {
            result = CSF_AT(std::uint32_t, slots, 0x04);
            CSF_AT(std::uint32_t, slots, 0x08) = 0;
            if (count > result)
            {
                result = reinterpret_cast<std::uintptr_t>(
                    AiTaskStates_GrowSlotStorage(
                        slots, nullptr, static_cast<int>(count - result)));
            }

            std::uint32_t index = 0;
            std::uint32_t offset = 0;
            while (index < count)
            {
                if (CSF_AT(std::uint32_t, slots, 0x08) >=
                    CSF_AT(std::uint32_t, slots, 0x04))
                {
                    AiTaskStates_GrowSlotStorage(
                        slots, nullptr, CSF_AT(int, slots, 0x0C));
                }

                const std::uint32_t used =
                    CSF_AT(std::uint32_t, slots, 0x08);
                void* newEntry = reinterpret_cast<void*>(
                    CSF_AT(std::uint32_t, slots, 0) + 12 * used);
                CSF_AT(std::uint32_t, slots, 0x08) = used + 1;
                CSF_AT(std::uint32_t, newEntry, 0) = 0;
                CSF_AT(std::uint32_t, newEntry, 0x04) = 0;
                CSF_AT(std::uint32_t, newEntry, 0x08) = 0;

                void* entry = reinterpret_cast<void*>(
                    CSF_AT(std::uint32_t, slots, 0) + offset);
                const bool entryFailed = CSF_AT(char, state, 0) != 0;
                CSF_AT(std::uint32_t, state, 0x1C) = 0;
                if (!entryFailed)
                {
                    void* owner = CSF_AT(void*, state, 0x04);
                    CSF_AT(char, state, 0x15) = 0;
                    CSF_AT(char, state, 0x16) = 0;
                    CSF_AT(char, state, 0) =
                        ((int(__fastcall*)(void*, void*, void*, char*))
                             CSF_AT(void**, owner, 0)[0x38 / 4])(
                            owner, nullptr, entry, state) == -1;
                }
                result = ++index;
                offset += 12;
            }
        }

        if (!CSF_AT(char, state, 0))
        {
            void* owner = CSF_AT(void*, state, 0x04);
            result = ((bool(__fastcall*)(void*, void*, char*))
                          CSF_AT(void**, owner, 0)[0x7C / 4])(
                owner, nullptr, state);
            if (result)
                failed = 0;
        }
    }

    CSF_AT(char, state, 0) = failed;
    return result;
}

HOOK(0x0041F6C0, AiTaskStates_ResetSoundSlotFlags, "auto");

