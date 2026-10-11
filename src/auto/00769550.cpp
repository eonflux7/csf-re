#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, MissionScript_GetComponentType, 0x006927F0, (void* self, void* edx));
CSF_FUNC(std::uint32_t, __fastcall, MenuExtras_GetChildCount, 0x00692B80, (void* self, void* edx));
CSF_FUNC(std::uint32_t, __fastcall, GetComponent, 0x00692BD0, (unsigned char* node, void* edx, unsigned int index));
CSF_FUNC(int, __fastcall, ParseIntegerComponent, 0x00692C40, (std::uint32_t component));
CSF_FUNC(char, __fastcall, CopyAttributeText, 0x00692D40, (std::uint8_t* attribute, void* edx, char* buffer, int capacity));
CSF_FUNC(std::uint8_t*, __fastcall, FindProperty, 0x006932B0, (std::uint8_t* self, void* edx, char* keyword));
CSF_FUNC(uint8_t*, __fastcall, LookupNamedElement, 0x00693360, (uint8_t* self, void* edx, const char* name));
CSF_FUNC(int, __fastcall, MissionScript_FindExpressionDefinition, 0x007692F0, (uint32_t* self, void* , char* String1));
CSF_FUNC(void, __fastcall, InputBindings_ParseValueComponent, 0x0078CC90, (void* self, void* edx, void* a2));

CSF_GLOBAL(std::uint32_t, g_NextBindingId, 0x008A9940);
CSF_GLOBAL(std::uint32_t, g_NextAlternateBindingId, 0x008A9944);
CSF_GLOBAL(std::uint32_t, g_Value_8BC1E8, 0x008BC1E8);

[[maybe_unused]] static auto const g_ChildCountVtableReference = CSF_VTBL(0x00692B80);

static int* __fastcall S_00769550_ParseBinding(
    S_00769550* self, void* /*edx*/, S_00692b80* component, int destination)
{
    char buffer[0x100] = {};
    auto* node = reinterpret_cast<std::uint8_t*>(component);
    auto* binding = reinterpret_cast<void*>(destination);
    int* result = nullptr;

    if (MissionScript_GetComponentType(component, nullptr) == 2)
    {
        auto* idComponent = LookupNamedElement(
            node, nullptr, reinterpret_cast<const char*>(CSF_ADDR_DATA(0x008305F4)));
        CSF_AT(std::uint32_t, binding, 0x0) =
            ParseIntegerComponent(reinterpret_cast<std::uint32_t>(idComponent));

        CSF_AT(std::uint8_t, binding, 0x8) =
            FindProperty(node, nullptr,
                         const_cast<char*>(CSF_STR(0x0086F17C, ".ARRAY"))) != nullptr;

        auto* typeComponent =
            LookupNamedElement(node, nullptr, CSF_STR(0x0086F174, ".TYPE"));
        CopyAttributeText(typeComponent, nullptr, buffer, 256);
        int definition = MissionScript_FindExpressionDefinition(
            reinterpret_cast<uint32_t*>(g_Value_8BC1E8), nullptr, buffer);
        CSF_AT(std::uint32_t, binding, 0x4) =
            CSF_AT(std::uint32_t, reinterpret_cast<void*>(definition), 0x8);

        auto* nameComponent =
            LookupNamedElement(node, nullptr, CSF_STR(0x00830F8C, ".NOMBRE"));
        CopyAttributeText(nameComponent, nullptr, buffer, 256);
        auto* valueComponent =
            LookupNamedElement(node, nullptr, CSF_STR(0x0086F16C, ".VALOR"));
        InputBindings_ParseValueComponent(binding, nullptr, valueComponent);
    }
    else
    {
        auto typeComponent = GetComponent(node, nullptr, 0);
        CopyAttributeText(
            reinterpret_cast<std::uint8_t*>(typeComponent), nullptr, buffer, 256);
        int definition = MissionScript_FindExpressionDefinition(
            reinterpret_cast<uint32_t*>(g_Value_8BC1E8), nullptr, buffer);
        CSF_AT(std::uint32_t, binding, 0x4) =
            CSF_AT(std::uint32_t, reinterpret_cast<void*>(definition), 0x8);

        auto nameComponent = GetComponent(node, nullptr, 1u);
        CopyAttributeText(
            reinterpret_cast<std::uint8_t*>(nameComponent), nullptr, buffer, 256);
        auto valueComponent = GetComponent(node, nullptr, 2u);
        InputBindings_ParseValueComponent(
            binding, nullptr, reinterpret_cast<void*>(valueComponent));

        auto childCount = MenuExtras_GetChildCount(component, nullptr);
        result = reinterpret_cast<int*>(childCount);
        if (childCount == 4)
        {
            auto idComponent = GetComponent(node, nullptr, 3u);
            int id = ParseIntegerComponent(idComponent);
            result = reinterpret_cast<int*>(id);
            CSF_AT(std::uint32_t, binding, 0x0) = id;
        }
        else if (self->field_0000)
        {
            CSF_AT(std::uint32_t, binding, 0x0) = g_NextAlternateBindingId;
            ++g_NextAlternateBindingId;
        }
        else
        {
            result = reinterpret_cast<int*>(g_NextBindingId);
            CSF_AT(std::uint32_t, binding, 0x0) = g_NextBindingId;
            ++g_NextBindingId;
        }
    }

    const std::uint32_t id = CSF_AT(std::uint32_t, binding, 0x0);
    if (self->field_0000)
    {
        if (id >= g_NextAlternateBindingId)
            g_NextAlternateBindingId = id + 1;
    }
    else if (id >= g_NextBindingId)
    {
        g_NextBindingId = id + 1;
    }
    return result;
}

HOOK(0x00769550, S_00769550_ParseBinding, "auto");
