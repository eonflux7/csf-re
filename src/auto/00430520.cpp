#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(void*, __fastcall, GrowButtonArray, 0x0041E5F0, (void* self, void* edx, int growth));
CSF_FUNC(int, __cdecl, Hud_ResolveComponentObject, 0x0042D750, (void* a1, int a2));
CSF_FUNC(std::uint32_t, __fastcall, MenuExtras_GetChildCount, 0x00692B80, (void* self, void* edx));
CSF_FUNC(std::uint32_t, __fastcall, GetComponent, 0x00692BD0, (unsigned char* node, void* edx, unsigned int index));
CSF_FUNC(int, __cdecl, GetGlobalValue_753A30, 0x00753A30, ());

static void __fastcall Hud_BindSingleMenuChild(void* self, void* /*edx*/, S_00692b80* menuNode)
{
    // Vtable references made by the original's calls on the menu node and list element types.
    // No objects are constructed here, so these are no-op expressions kept for the address gate.
    static_cast<void>(CSF_VTBL(0x00692B80));
    static_cast<void>(CSF_VTBL(0x0041E5F0));

    CSF_AT(std::uint32_t, self, 0x6C) = 0;

    if (menuNode == nullptr) {
        CSF_AT(std::uint8_t, self, 0x11F) = 0;
        return;
    }
    if (MenuExtras_GetChildCount(menuNode, nullptr) != 1) {
        MenuExtras_GetChildCount(menuNode, nullptr);
        CSF_AT(std::uint8_t, self, 0x11F) = 0;
        return;
    }

    auto* list = reinterpret_cast<S_0041e5f0*>(static_cast<std::uint8_t*>(self) + 0x64);

    if (CSF_AT(std::uint32_t, self, 0x68) == 0) {
        GrowButtonArray(list, nullptr, 1);
    }

    std::uint32_t componentHandle = GetComponent(reinterpret_cast<unsigned char*>(menuNode), nullptr, 0);
    int resolvedObject = Hud_ResolveComponentObject(reinterpret_cast<void*>(componentHandle), 4);

    if (list->field_0008 >= list->field_0004) {
        GrowButtonArray(list, nullptr, static_cast<int>(list->field_000c));
    }
    std::uint32_t appendIndex = list->field_0008;
    int* slots = reinterpret_cast<int*>(list->field_0000);
    list->field_0008 = appendIndex + 1;
    slots[appendIndex] = resolvedObject;

    if (resolvedObject == 0) {
        return;
    }

    std::uint32_t childCount = CSF_AT(std::uint32_t, self, 0x6C);
    for (std::uint32_t i = 0; i < childCount; ++i) {
        void* element = reinterpret_cast<void*>(reinterpret_cast<int*>(list->field_0000)[i]);
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, element, 0)[28])(element, nullptr, 1);
    }

    void* firstElement = reinterpret_cast<void*>(reinterpret_cast<int*>(list->field_0000)[0]);
    int* sizeRect = ((int*(__fastcall*)(void*, void*))CSF_AT(void**, firstElement, 0)[15])(firstElement, nullptr);
    int sizeXY[2];
    sizeXY[0] = sizeRect[0];
    sizeXY[1] = sizeRect[1];

    std::uint8_t* globalValues = reinterpret_cast<std::uint8_t*>(GetGlobalValue_753A30());
    sizeXY[1] = static_cast<int>(static_cast<float>(sizeXY[1]) * CSF_AT(float, globalValues, 0x230));

    void* firstElementAgain = reinterpret_cast<void*>(reinterpret_cast<int*>(list->field_0000)[0]);
    ((void(__fastcall*)(void*, void*, int*))CSF_AT(void**, firstElementAgain, 0)[14])(firstElementAgain, nullptr, sizeXY);

    CSF_AT(std::uint8_t, self, 0x11F) = 1;
}

HOOK(0x00430520, Hud_BindSingleMenuChild, "auto");
