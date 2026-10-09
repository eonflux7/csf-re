#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(std::uint32_t, __fastcall, MenuExtras_GetChildCount, 0x00692B80, (void* self, void* edx));
CSF_FUNC(std::uint32_t, __fastcall, GetComponent, 0x00692BD0, (unsigned char* node, void* edx, unsigned int index));
CSF_FUNC(double, __fastcall, ReadComponentFloat, 0x00692CC0, (std::uint32_t component));
CSF_FUNC(std::uint8_t*, __fastcall, FindProperty, 0x006932B0, (std::uint8_t* self, void* edx, char* keyword));
CSF_FUNC(int, __fastcall, ReadIntegerAttribute, 0x00693970, (std::uint8_t* node, void* edx, char* name));
CSF_FUNC(double, __fastcall, ReadFloatAttribute, 0x00693990, (std::uint8_t* node, void* edx, char* name));
CSF_FUNC(char, __fastcall, MenuExtras_ReadText, 0x006939C0, (std::uint8_t* self, void* edx, char* keyword, char* buffer, int capacity));
CSF_FUNC(int, __fastcall, DataNode_ReadVector3Property, 0x00693A60, (void* self, void* edx, char* String1, int a3));
CSF_FUNC(unsigned int, __fastcall, DummyList_Reserve, 0x00768AE0, (void* self, void* edx, unsigned int a2));
CSF_FUNC(uint32_t*, __fastcall, DummyList_PushEntry, 0x00768C60, (void* self, void* edx, uint32_t* a3));

CSF_GLOBAL(char*, g_pszKey_DUMMIES, 0x008A9924);
CSF_GLOBAL(char*, g_pszKey_ID_input_bindings, 0x008A9928);
CSF_GLOBAL(char*, g_pszKey_NOMBRE_input_bindings, 0x008A992C);
CSF_GLOBAL(char*, g_pszKey_POS_input_bindings, 0x008A9930);
CSF_GLOBAL(char*, g_pszKey_ROT_input_bindings, 0x008A9934);
CSF_GLOBAL(char*, g_pszKey_ROT_X_input_bindings, 0x008A9938);

static char __fastcall CDummyMesh_LoadDummies(CDummyMesh* self, void* /*edx*/, std::uint8_t* levelNode)
{
    std::uint8_t* dummies = FindProperty(levelNode, nullptr, g_pszKey_DUMMIES);
    if (!dummies)
        return 0;

    char name[256];
    std::memset(name, 0, sizeof(name));

    unsigned int count = MenuExtras_GetChildCount(dummies, nullptr);
    DummyList_Reserve(reinterpret_cast<std::uint8_t*>(self) + 4, nullptr, count);

    unsigned int i = 0;
    if (count) {
        uint32_t temp[6];
        std::memset(&temp[1], 0, 20);
        temp[0] = 0xFFFFFFFFu;
        for (;;) {
            std::uint8_t* child = reinterpret_cast<std::uint8_t*>(
                GetComponent(dummies, nullptr, i));
            DummyList_PushEntry(reinterpret_cast<std::uint8_t*>(self) + 4, nullptr, temp);
            std::uint8_t* entry = reinterpret_cast<std::uint8_t*>(self->m_dummiesEnd) - 24;
            CSF_AT(int, entry, 0) = ReadIntegerAttribute(child, nullptr, g_pszKey_ID_input_bindings);
            MenuExtras_ReadText(child, nullptr, g_pszKey_NOMBRE_input_bindings, name, 0x100);
            DataNode_ReadVector3Property(child, nullptr, g_pszKey_POS_input_bindings,
                                         static_cast<int>(reinterpret_cast<std::uintptr_t>(entry + 4)));
            CSF_AT(float, entry, 0x14) = static_cast<float>(
                ReadFloatAttribute(child, nullptr, g_pszKey_ROT_input_bindings));
            std::uint8_t* rotX = FindProperty(child, nullptr, g_pszKey_ROT_X_input_bindings);
            if (rotX)
                CSF_AT(float, entry, 0x10) = static_cast<float>(
                    ReadComponentFloat(reinterpret_cast<std::uint32_t>(rotX)));
            if (++i >= count)
                break;
        }
    }
    return 1;
}

HOOK(0x00768CF0, CDummyMesh_LoadDummies, "auto");
