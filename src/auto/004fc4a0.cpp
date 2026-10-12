#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* , int additionalCapacity));
CSF_FUNC(C_00860178**, __fastcall, S_0042cfd0_Construct, 0x0042D470, (S_0042cfd0* self, void* , int a2));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(std::uint32_t, __fastcall, MenuExtras_GetChildCount, 0x00692B80, (void* self, void* edx));
CSF_FUNC(std::uint32_t, __fastcall, GetComponent, 0x00692BD0, (unsigned char* node, void* edx, unsigned int index));
CSF_FUNC(std::uint8_t*, __fastcall, FindProperty, 0x006932B0, (std::uint8_t* self, void* edx, char* keyword));

CSF_GLOBAL(const char*, g_pszKey_VENTANADRAMATICA, 0x008998FC);

[[maybe_unused]] static const auto DramaticWindowNodeVtable = CSF_VTBL(0x00692B80);
[[maybe_unused]] static const auto DramaticWindowArrayVtable = CSF_VTBL(0x0041E5F0);

static S_00692b80* __fastcall CommandoAnims_LoadDramaticWindows(
    void* self, void* /*edx*/, S_00692b80* properties)
{
    auto* property = FindProperty(
        reinterpret_cast<std::uint8_t*>(properties),
        nullptr,
        const_cast<char*>(g_pszKey_VENTANADRAMATICA));
    if (!property)
        return nullptr;

    const std::uint32_t childCount =
        MenuExtras_GetChildCount(property, nullptr);
    if (!childCount)
        return nullptr;

    auto* windows = reinterpret_cast<S_0041e5f0*>(
        static_cast<std::uint8_t*>(self) + 0x24);

    if (childCount > static_cast<std::uint32_t>(windows->field_0004))
    {
        S_0041e5f0_GrowArrayCapacity(
            windows, nullptr,
            static_cast<int>(childCount -
                             static_cast<std::uint32_t>(windows->field_0004)));
    }

    for (std::uint32_t index = 0; index < childCount; ++index)
    {
        auto* storage = static_cast<S_0042cfd0*>(Mem_Alloc(52));
        C_00860178** window = nullptr;
        if (storage)
        {
            const std::uint32_t component =
                GetComponent(property, nullptr, index);
            window = S_0042cfd0_Construct(
                storage, nullptr, static_cast<int>(component));
        }

        if (static_cast<std::uint32_t>(windows->field_0008) >=
            static_cast<std::uint32_t>(windows->field_0004))
        {
            S_0041e5f0_GrowArrayCapacity(
                windows, nullptr, static_cast<int>(windows->field_000c));
        }

        const std::uint32_t size = windows->field_0008;
        auto* destination =
            reinterpret_cast<C_00860178***>(windows->field_0000) + size;
        windows->field_0008 = size + 1;
        *destination = window;
    }

    return reinterpret_cast<S_00692b80*>(childCount);
}

HOOK(0x004FC4A0, CommandoAnims_LoadDramaticWindows, "auto");
