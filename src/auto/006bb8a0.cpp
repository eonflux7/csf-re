#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_GLOBAL(void*, g_EntryTable, 0x008BB47C);
CSF_GLOBAL(void*, g_pRwEngineInstance, 0x008B68C0);
CSF_GLOBAL(int32_t, g_ObjectPropsPluginOffset, 0x008BB474);

CSF_FUNC(int, __cdecl, RwTextureRemoveUserDataArray, 0x0058BDC0, (int a1, int a2));
CSF_FUNC(int, __cdecl, RwTextureGetUserDataArrayCount, 0x0058BDE0, (int a1));
CSF_FUNC(int, __cdecl, RwTextureGetUserDataArray, 0x0058BE10, (int a1, int a2));
CSF_FUNC(int, __fastcall, GfxObjectProps_ReleaseProperties, 0x006B9CB0, (void* self, void* edx, int* a2));
CSF_FUNC(uint32_t*, __fastcall, GfxObjectProps_AllocateProperties, 0x006BA400, (void* self, void* edx));
CSF_FUNC(uint32_t*, __cdecl, GfxObjectProps_GetDefaultProperties, 0x006BA5B0, ());
CSF_FUNC(char, __cdecl, GfxObjectProps_ParseUserData, 0x006BADA0, (int a1, int a2, int a3, int a4));

static void* __cdecl GfxObjectProps_PrepareObject(uint32_t* object)
{
    void* result = g_EntryTable;
    if (CSF_AT(uint32_t, object, reinterpret_cast<uintptr_t>(result) + 4))
        return result;

    void* manager = CSF_AT(void*, g_pRwEngineInstance,
                           g_ObjectPropsPluginOffset + 4);
    int properties = 0;
    bool parsedAny = false;
    const int objectAddress = reinterpret_cast<int>(object);
    const int count = RwTextureGetUserDataArrayCount(objectAddress);

    if (count != 0)
    {
        properties = reinterpret_cast<int>(
            GfxObjectProps_AllocateProperties(manager, nullptr));

        int index = static_cast<int32_t>(static_cast<uint32_t>(count) - 1);
        while (index >= 0)
        {
            int userData = RwTextureGetUserDataArray(objectAddress, index);
            if (GfxObjectProps_ParseUserData(properties, userData,
                                             objectAddress, index))
            {
                RwTextureRemoveUserDataArray(objectAddress, index);
                parsedAny = true;
            }
            --index;
        }
    }

    if (parsedAny)
    {
        result = reinterpret_cast<void*>(properties);
        CSF_AT(uint32_t, object,
               reinterpret_cast<uintptr_t>(g_EntryTable) + 4) =
            static_cast<uint32_t>(properties);
        return result;
    }

    GfxObjectProps_ReleaseProperties(manager, nullptr, &properties);
    uint8_t* entry = reinterpret_cast<uint8_t*>(object) +
                     reinterpret_cast<uintptr_t>(g_EntryTable);
    result = GfxObjectProps_GetDefaultProperties();
    CSF_AT(void*, entry, 4) = result;
    return result;
}

HOOK(0x006BB8A0, GfxObjectProps_PrepareObject, "auto");
