#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));

// Fills a 4-dword debug record (handle id, handle generation, vtable, scene tag) and returns it.
static int* __fastcall S_004bd310_FillDebugRecord(S_004bd310* self, void* /*edx*/, int* outRecord, int a2)
{
    // Scene tag: *(*(g_pCurrentScene + 0xB0) + 8)
    uint32_t sceneTablePtr = CSF_AT(uint32_t, g_pCurrentScene, 0xB0);
    int sceneTag = CSF_AT(int, reinterpret_cast<void*>(sceneTablePtr), 8);

    int handleId = -1;
    int handleGen = -1;

    void* sourceRecord = reinterpret_cast<void*>(static_cast<uintptr_t>(a2));
    if (CSF_AT(int, reinterpret_cast<void*>(self->field_0000), 0x20) != CSF_AT(int, sourceRecord, 4)
        && CHandleTable_Resolve(g_pCurrentScene, nullptr, a2))
    {
        handleId = CSF_AT(int, sourceRecord, 4);
        handleGen = CSF_AT(int, sourceRecord, 8);
    }

    outRecord[1] = handleId;
    outRecord[2] = handleGen;
    *reinterpret_cast<void***>(outRecord) = CSF_VTBL(0x0082D448);
    outRecord[3] = sceneTag;
    return outRecord;
}

HOOK(0x004BD310, S_004bd310_FillDebugRecord, "auto");
