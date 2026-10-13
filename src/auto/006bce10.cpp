#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, SceneLoader_CollectSectorObjects, 0x006C0B90, (int clumpHandle, int collectorHandle));
CSF_FUNC(int, __cdecl, GfxObjectProps_GetObjectAttachment, 0x006B87A0, (int objectIndex));
CSF_FUNC(int, __fastcall, GfxEnvironment_GetAttachmentId, 0x006B8080, (void* self, void* edx));

CSF_GLOBAL(int*, g_SectorObjects, 0x008A7E50);
CSF_GLOBAL(std::uint32_t, g_SectorObjectCount, 0x008A7E58);

// Collects the sector objects of a clump, then returns the first object whose
// attachment has an id other than -1. Falls back to the first collected object.
static int __cdecl GfxEnvironment_FindSectorObjectWithAttachment(int clumpHandle)
{
    SceneLoader_CollectSectorObjects(clumpHandle, static_cast<int>(CSF_ADDR_DATA(0x008A7E50)));

    std::uint32_t objectIndex = 0;
    if (g_SectorObjectCount == 0)
        return g_SectorObjects[0];

    do {
        int objectHandle = g_SectorObjects[objectIndex];
        int attachment = GfxObjectProps_GetObjectAttachment(objectHandle);
        int attachmentId = GfxEnvironment_GetAttachmentId(reinterpret_cast<void*>(attachment), nullptr);
        if (attachmentId != -1)
            return g_SectorObjects[objectIndex];
        ++objectIndex;
    } while (objectIndex < g_SectorObjectCount);

    return g_SectorObjects[0];
}

HOOK(0x006BCE10, GfxEnvironment_FindSectorObjectWithAttachment, "auto");
