#include <cstdint>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// S_00692b80's vtable, referenced at the same address as the child-node call target.
[[maybe_unused]] static const auto kNodeVtable = CSF_VTBL(0x00692B80);

CSF_FUNC(std::uint32_t, __fastcall, MenuExtras_GetChildCount, 0x00692B80, (void* self, void* edx));
CSF_FUNC(std::uint32_t, __fastcall, GetComponent, 0x00692BD0, (unsigned char* node, void* edx, unsigned int index));
CSF_FUNC(int, __fastcall, ReadIntegerProperty, 0x00693910, (std::uint8_t* self, void* edx, char* name));
CSF_FUNC(char, __cdecl, ReadIntPairFromNode, 0x0042E640, (void* a1, int* a2, int* a3));
CSF_FUNC(std::uint64_t, __cdecl, ApplyIntPairSetting, 0x0042F3B0, (int* a1));
CSF_FUNC(int, __cdecl, ResolveWidgetFromNode, 0x0042D750, (void* a1, int a2));

CSF_GLOBAL(const char*, g_pszKey_ID, 0x00899544);

// Widget vtable slots (byte offset / 4): 0x3C -> 15, 0x38 -> 14.
using WidgetGetValuesFn = std::uint32_t* (__fastcall*)(void* self, void* edx);
using WidgetSetValuesFn = void (__fastcall*)(void* self, void* edx, std::uint32_t* values);

static void __fastcall S_0042cfd0_LoadChildComponents(S_0042cfd0* self, void* /*edx*/, S_00692b80* node)
{
    if (node == nullptr)
    {
        return;
    }

    const std::uint32_t childCount = MenuExtras_GetChildCount(node, nullptr);
    if (childCount >= 2 && childCount <= 4)
    {
        // Child 0: element id.
        S_00692b80* idComponent = reinterpret_cast<S_00692b80*>(
            GetComponent(reinterpret_cast<unsigned char*>(node), nullptr, 0u));
        self->field_000c = static_cast<std::uint32_t>(
            ReadIntegerProperty(reinterpret_cast<std::uint8_t*>(idComponent), nullptr,
                                const_cast<char*>(g_pszKey_ID)));

        // Child 1: pair of values written into this->field_0030 and this->field_0020.
        S_00692b80* pairComponent = reinterpret_cast<S_00692b80*>(
            GetComponent(reinterpret_cast<unsigned char*>(node), nullptr, 1u));
        ReadIntPairFromNode(reinterpret_cast<void*>(pairComponent),
                            reinterpret_cast<int*>(&self->field_0030),
                            reinterpret_cast<int*>(&self->field_0020));

        if (childCount == 3)
        {
            // Child 2: embedded widget whose value pair is forwarded to its own setter.
            S_00692b80* widgetNode = reinterpret_cast<S_00692b80*>(
                GetComponent(reinterpret_cast<unsigned char*>(node), nullptr, 2u));
            const std::uint32_t widgetHandle =
                static_cast<std::uint32_t>(ResolveWidgetFromNode(reinterpret_cast<void*>(widgetNode), 0));
            self->field_0004 = widgetHandle;
            if (widgetHandle != 0)
            {
                void* widgetObject = reinterpret_cast<void*>(widgetHandle);
                auto getValues = reinterpret_cast<WidgetGetValuesFn>(CSF_AT(void**, widgetObject, 0)[15]);
                std::uint32_t* sourceValues = getValues(widgetObject, nullptr);

                std::uint32_t copiedValues[2];
                copiedValues[0] = sourceValues[0];
                copiedValues[1] = sourceValues[1];
                ApplyIntPairSetting(reinterpret_cast<int*>(copiedValues));

                void* storedWidget = reinterpret_cast<void*>(self->field_0004);
                auto setValues = reinterpret_cast<WidgetSetValuesFn>(CSF_AT(void**, storedWidget, 0)[14]);
                setValues(storedWidget, nullptr, copiedValues);
            }
        }
        else if (childCount == 4)
        {
            // Child 3: second embedded widget.
            S_00692b80* extraNode = reinterpret_cast<S_00692b80*>(
                GetComponent(reinterpret_cast<unsigned char*>(node), nullptr, 3u));
            self->field_0008 = static_cast<std::uint32_t>(
                ResolveWidgetFromNode(reinterpret_cast<void*>(extraNode), 0));
        }
    }
}

HOOK(0x0042CFD0, S_0042cfd0_LoadChildComponents, "auto");
