#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(std::uint32_t, __fastcall, MenuExtras_GetChildCount, 0x00692B80, (void* self, void* edx));
CSF_FUNC(std::uint32_t, __fastcall, GetComponent, 0x00692BD0, (unsigned char* node, void* edx, unsigned int index));
CSF_FUNC(uint8_t*, __fastcall, LookupNamedElement, 0x00693360, (uint8_t* self, void* edx, const char* name));
CSF_FUNC(int, __fastcall, ReadIntegerAttribute, 0x00693970, (std::uint8_t* node, void* edx, char* name));
CSF_FUNC(double, __fastcall, ReadFloatAttribute, 0x00693990, (std::uint8_t* node, void* edx, char* name));
CSF_FUNC(char, __fastcall, MenuExtras_ReadText, 0x006939C0, (std::uint8_t* self, void* edx, char* keyword, char* buffer, int capacity));
CSF_FUNC(uint32_t*, __fastcall, AnimationDefs_AssignFile, 0x007730D0, (void* self, void* edx, void* a3));
CSF_FUNC(int, __fastcall, AnimationDefs_ResizeEntries, 0x00774160, (void* self, void* edx, unsigned int a2));

CSF_GLOBAL(const char*, g_pszKey_LISTADATOS_8A9984, 0x008A9984);
CSF_GLOBAL(const char*, g_pszKey_ID_8A9988, 0x008A9988);
CSF_GLOBAL(const char*, g_pszKey_NOMBRE_8A998C, 0x008A998C);
CSF_GLOBAL(const char*, g_pszKey_FILE_8A9998, 0x008A9998);
CSF_GLOBAL(const char*, g_pszKey_LOOP_8A99A0, 0x008A99A0);
CSF_GLOBAL(const char*, g_pszKey_BLEND_IN, 0x008A99A4);
CSF_GLOBAL(const char*, g_pszKey_VEL, 0x008A99A8);
CSF_GLOBAL(const char*, g_pszKey_TRASLACION, 0x008A99AC);

static char __fastcall BddAnimationDefs_LoadAnimationData(
    void* self, void* /*edx*/, S_00692b80* source)
{
    alignas(8) uint8_t textBytes[0x100];
    char* text = reinterpret_cast<char*>(textBytes);
    std::memset(text, 0, sizeof(textBytes));

    // The packet also classifies the direct child-count call as a vtable reference.
    (void)CSF_VTBL(0x00692B80);

    uint8_t* list = LookupNamedElement(
        reinterpret_cast<uint8_t*>(source), nullptr, g_pszKey_LISTADATOS_8A9984);
    const unsigned int count = MenuExtras_GetChildCount(list, nullptr);
    AnimationDefs_ResizeEntries(CSF_AT(void*, self, 0x4), nullptr, count);

    alignas(8) uint8_t fileBytes[0x38];
    C_0082d410& file = *reinterpret_cast<C_0082d410*>(fileBytes);

    unsigned int entryOffset = 0;
    for (unsigned int index = 0; index < count; ++index, entryOffset += 96)
    {
        uint8_t* component = reinterpret_cast<uint8_t*>(
            GetComponent(list, nullptr, index));

        uint8_t* entry =
            CSF_AT(uint8_t*, CSF_AT(void*, self, 0x4), 0x4) + entryOffset;
        const int id = ReadIntegerAttribute(
            component, nullptr, const_cast<char*>(g_pszKey_ID_8A9988));
        CSF_AT(int, entry, 0x0) = id;

        MenuExtras_ReadText(component, nullptr,
            const_cast<char*>(g_pszKey_NOMBRE_8A998C), text, 256);
        entry = CSF_AT(uint8_t*, CSF_AT(void*, self, 0x4), 0x4) + entryOffset;
        AssignStringRange_429B60(entry + 4, nullptr, text,
            static_cast<unsigned int>(std::strlen(text)));

        MenuExtras_ReadText(component, nullptr,
            const_cast<char*>(g_pszKey_FILE_8A9998), text, 256);
        CSF_AT(uint32_t, fileBytes, 0x14) = 0;
        reinterpret_cast<uint8_t*>(&file.field_0004)[0] = 0;
        CSF_AT(void*, fileBytes, 0x20) = nullptr;
        CSF_AT(uint32_t, fileBytes, 0x24) = 0;
        CSF_AT(uint32_t, fileBytes, 0x28) = 0;
        CSF_AT(uint32_t, fileBytes, 0x18) = 15;
        AssignStringRange_429B60(&file, nullptr, text,
            static_cast<unsigned int>(std::strlen(text)));

        entry = CSF_AT(uint8_t*, CSF_AT(void*, self, 0x4), 0x4) + entryOffset;
        AnimationDefs_AssignFile(entry + 32, nullptr, &file);

        entry = CSF_AT(uint8_t*, CSF_AT(void*, self, 0x4), 0x4) + entryOffset;
        const double blendIn = ReadFloatAttribute(
            component, nullptr, const_cast<char*>(g_pszKey_BLEND_IN));
        CSF_AT(float, entry, 0x34) = static_cast<float>(blendIn);

        entry = CSF_AT(uint8_t*, CSF_AT(void*, self, 0x4), 0x4) + entryOffset;
        const double speed = ReadFloatAttribute(
            component, nullptr, const_cast<char*>(g_pszKey_VEL));
        CSF_AT(float, entry, 0x30) = static_cast<float>(speed);

        entry = CSF_AT(uint8_t*, CSF_AT(void*, self, 0x4), 0x4) + entryOffset;
        const int translation = ReadIntegerAttribute(
            component, nullptr, const_cast<char*>(g_pszKey_TRASLACION));
        CSF_AT(uint8_t, entry, 0x59) = translation == 1;

        entry = CSF_AT(uint8_t*, CSF_AT(void*, self, 0x4), 0x4) + entryOffset;
        const int loop = ReadIntegerAttribute(
            component, nullptr, const_cast<char*>(g_pszKey_LOOP_8A99A0));
        CSF_AT(uint8_t, entry, 0x58) = loop == 1;

        if (CSF_AT(void*, fileBytes, 0x20))
            Mem_FreeDebug(CSF_AT(void*, fileBytes, 0x20));
        CSF_AT(void*, fileBytes, 0x20) = nullptr;
        CSF_AT(uint32_t, fileBytes, 0x24) = 0;
        CSF_AT(uint32_t, fileBytes, 0x28) = 0;

        if (CSF_AT(uint32_t, fileBytes, 0x18) >= 0x10 && file.field_0004)
            Mem_FreeDebug(reinterpret_cast<void*>(file.field_0004));

        CSF_AT(uint32_t, fileBytes, 0x18) = 15;
        CSF_AT(uint32_t, fileBytes, 0x14) = 0;
        reinterpret_cast<uint8_t*>(&file.field_0004)[0] = 0;
    }
    return 1;
}

HOOK(0x007741F0, BddAnimationDefs_LoadAnimationData, "auto");
