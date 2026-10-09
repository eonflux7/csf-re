#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));

static bool WeaponObject_IsSelectable(void* object)
{
    return ((bool(__fastcall*)(void*, void*))
        CSF_AT(void**, object, 0)[0x98 / 4])(object, nullptr);
}

static void __fastcall CCharWeaponFsm_SelectWeapon(
    CCharWeaponFsm* self, void* /*edx*/, std::uint32_t* request)
{
    if (self->m_stateCode == 9)
        return;

    const std::uint32_t currentIndex = self->field_0024;
    if (currentIndex == static_cast<std::uint32_t>(-1))
        return;

    int selectedIndex = -1;
    switch (request[0])
    {
    case 0:
        selectedIndex = self->vFindSubObjectIndex(static_cast<int>(request[1]));
        if (selectedIndex != -1)
        {
            void* object =
                reinterpret_cast<void**>(self->field_0018)[selectedIndex];
            if (!WeaponObject_IsSelectable(object))
                selectedIndex = -1;
        }
        break;

    case 1:
    case 2:
    {
        const bool forward = request[0] == 1;
        const std::uint32_t begin = self->field_0018;
        const int count = begin
            ? (static_cast<std::int32_t>(self->field_001c - begin) >> 2)
            : 0;
        const std::uint32_t unsignedCount = static_cast<std::uint32_t>(count);
        const std::uint32_t pendingIndex = self->field_0028;

        std::uint32_t candidate;
        if (forward)
        {
            candidate = pendingIndex == static_cast<std::uint32_t>(-1)
                ? currentIndex + 1
                : pendingIndex + 1;
        }
        else
        {
            candidate = pendingIndex == static_cast<std::uint32_t>(-1)
                ? currentIndex + unsignedCount - 1
                : pendingIndex + unsignedCount - 1;
        }

        selectedIndex = static_cast<std::int32_t>(candidate) % count;
        void* provider = GetCachedObjectProvider(
            reinterpret_cast<void**>(self->field_0018)[selectedIndex],
            nullptr);

        bool acceptable = self->vFindProviderIndex(
            static_cast<int>(reinterpret_cast<std::intptr_t>(provider)))
            >= selectedIndex;
        if (acceptable)
        {
            acceptable = WeaponObject_IsSelectable(
                reinterpret_cast<void**>(self->field_0018)[selectedIndex]);
        }

        if (!acceptable)
        {
            const std::uint32_t firstIndex =
                static_cast<std::uint32_t>(selectedIndex);
            std::uint32_t index = forward
                ? (firstIndex + 1) % unsignedCount
                : (firstIndex + unsignedCount - 1) % unsignedCount;
            selectedIndex = -1;

            while (index != firstIndex)
            {
                provider = GetCachedObjectProvider(
                    reinterpret_cast<void**>(self->field_0018)[index],
                    nullptr);

                const std::uint32_t availableIndex =
                    static_cast<std::uint32_t>(self->vFindProviderIndex(
                        static_cast<int>(
                            reinterpret_cast<std::intptr_t>(provider))));

                if (availableIndex >= index &&
                    WeaponObject_IsSelectable(
                        reinterpret_cast<void**>(self->field_0018)[index]))
                {
                    selectedIndex = static_cast<int>(index);
                    break;
                }

                index = forward
                    ? (index + 1) % unsignedCount
                    : (index - 1) % unsignedCount;
            }
        }
        break;
    }
    }

    self->vSetWeaponStateIfChanged(selectedIndex);
}

HOOK(0x004814E0, CCharWeaponFsm_SelectWeapon, "auto");
