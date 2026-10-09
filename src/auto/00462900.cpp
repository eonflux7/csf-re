#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __cdecl, GetGlobalValue_521BB0, 0x00521BB0, ());
CSF_FUNC(int, __fastcall, ResolveGlobalValueQuery, 0x005242B0, (void* self, void* edx, void* a2, int a3));
CSF_FUNC(int, __cdecl, QueryTargetStateFlag, 0x00755880, (int a1));
CSF_FUNC(int, __fastcall, ResolveSlotIndexForState, 0x00756810, (void* self, void* edx, int a2, int a3));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

// Fills the 16-byte query tag: {vtable, posX, posY, type tag}.
static void WriteQueryTag(uint8_t* queryTag, int posX, int posY)
{
    CSF_AT(void*, queryTag, 0x00) = CSF_VTBL(0x0082D448);
    CSF_AT(int, queryTag, 0x04) = posX;
    CSF_AT(int, queryTag, 0x08) = posY;
    CSF_AT(int, queryTag, 0x0C) = 1232348224; // 0x49742440
}

static int __fastcall SyncTargetFromHandle(void* self, void* /*edx*/,
                                           [[maybe_unused]] int stackArgA,
                                           [[maybe_unused]] int stackArgB,
                                           uint32_t* args)
{
    auto* selfBytes = static_cast<uint8_t*>(self);

    // args[0] -> record; its +4 pointer is the block the new handle key is copied from.
    // Each key part is stored to self+0x60/+0x64 right after it is read, in the original's order.
    auto* sourceRecord = reinterpret_cast<uint8_t*>(args[0]);
    auto* sourceBlock = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, sourceRecord, 4));
    CSF_AT(uint32_t, selfBytes, 0x60) = CSF_AT(uint32_t, sourceBlock, 8);
    CSF_AT(uint32_t, selfBytes, 0x64) = CSF_AT(uint32_t, sourceBlock, 12);

    // Resolve the handle stored at self+0x5C (passed by address) in the current scene.
    const int resolvedTarget = CHandleTable_Resolve(g_pCurrentScene, nullptr,
                                                    reinterpret_cast<int>(selfBytes + 0x5C));
    int result = resolvedTarget;
    if (resolvedTarget != 0)
    {
        auto* targetObject = reinterpret_cast<uint8_t*>(resolvedTarget);
        result = QueryTargetStateFlag(CSF_AT(int, targetObject, 0x50));
        if (static_cast<uint8_t>(result) != 0)
        {
            const int targetPosX = CSF_AT(int, targetObject, 0x20);
            const int targetPosY = CSF_AT(int, targetObject, 0x24);
            const int targetLinkedId = CSF_AT(int, targetObject, 0x50);

            auto* modeRecord = reinterpret_cast<uint8_t*>(args[1]);
            auto* modeBlock = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, modeRecord, 4));
            const bool useSlotMode2 = CSF_AT(uint8_t, modeBlock, 1) != 0;

            // Query key (0x38 bytes) shared by both modes.
            alignas(4) uint8_t queryKey[0x38];
            alignas(4) uint8_t queryTag[0x10];

            CSF_AT(int, queryKey, 0x00) = 2;
            CSF_AT(int, queryKey, 0x04) = 1;
            CSF_AT(int, queryKey, 0x08) = -1;   // slot index, filled per mode
            CSF_AT(int, queryKey, 0x0C) = -1;
            CSF_AT(int, queryKey, 0x10) = 0;
            CSF_AT(int, queryKey, 0x14) = 0;
            CSF_AT(int, queryKey, 0x18) = 0;
            CSF_AT(int, queryKey, 0x1C) = 0;
            CSF_AT(int, queryKey, 0x20) = 0;
            CSF_AT(void*, queryKey, 0x24) = CSF_VTBL(0x0082D448);
            CSF_AT(int, queryKey, 0x28) = targetPosX;
            CSF_AT(int, queryKey, 0x2C) = targetPosY;
            CSF_AT(int, queryKey, 0x30) = 0;
            CSF_AT(uint8_t, queryKey, 0x34) = 0;
            CSF_AT(uint8_t, queryKey, 0x35) = 1;
            CSF_AT(uint8_t, queryKey, 0x36) = 1;

            if (useSlotMode2)
            {
                auto* slotOwner = reinterpret_cast<uint8_t*>(GetGlobalValue_8BC1E0());
                CSF_AT(int, queryKey, 0x08) =
                    ResolveSlotIndexForState(slotOwner + 0x22A4, nullptr, 2, targetLinkedId);
                CSF_AT(uint8_t, queryKey, 0x36) = 0;
                WriteQueryTag(queryTag, targetPosX, targetPosY);

                // The original pushes the key/tag addresses before this call and
                // ResolveGlobalValueQuery (ret 8) pops them.
                const int globalValue = GetGlobalValue_521BB0();
                const int queryResult = ResolveGlobalValueQuery(reinterpret_cast<void*>(globalValue),
                                                                nullptr, queryKey,
                                                                reinterpret_cast<int>(queryTag));
                CSF_AT(int, selfBytes, 0x7C) = queryResult;
                if (queryResult != 0)
                {
                    CSF_AT(int, selfBytes, 0x18) = 9;
                    CSF_AT(uint8_t, selfBytes, 0x82) = 0;
                }
                result = queryResult;
            }
            else
            {
                auto* slotOwner = reinterpret_cast<uint8_t*>(GetGlobalValue_8BC1E0());
                CSF_AT(int, queryKey, 0x08) =
                    ResolveSlotIndexForState(slotOwner + 0x22A4, nullptr, 3, targetLinkedId);
                WriteQueryTag(queryTag, targetPosX, targetPosY);

                const int globalValue = GetGlobalValue_521BB0();
                return ResolveGlobalValueQuery(reinterpret_cast<void*>(globalValue),
                                               nullptr, queryKey,
                                               reinterpret_cast<int>(queryTag));
            }
        }
    }
    return result;
}

HOOK(0x00462900, SyncTargetFromHandle, "auto");
