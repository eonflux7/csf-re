#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, GetEntryCount, 0x0044BB80, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GetEntryAt, 0x0044BBA0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __cdecl, GetGlobalValue_8B0568, 0x004FDBB0, ());
CSF_FUNC(void*, __fastcall, UmsVirtualProcessor_GetDefaultDestination, 0x004FE4A0, (void* self, void*));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

CSF_GLOBAL(float, g_UnsignedIntegerBias, 0x0082D744);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static std::uint64_t __stdcall ComputeControlAnimationKey(
    int controlAddress, int direction, std::uint32_t* flags, char includeEntries)
{
    void* control = reinterpret_cast<void*>(controlAddress);
    *flags = 14;
    float animationKey = 0.0f;

    if (CSF_AT(int, control, 0x50) != 1)
    {
        *flags = 15;
        return static_cast<std::uint64_t>(
            static_cast<std::int64_t>(animationKey));
    }

    switch (CSF_AT(int, control, 0x7C))
    {
    case 1:
        animationKey = CSF_AT(float,
            reinterpret_cast<void*>(GetGlobalValue_8BC1E0()), 0x1D4C);
        break;
    case 2:
        animationKey = CSF_AT(float,
            reinterpret_cast<void*>(GetGlobalValue_8BC1E0()), 0x1D50);
        break;
    case 3:
        animationKey = CSF_AT(float,
            reinterpret_cast<void*>(GetGlobalValue_8BC1E0()), 0x1D54);
        break;
    case 4:
        animationKey = CSF_AT(float,
            reinterpret_cast<void*>(GetGlobalValue_8BC1E0()), 0x1D58);
        break;
    case 5:
        animationKey = CSF_AT(float,
            reinterpret_cast<void*>(GetGlobalValue_8BC1E0()), 0x1D5C);
        break;
    default:
        *flags = 12;
        break;
    }

    if (direction == 1)
    {
        animationKey += CSF_AT(float,
            reinterpret_cast<void*>(GetGlobalValue_8BC1E0()), 0x1D48);
    }
    else if (direction == 2)
    {
        animationKey += CSF_AT(float,
            reinterpret_cast<void*>(GetGlobalValue_8BC1E0()), 0x1D44);
    }
    else
    {
        *flags &= ~4u;
    }

    if (includeEntries)
    {
        void* entries = UmsVirtualProcessor_GetDefaultDestination(
            reinterpret_cast<void*>(GetGlobalValue_8B0568()), nullptr);

        if (entries)
        {
            const std::uint32_t entryCount =
                static_cast<std::uint32_t>(GetEntryCount(entries, nullptr));

            for (std::uint32_t index = 0; index < entryCount; ++index)
            {
                void* entry = reinterpret_cast<void*>(
                    GetEntryAt(entries, nullptr, static_cast<int>(index)));

                if (CSF_AT(int, entry, 0x00) == 2 &&
                    CHandleTable_Resolve(
                        g_pCurrentScene, nullptr,
                        reinterpret_cast<int>(
                            static_cast<std::uint8_t*>(entry) + 0x10)) ==
                        controlAddress)
                {
                    *flags |= 8u;

                    double entryKey = CSF_AT(std::int32_t, entry, 0x60);
                    if (CSF_AT(std::int32_t, entry, 0x60) < 0)
                        entryKey += g_UnsignedIntegerBias;

                    animationKey = static_cast<float>(entryKey + animationKey);
                    return static_cast<std::uint64_t>(
                        static_cast<std::int64_t>(animationKey));
                }
            }
        }
    }

    return static_cast<std::uint64_t>(
        static_cast<std::int64_t>(animationKey));
}

HOOK(0x00444BB0, ComputeControlAnimationKey, "auto");
