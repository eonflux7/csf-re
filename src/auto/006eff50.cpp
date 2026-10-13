#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, SceneLoader_GetObjectRegistry, 0x00726C40, (void* self, void* edx));
CSF_FUNC(void*, __cdecl, _memcpy, 0x007AEE60, (void* destination, const void* source, std::size_t byteCount));

static void __fastcall GfxParticlePtank_RemoveObject(
    void* self, void* /*edx*/, std::uint32_t object)
{
    const std::uint32_t count = CSF_AT(std::uint32_t, self, 0x94);
    if (count == 0)
        return;

    std::uint32_t index = 0;
    const std::uint32_t* entry = CSF_AT(std::uint32_t*, self, 0x8C);
    while (*entry != object)
    {
        ++index;
        ++entry;
        if (index >= count)
            return;
    }

    if (index == static_cast<std::uint32_t>(-1))
        return;

    if (CSF_AT(std::uint8_t, self, 0xAC))
    {
        void* sceneLoader = reinterpret_cast<void*>(GetGlobalInt());
        void* registry = reinterpret_cast<void*>(
            SceneLoader_GetObjectRegistry(sceneLoader, nullptr));
        const std::uint32_t storedObject =
            CSF_AT(std::uint32_t*, self, 0x8C)[index];

        reinterpret_cast<void (__fastcall*)(void*, void*, std::uint32_t)>(
            CSF_AT(void**, registry, 0)[0x18 / 4])(
                registry, nullptr, storedObject);
    }
    else
    {
        void* storedObject = reinterpret_cast<void*>(
            CSF_AT(std::uint32_t*, self, 0x8C)[index]);
        if (storedObject)
        {
            reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, storedObject, 0)[4 / 4])(
                    storedObject, nullptr, 1);
            CSF_AT(std::uint32_t*, self, 0x8C)[index] = 0;
        }
    }

    const std::uint32_t remainingCount =
        CSF_AT(std::uint32_t, self, 0x94) - 1;
    CSF_AT(std::uint32_t, self, 0x94) = remainingCount;

    if (index != remainingCount)
    {
        std::uint32_t* destination =
            CSF_AT(std::uint32_t*, self, 0x8C) + index;
        _memcpy(destination, destination + 1, 4 * (remainingCount - index));
    }
}

HOOK(0x006EFF50, GfxParticlePtank_RemoveObject, "auto");
