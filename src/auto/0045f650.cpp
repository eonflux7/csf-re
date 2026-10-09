#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, SceneSlotMgr_IsRejected, 0x004686F0, (void* self, void* edx, int a2, int a3));
CSF_FUNC(uint32_t*, __fastcall, SceneSlotMgr_FindSlot, 0x00469040, (void* self, void* edx, int a2));
CSF_FUNC(void, __fastcall, SceneSlotMgr_ReleaseSlot, 0x00469310, (void* self, void* edx, uint32_t* slot));
CSF_FUNC(int, __fastcall, SceneObj_IsInactive, 0x0048E100, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneObj_ReleaseFromSlot, 0x0048FE90, (void* self, void* edx));

// Reads *(*(message[1] + 4) + 4): the key that the original re-reads for each slot lookup.
static uint32_t ReadSlotKeyFromMessage(uint32_t* message)
{
    auto* messageNode = reinterpret_cast<uint8_t*>(message[1]);
    auto* keyHolder = reinterpret_cast<uint8_t*>(*reinterpret_cast<uint32_t*>(messageNode + 4));
    return *reinterpret_cast<uint32_t*>(keyHolder + 4);
}

static void __stdcall ApplySceneSlotUpdate(int /*unusedA1*/, int /*unusedA2*/, uint32_t* message)
{
    auto* envelope = reinterpret_cast<uint8_t*>(message[0]);
    uint32_t recordAddress = *reinterpret_cast<uint32_t*>(envelope + 4);
    auto* record = reinterpret_cast<uint8_t*>(recordAddress);
    int32_t recordState = *reinterpret_cast<int32_t*>(record + 12);
    int32_t handle = static_cast<int32_t>(recordAddress + 4);

    if (recordState < 0)
        return;

    auto* resolved = reinterpret_cast<C_00833b88*>(CHandleTable_Resolve(g_pCurrentScene, nullptr, handle));
    if (!resolved)
        return;

    auto* object = reinterpret_cast<C_00833908*>(resolved);
    if (static_cast<uint8_t>(SceneObj_IsInactive(object, nullptr)) != 0)
        return;

    if (!object->field_0034)
        return;

    if (SceneSlotMgr_IsRejected(CSF_AT(void*, g_pCurrentScene, 0xBC), nullptr,
                                reinterpret_cast<int>(object),
                                static_cast<int>(ReadSlotKeyFromMessage(message))))
        return;

    uint32_t* slot = SceneSlotMgr_FindSlot(CSF_AT(void*, g_pCurrentScene, 0xBC), nullptr,
                                           static_cast<int>(ReadSlotKeyFromMessage(message)));
    if (!slot)
        return;

    if (*reinterpret_cast<uint8_t*>(slot[4]) != 0)
    {
        SceneObj_ReleaseFromSlot(object, nullptr);
        SceneSlotMgr_ReleaseSlot(CSF_AT(void*, g_pCurrentScene, 0xBC), nullptr, slot);
    }
    else
    {
        slot[2] = *reinterpret_cast<uint32_t*>(record + 8);
        slot[3] = *reinterpret_cast<uint32_t*>(record + 12);
    }
}

HOOK(0x0045F650, ApplySceneSlotUpdate, "auto");
