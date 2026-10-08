#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __fastcall, AssignChildUpdateList, 0x0071C140,
         (void* destination, void* edx, void* source));

static char __fastcall UpdateChildrenAndElapsedTime(
    void* self, void* /*edx*/, float deltaTime)
{
    reinterpret_cast<void(__fastcall*)(void*, void*)>(
        CSF_AT(void**, self, 0)[37])(self, nullptr);

    reinterpret_cast<void(__fastcall*)(void*, void*)>(
        CSF_AT(void**, self, 0)[36])(self, nullptr);

    void* controller = CSF_AT(void*, self, 0x40);
    reinterpret_cast<void(__fastcall*)(void*, void*, float)>(
        CSF_AT(void**, controller, 0)[79])(controller, nullptr, deltaTime);

    AssignChildUpdateList(
        static_cast<std::uint8_t*>(self) + 0x54, nullptr,
        static_cast<std::uint8_t*>(self) + 0x44);

    const std::uint32_t childrenBegin = CSF_AT(std::uint32_t, self, 0x58);
    const std::uint32_t childCount = childrenBegin
        ? static_cast<std::uint32_t>(
              static_cast<std::int32_t>(
                  CSF_AT(std::uint32_t, self, 0x5C) - childrenBegin) >> 2)
        : 0;

    for (std::uint32_t childIndex = 0; childIndex < childCount; ++childIndex)
    {
        const std::uint32_t exclusionsBegin =
            CSF_AT(std::uint32_t, self, 0x34);
        const std::uint32_t exclusionCount = exclusionsBegin
            ? static_cast<std::uint32_t>(
                  static_cast<std::int32_t>(
                      CSF_AT(std::uint32_t, self, 0x38) - exclusionsBegin) >> 3)
            : 0;

        std::uint32_t exclusionIndex = 0;
        if (exclusionCount != 0)
        {
            auto* entry = CSF_AT(std::uint32_t*, self, 0x34);
            while (*entry != childIndex)
            {
                ++exclusionIndex;
                entry += 2;
                if (exclusionIndex >= exclusionCount)
                    break;
            }
        }

        if (exclusionIndex >= exclusionCount)
        {
            void* child = CSF_AT(void**, self, 0x58)[childIndex];
            const unsigned char shouldUpdate =
                reinterpret_cast<unsigned char(__fastcall*)(
                    void*, void*, void*, int)>(
                    CSF_AT(void**, self, 0)[29])(self, nullptr, child, 0);

            if (shouldUpdate)
            {
                child = CSF_AT(void**, self, 0x58)[childIndex];
                reinterpret_cast<void(__fastcall*)(void*, void*, float)>(
                    CSF_AT(void**, child, 0)[4])(child, nullptr, deltaTime);
            }
        }
    }

    const char accumulateTime = CSF_AT(char, self, 0xA0);
    if (accumulateTime)
        CSF_AT(float, self, 0x9C) = deltaTime + CSF_AT(float, self, 0x9C);

    return accumulateTime;
}

HOOK(0x0071D520, UpdateChildrenAndElapsedTime, "auto");
