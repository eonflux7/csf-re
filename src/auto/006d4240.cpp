#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, RpHAnimFrameGetHierarchy, 0x0056CF90, (int a1));
CSF_FUNC(int, __cdecl, RpHAnimFrameGetID, 0x0056CFA0, (int a1));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(int, __cdecl, GfxObjectProps_GetEntryWord, 0x006B87E0, (int objectIndex));
CSF_FUNC(int, __cdecl, SceneLoader_CollectSectorObjects, 0x006C0B90, (int clumpHandle, int collectorHandle));

static uint32_t __cdecl GfxClumpDict_IsFrameRequired(
    hook::Regs* regs, const uint32_t* /*args*/)
{
    void* clump = reinterpret_cast<void*>(regs->eax);
    void* frame = reinterpret_cast<void*>(regs->esi);
    const int frameHandle = static_cast<int>(regs->esi);

    if (CSF_AT(void*, clump, 0x4) == frame)
        return 1;

    const int id = RpHAnimFrameGetID(frameHandle);
    if (id == 40 || id == 50 || id == 60 || id == 100 ||
        id == 70 || id == 71 || id == 72)
        return 1;

    if (CSF_AT(void*, frame, 0x90) ==
        reinterpret_cast<uint8_t*>(frame) + 0x90)
    {
        alignas(8) uint8_t collectorBytes[0x10];
        CSF_AT(uint32_t, collectorBytes, 0x0) = 0;
        CSF_AT(uint32_t, collectorBytes, 0x4) = 0;
        CSF_AT(uint32_t, collectorBytes, 0x8) = 0;
        CSF_AT(int, collectorBytes, 0xC) = 10;

        SceneLoader_CollectSectorObjects(
            static_cast<int>(regs->eax),
            static_cast<int>(reinterpret_cast<uintptr_t>(collectorBytes)));

        const uint32_t count = CSF_AT(uint32_t, collectorBytes, 0x8);
        void** objects = CSF_AT(void**, collectorBytes, 0x0);
        bool found = false;
        for (uint32_t i = 0; i < count; ++i)
        {
            if (found)
                break;
            found = CSF_AT(void*, objects[i], 0x4) == frame;
        }

        if (objects)
            Mem_Free(objects);

        if (found)
            return 1;
        if (RpHAnimFrameGetHierarchy(frameHandle))
            return 1;
        if (GfxObjectProps_GetEntryWord(frameHandle))
            return 1;
        return 0;
    }

    return 1;
}

HOOK_REGS(0x006D4240, GfxClumpDict_IsFrameRequired, 0, "gfx_clump_dict");
