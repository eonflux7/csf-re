#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_GLOBAL(void*, g_pRwEngineInstance, 0x008B68C0);
CSF_GLOBAL(int, g_ObjectPropsPluginOffset, 0x008BB474);
CSF_GLOBAL(void*, g_EntryTable, 0x008BB480);

CSF_FUNC(int, __cdecl, RwFrameRemoveUserDataArray, 0x0058BD40, (int a1, int a2));
CSF_FUNC(int, __cdecl, RpMaterialGetUserDataArrayCount, 0x0058BD60, (int a1));
CSF_FUNC(int, __cdecl, RpMaterialGetUserDataArray, 0x0058BD90, (int a1, int a2));
CSF_FUNC(int, __cdecl, RwFrameForAllChildren, 0x006237C0,
         (int a1, int (__cdecl *a2)(int, int), int a3));
CSF_FUNC(int, __fastcall, GfxObjectProps_ReleasePropertyBlock, 0x006B9E10,
         (void* self, void* edx, int* a2));
CSF_FUNC(uint32_t*, __fastcall, GfxObjectProps_CreatePropertyBlock, 0x006BA460,
         (void* self, void* edx));
CSF_FUNC(char, __cdecl, GfxObjectProps_ImportUserDataArray, 0x006BAEB0,
         (void** a1, int a2, int a3, int a4));

static int __cdecl GfxObjectProps_ExtractFrameProperties(int frame)
{
    void* frameObject = reinterpret_cast<void*>(frame);

    if (!CSF_AT(int, frameObject,
                reinterpret_cast<uintptr_t>(g_EntryTable) + 4))
    {
        int properties = 0;
        bool imported = false;
        const int count = RpMaterialGetUserDataArrayCount(frame);

        if (count != 0)
        {
            properties = reinterpret_cast<int>(
                GfxObjectProps_CreatePropertyBlock(
                    CSF_AT(void*, g_pRwEngineInstance,
                           g_ObjectPropsPluginOffset + 8),
                    nullptr));

            int index = static_cast<int>(static_cast<uint32_t>(count) - 1);
            while (index >= 0)
            {
                const int userData = RpMaterialGetUserDataArray(frame, index);
                // The getter's two cdecl arguments remain on the stack
                // and become the importer's third and fourth arguments.
                if (GfxObjectProps_ImportUserDataArray(
                        reinterpret_cast<void**>(properties),
                        userData, frame, index))
                {
                    RwFrameRemoveUserDataArray(frame, index);
                    imported = true;
                }
                --index;
            }
        }

        if (imported)
        {
            CSF_AT(int, frameObject,
                   reinterpret_cast<uintptr_t>(g_EntryTable) + 4) = properties;
        }
        else
        {
            GfxObjectProps_ReleasePropertyBlock(
                CSF_AT(void*, g_pRwEngineInstance,
                       g_ObjectPropsPluginOffset + 8),
                nullptr, &properties);
        }
    }

    return RwFrameForAllChildren(
        frame,
        reinterpret_cast<int (__cdecl*)(int, int)>(
            CSF_ADDR_FN(0x006B8680)),
        0);
}

HOOK(0x006BB960, GfxObjectProps_ExtractFrameProperties, "auto");
