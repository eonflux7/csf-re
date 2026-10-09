#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(uint32_t*, __fastcall, InitStateBlockDefaults, 0x0045BC30, (uint32_t* self, void*));
CSF_FUNC(int, __fastcall, HandleTargetUnavailable, 0x0048FE90, (void* self, void* edx));

// Processes a target-update command record. Resolves the handle in the current scene, checks the
// target's state, builds a state block from the record and forwards it to the target's sink.
// If the status-flag owner reports bit 2, copies two dwords from the handle record into this.
static int __fastcall ProcessHandleTargetCommand(void* self, void* /*edx*/,
                                                 [[maybe_unused]] int a2,
                                                 [[maybe_unused]] int a3,
                                                 uint32_t a4)
{
    // a4 -> command record; record[0] -> header block; header block[4] -> handle record
    uint32_t* commandRecord = reinterpret_cast<uint32_t*>(a4);
    uint32_t* headerBlock = reinterpret_cast<uint32_t*>(commandRecord[0]);
    uint32_t handleRecord = headerBlock[1];

    int result = *reinterpret_cast<int*>(handleRecord + 12);
    if (result < 0)
        return result;

    uint32_t handleSlot = handleRecord + 4;
    result = CHandleTable_Resolve(g_pCurrentScene, nullptr, static_cast<int>(handleSlot));
    C_00833908* target = reinterpret_cast<C_00833908*>(result);
    if (target == nullptr)
        return result;

    if (target->field_0034 == 0)
        return 0;

    if (!target->vGetField016c())
        return HandleTargetUnavailable(target, nullptr);

    void* stateOwner = reinterpret_cast<void*>(target->vGetField016c());
    uint8_t isStateBlocked =
        ((uint8_t(__fastcall*)(void*, void*))CSF_AT(void**, stateOwner, 0)[42])(stateOwner, nullptr);
    if (isStateBlocked)
        return HandleTargetUnavailable(target, nullptr);

    uint8_t stateBlock[24];
    InitStateBlockDefaults(reinterpret_cast<uint32_t*>(stateBlock), nullptr);

    // record[1] -> P; P[1] -> Q; payload = Q + 4
    uint32_t payloadAddr = reinterpret_cast<uint32_t*>(commandRecord[1])[1] + 4;
    uint32_t* payload = reinterpret_cast<uint32_t*>(payloadAddr);

    uint32_t* blockWords = reinterpret_cast<uint32_t*>(stateBlock);
    blockWords[1] = payload[0];
    blockWords[2] = payload[1];
    blockWords[3] = payload[2];
    stateBlock[21] = 1;

    C_008672e4* stateSink = reinterpret_cast<C_008672e4*>(target->field_0034);
    result = ((int(__fastcall*)(void*, void*, void*))stateSink->vtbl[42])(stateSink, nullptr, stateBlock);

    uint32_t statusFlagOwnerAddr = target->field_0030;
    if (statusFlagOwnerAddr != 0) {
        void* statusFlagOwner = reinterpret_cast<void*>(statusFlagOwnerAddr);
        result = ((int(__fastcall*)(void*, void*, uint32_t*))
                      CSF_AT(void**, statusFlagOwner, 0)[47])(statusFlagOwner, nullptr, &a4);
        if (*reinterpret_cast<uint8_t*>(result) & 2) {
            result = static_cast<int>(reinterpret_cast<uintptr_t>(self));
            CSF_AT(uint32_t, self, 0x60) = *reinterpret_cast<uint32_t*>(handleSlot + 4);
            CSF_AT(uint32_t, self, 0x64) = *reinterpret_cast<uint32_t*>(handleSlot + 8);
            CSF_AT(uint8_t, self, 0x82) = 0;
        }
    }

    return result;
}

HOOK(0x0045E0A0, ProcessHandleTargetCommand, "auto");
