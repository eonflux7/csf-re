#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(char*, __fastcall, S_0046b100_AppendReference, 0x0046B2B0, (void* self, void* edx, int a3));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(std::uint32_t, __fastcall, MenuExtras_GetChildCount, 0x00692B80, (void* self, void* edx));
CSF_FUNC(std::uint32_t, __fastcall, GetComponent, 0x00692BD0, (unsigned char* node, void* edx, unsigned int index));
CSF_FUNC(int, __fastcall, ParseIntegerComponent, 0x00692C40, (std::uint32_t component));
CSF_FUNC(char, __fastcall, CopyAttributeText, 0x00692D40, (std::uint8_t* attribute, void* edx, char* buffer, int capacity));
CSF_FUNC(std::uint8_t*, __fastcall, FindProperty, 0x006932B0, (std::uint8_t* self, void* edx, char* keyword));

[[maybe_unused]] static const auto g_ChildCountVtableAnnotation = CSF_VTBL(0x00692B80);

static void MissionScript_InitializeReference(std::uint8_t* reference, int kind)
{
    CSF_AT(int, reference, 0x00) = -1;
    CSF_AT(int, reference, 0x04) = kind;
    CSF_AT(std::uint8_t, reference, 0x08) = 0;
    CSF_AT(std::uint8_t, reference, 0x09) = 21;
    CSF_AT(std::uint8_t, reference, 0x0A) = 0;
    CSF_AT(std::uint8_t, reference, 0x0C) = 22;
    CSF_AT(float, reference, 0x10) = 0.0f;
    CSF_AT(std::uint8_t, reference, 0x14) = 23;

    // String subobject at +0x18.
    CSF_AT(std::uint8_t, reference, 0x1C) = 0;
    CSF_AT(int, reference, 0x2C) = 0;
    CSF_AT(unsigned int, reference, 0x30) = 15;

    CSF_AT(std::uint8_t, reference, 0x34) = 20;
    CSF_AT(int, reference, 0x38) = 0;
    CSF_AT(int, reference, 0x3C) = 0;
    CSF_AT(int, reference, 0x40) = 0;
}

static void MissionScript_ParseIntegerReferences(
    S_0046b530* self, std::uint8_t* root, const char* keyword, int kind)
{
    std::uint8_t* property = FindProperty(root, nullptr, const_cast<char*>(keyword));
    if (!property)
        return;

    alignas(4) std::uint8_t reference[0x4C] = {};
    MissionScript_InitializeReference(reference, kind);

    const std::uint32_t count = MenuExtras_GetChildCount(property, nullptr);
    if (!count)
        return;

    unsigned int index = 0;
    do
    {
        const std::uint32_t component = GetComponent(property, nullptr, index);
        const int value = ParseIntegerComponent(component);
        CSF_AT(float, reference, 0x10) = static_cast<float>(value);
        S_0046b100_AppendReference(
            reinterpret_cast<std::uint8_t*>(self) + 8,
            nullptr, reinterpret_cast<int>(reference));
        ++index;
    }
    while (index < count);

    if (CSF_AT(unsigned int, reference, 0x30) >= 0x10)
    {
        const std::uint32_t allocation = CSF_AT(std::uint32_t, reference, 0x1C);
        if (allocation)
            Mem_FreeDebug(reinterpret_cast<void*>(allocation));
    }
}

static char* MissionScript_ParseTextReferences(
    S_0046b530* self, std::uint8_t* root, const char* keyword, int kind)
{
    std::uint8_t* property = FindProperty(root, nullptr, const_cast<char*>(keyword));
    if (!property)
        return nullptr;

    alignas(4) std::uint8_t reference[0x4C] = {};
    char buffer[0x204] = {};
    MissionScript_InitializeReference(reference, kind);

    const std::uint32_t count = MenuExtras_GetChildCount(property, nullptr);
    char* result = reinterpret_cast<char*>(count);
    unsigned int index = 0;
    if (count)
    {
        do
        {
            const std::uint32_t component = GetComponent(property, nullptr, index);
            CopyAttributeText(
                reinterpret_cast<std::uint8_t*>(component), nullptr, buffer, 512);
            AssignStringRange_429B60(
                reference + 0x18, nullptr, buffer,
                static_cast<unsigned int>(std::strlen(buffer)));
            result = S_0046b100_AppendReference(
                reinterpret_cast<std::uint8_t*>(self) + 8,
                nullptr, reinterpret_cast<int>(reference));
            ++index;
        }
        while (index < count);
    }

    if (CSF_AT(unsigned int, reference, 0x30) >= 0x10)
    {
        result = reinterpret_cast<char*>(
            CSF_AT(std::uint32_t, reference, 0x1C));
        if (result)
            return reinterpret_cast<char*>(Mem_FreeDebug(result));
    }
    return result;
}

static char* __fastcall S_0046b530_ParseResourceReferences(
    S_0046b530* self, void* /*edx*/, S_00692b80* node)
{
    auto* root = reinterpret_cast<std::uint8_t*>(node);

    MissionScript_ParseIntegerReferences(
        self, root, CSF_STR(0x00831560, ".FNC"), 0);
    MissionScript_ParseIntegerReferences(
        self, root, CSF_STR(0x00831554, ".CLASSID"), 32);
    MissionScript_ParseIntegerReferences(
        self, root, CSF_STR(0x00831548, ".SONIDOSID"), 38);
    MissionScript_ParseIntegerReferences(
        self, root, CSF_STR(0x0083153C, ".EFFCLASSID"), 41);
    MissionScript_ParseIntegerReferences(
        self, root, CSF_STR(0x0083152C, ".ARMACLASSID"), 47);
    MissionScript_ParseIntegerReferences(
        self, root, CSF_STR(0x00831524, ".EFECTO"), 17);
    MissionScript_ParseIntegerReferences(
        self, root, CSF_STR(0x00831514, ".ANIMACIONES"), 36);
    MissionScript_ParseTextReferences(
        self, root, CSF_STR(0x00831508, ".SONIDOS"), 37);
    return MissionScript_ParseTextReferences(
        self, root, CSF_STR(0x00831500, ".FBS"), 18);
}

HOOK(0x0046B530, S_0046b530_ParseResourceReferences, "auto");
