#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(std::uint32_t, __fastcall, MenuExtras_GetChildCount, 0x00692B80, (void* self, void* edx));
CSF_FUNC(std::uint32_t, __fastcall, GetComponent, 0x00692BD0, (unsigned char* node, void* edx, unsigned int index));
CSF_FUNC(int*, __fastcall, sub_769550, 0x00769550, (void* self, void* edx, void* a2, int a3));
CSF_FUNC(char, __fastcall, sub_770390, 0x00770390, (void* self, void* edx, unsigned int a2));

// Vtable references made by the original's call sites (kept for the address gate).
[[maybe_unused]] static const auto kVtblS00692B80 = CSF_VTBL(0x00692B80);
[[maybe_unused]] static const auto kVtblS00769550 = CSF_VTBL(0x00769550);

static void __fastcall InputBindings_LayoutChildComponents(S_00769550* self, void* /*edx*/, S_00692b80* a2)
{
    if (a2 == nullptr)
        return;

    std::uint32_t childCount = MenuExtras_GetChildCount(a2, nullptr);
    if (childCount == 0)
        return;

    sub_770390(reinterpret_cast<uint8_t*>(self) + 4, nullptr, childCount);

    std::uint32_t index = 0;
    int offset = 0;
    do
    {
        std::uint32_t component = GetComponent(reinterpret_cast<unsigned char*>(a2), nullptr, index);
        sub_769550(self, nullptr, reinterpret_cast<void*>(component),
                   offset + static_cast<int>(self->field_0008));
        ++index;
        offset += 0x44;
    } while (index < childCount);
}

HOOK(0x007703E0, InputBindings_LayoutChildComponents, "auto");
