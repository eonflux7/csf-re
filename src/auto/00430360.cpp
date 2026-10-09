#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, GrowButtonArray, 0x0041E5F0, (void* self, void* edx, int growth));
CSF_FUNC(int, __cdecl, ResolveComponentId, 0x0042D750, (void* a1, int a2));
CSF_FUNC(std::uint32_t, __fastcall, MenuExtras_GetChildCount, 0x00692B80, (void* self, void* edx));
CSF_FUNC(std::uint32_t, __fastcall, GetComponent, 0x00692BD0, (unsigned char* node, void* edx, unsigned int index));

// Collects the ids of the three child components of a menu node into the
// id list stored inside the HUD object (at +0x288), growing it as needed.
static void __fastcall Hud_CollectMenuChildIds(void* self, void* /*edx*/, S_00692b80* a2)
{
    if (a2 == nullptr)
        return;

    (void)CSF_VTBL(0x00692B80);
    if (MenuExtras_GetChildCount(a2, nullptr) != 3)
        return;

    std::uint32_t reservedCount = CSF_AT(std::uint32_t, self, 0x28C);
    S_0041e5f0* idList = reinterpret_cast<S_0041e5f0*>(static_cast<std::uint8_t*>(self) + 0x288);

    (void)CSF_VTBL(0x0041E5F0);
    if (reservedCount < 3)
        GrowButtonArray(idList, nullptr, static_cast<int>(3 - reservedCount));

    unsigned int index = 0;
    do {
        std::uint32_t childNode = GetComponent(reinterpret_cast<unsigned char*>(a2), nullptr, index);
        int componentId = ResolveComponentId(reinterpret_cast<void*>(childNode), 0);

        (void)CSF_VTBL(0x0041E5F0);
        if (idList->field_0008 >= idList->field_0004)
            GrowButtonArray(idList, nullptr, idList->field_000c);

        std::uint32_t slot = idList->field_0008;
        int* slotPtr = reinterpret_cast<int*>(idList->field_0000 + 4 * slot);
        ++index;
        idList->field_0008 = slot + 1;
        *slotPtr = componentId;
    } while (index < 3);
}

HOOK(0x00430360, Hud_CollectMenuChildIds, "auto");
