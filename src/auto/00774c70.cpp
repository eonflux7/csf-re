#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(std::uint32_t, __fastcall, MenuExtras_GetChildCount, 0x00692B80, (void* self, void* edx));
CSF_FUNC(std::uint32_t, __fastcall, GetComponent, 0x00692BD0, (unsigned char* node, void* edx, unsigned int index));
CSF_FUNC(int, __fastcall, ReadIntegerAttribute, 0x00693970, (std::uint8_t* node, void* edx, char* name));
CSF_FUNC(char, __fastcall, MenuExtras_ReadText, 0x006939C0, (std::uint8_t* self, void* edx, char* keyword, char* buffer, int capacity));
CSF_FUNC(int, __cdecl, CharacterDefs_ParseSoldierAnimTypeName, 0x00761800, (char* String1));
CSF_FUNC(int, __cdecl, CharacterDefs_ParsePlayerAnimTypeName, 0x007618A0, (char* String1));
CSF_FUNC(void, __fastcall, AnimRefList_Add, 0x00771FB0, (void* self, void* edx, uint32_t* a2));

CSF_GLOBAL(const char*, g_pszKey_TIPO_8A99F8, 0x008A99F8);
CSF_GLOBAL(const char*, g_pszKey_ID_8A9A90, 0x008A9A90);

static char __fastcall CLevelData_ReadEntityAnimations(
    CLevelData* self, void* /*edx*/, S_00692b80* node,
    int* entityClassId, S_00771b80* animations)
{
    if (!node)
        return 1;

    const int classId = *entityClassId;
    alignas(8) std::uint8_t bufferBytes[0x104];
    char* buffer = reinterpret_cast<char*>(bufferBytes);
    std::memset(buffer, 0, 256);

    void* classTable = reinterpret_cast<void*>(self->m_entityClassTable);
    const int classIndex =
        reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, classTable, 0)[0x10 / 4])(
                classTable, nullptr, classId);

    classTable = reinterpret_cast<void*>(self->m_entityClassTable);
    void* entityClass =
        reinterpret_cast<void*(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, classTable, 0)[8 / 4])(
                classTable, nullptr, classIndex);

    (void)CSF_VTBL(0x00692B80);
    const std::uint32_t childCount = MenuExtras_GetChildCount(node, nullptr);

    for (std::uint32_t index = 0; index < childCount; ++index)
    {
        auto* component = reinterpret_cast<std::uint8_t*>(
            GetComponent(reinterpret_cast<unsigned char*>(node), nullptr, index));

        alignas(8) std::uint8_t referenceBytes[0xC];
        int* reference = reinterpret_cast<int*>(referenceBytes);
        reference[0] = -1;
        reference[1] = -1;
        reference[2] = -1;

        const int animationId = ReadIntegerAttribute(
            component, nullptr, const_cast<char*>(g_pszKey_ID_8A9A90));

        void* animationTable = reinterpret_cast<void*>(self->field_1fa0);
        reference[0] = animationId;
        reference[2] =
            reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, animationTable, 0)[8 / 4])(
                    animationTable, nullptr, animationId);

        const std::uint32_t classType =
            CSF_AT(std::uint32_t, entityClass, 0xB4);

        if (classType == 0)
        {
            MenuExtras_ReadText(component, nullptr,
                const_cast<char*>(g_pszKey_TIPO_8A99F8), buffer, 256);
            reference[1] = CharacterDefs_ParsePlayerAnimTypeName(buffer);
        }
        else if (classType <= 3)
        {
            MenuExtras_ReadText(component, nullptr,
                const_cast<char*>(g_pszKey_TIPO_8A99F8), buffer, 256);
            reference[1] = CharacterDefs_ParseSoldierAnimTypeName(buffer);
        }

        if (reference[0] != -1 && reference[1] != -1 && reference[2] != -1)
        {
            AnimRefList_Add(animations, nullptr,
                reinterpret_cast<std::uint32_t*>(referenceBytes));
        }
    }

    return 1;
}

HOOK(0x00774C70, CLevelData_ReadEntityAnimations, "auto");
