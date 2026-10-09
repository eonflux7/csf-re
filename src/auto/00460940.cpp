#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, GetPointerRangeElementCount, 0x0045BCF0, (void* self, void*));
CSF_FUNC(unsigned int, __fastcall, CharActionFsm_GetMaskId, 0x00483130, (void* self, void* edx));
CSF_FUNC(void, __fastcall, CharActionFsm_SetFlag, 0x00485080, (void* self, void* edx, uint32_t a2));
CSF_FUNC(int, __fastcall, CharActionFsm_ResolveIndex, 0x00485980, (void* self, void* edx, int a2));
CSF_FUNC(uint32_t, __fastcall, CharActionFsm_ApplyIndex, 0x00486CC0, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __cdecl, QueryObjectStateFlag, 0x00755810, (unsigned int a1));
CSF_FUNC(int, __cdecl, QueryDisplayStateFlag, 0x00760F90, (int a1));

// Reimplementation of 0x00460940 (__stdcall, 3 stack args, ret 12).
// Resolves the handle record referenced by commandRefs[0] through g_pCurrentScene,
// then dispatches the action index (obtained from the services subsystem) to the
// CCharActionFsm stored at offset 68 of the resolved object.
static unsigned int __stdcall ApplyCharActionFromHandleRef(int /*a2*/, int /*a3*/, uint32_t* commandRefs)
{
    // v4 = *(*commandRefs + 4); v5 = *(v4 + 12); result = v4 + 4
    uint8_t* commandRecord = reinterpret_cast<uint8_t*>(commandRefs[0]);
    uint32_t handleRecordAddr = CSF_AT(uint32_t, commandRecord, 4);
    uint8_t* handleRecord = reinterpret_cast<uint8_t*>(handleRecordAddr);
    int32_t handleRecordTag = CSF_AT(int32_t, handleRecord, 12);
    uint32_t handle = handleRecordAddr + 4u;
    if (handleRecordTag < 0) {
        return handle;
    }

    int resolvedHandle = CHandleTable_Resolve(g_pCurrentScene, nullptr, static_cast<int>(handle));
    if (resolvedHandle == 0) {
        return 0u;
    }
    uint8_t* resolvedObject = reinterpret_cast<uint8_t*>(resolvedHandle);

    uint32_t lookupResult = static_cast<uint32_t>(QueryObjectStateFlag(CSF_AT(uint32_t, resolvedObject, 80)));
    if (static_cast<uint8_t>(lookupResult) == 0) {
        return lookupResult;
    }

    // v9 = (*services->subsystem->vtbl[3])(commandRefs[1]->[4]->[4])
    uint8_t* services = reinterpret_cast<uint8_t*>(GetValue8B0554());
    void* subsystem = CSF_AT(void*, services, 8128);
    uint8_t* secondRecord = reinterpret_cast<uint8_t*>(commandRefs[1]);
    uint8_t* thirdRecord = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, secondRecord, 4));
    uint32_t actionArg = CSF_AT(uint32_t, thirdRecord, 4);
    int actionIndex = ((int(__fastcall*)(void*, void*, uint32_t))(CSF_AT(void**, subsystem, 0)[3]))(
        subsystem, nullptr, actionArg);
    if (actionIndex == -1) {
        return static_cast<uint32_t>(actionIndex);
    }

    // Branch A is taken when the object's flag at 80 is set, or when the
    // display-state query of the action object (via vtbl[2]) returns 0.
    bool useBodyA = CSF_AT(uint32_t, resolvedObject, 80) != 0u;
    if (!useBodyA) {
        uint8_t* servicesAgain = reinterpret_cast<uint8_t*>(GetValue8B0554());
        void* subsystemAgain = CSF_AT(void*, servicesAgain, 8128);
        void* actionObject = ((void*(__fastcall*)(void*, void*, int))(CSF_AT(void**, subsystemAgain, 0)[2]))(
            subsystemAgain, nullptr, actionIndex);
        int actionObjectFlag = QueryDisplayStateFlag(CSF_AT(int, actionObject, 116));
        useBodyA = static_cast<uint8_t>(actionObjectFlag) == 0u;
    }

    if (useBodyA) {
        void* fsm = CSF_AT(void*, resolvedObject, 68);
        uint32_t fsmSlotCount = ((uint32_t(__fastcall*)(void*, void*))(CSF_AT(void**, fsm, 0)[40]))(fsm, nullptr);
        if (fsmSlotCount <= 1u) {
            return fsmSlotCount;
        }

        uint32_t selectedSlot = ((uint32_t(__fastcall*)(void*, void*, int))(CSF_AT(void**, fsm, 0)[48]))(
            fsm, nullptr, actionIndex);
        if (selectedSlot == 0xFFFFFFFFu) {
            return selectedSlot;
        }

        uint32_t currentSlot = ((uint32_t(__fastcall*)(void*, void*))(CSF_AT(void**, fsm, 0)[63]))(fsm, nullptr);
        if (selectedSlot == currentSlot) {
            ((void(__fastcall*)(void*, void*, int))(CSF_AT(void**, fsm, 0)[62]))(
                fsm, nullptr, selectedSlot == 0u ? 1 : 0);
            ((void(__fastcall*)(void*, void*, int, int, int, int))(CSF_AT(void**, resolvedObject, 0)[35]))(
                resolvedObject, nullptr, 0, 0, -1, 0);
        }

        // The original reloads the FSM pointer from offset 68 here.
        void* fsmReloaded = CSF_AT(void*, resolvedObject, 68);
        return ((uint32_t(__fastcall*)(void*, void*, uint32_t))(CSF_AT(void**, fsmReloaded, 0)[45]))(
            fsmReloaded, nullptr, selectedSlot);
    }

    // Branch B: CCharActionFsm path.
    void* actionFsm = CSF_AT(void*, resolvedObject, 68);
    uint32_t actionSlot = static_cast<uint32_t>(CharActionFsm_ResolveIndex(actionFsm, nullptr, actionIndex));
    if (actionSlot == 0xFFFFFFFFu) {
        return actionSlot;
    }

    if (actionSlot == CharActionFsm_GetMaskId(actionFsm, nullptr) &&
        static_cast<uint32_t>(GetPointerRangeElementCount(actionFsm, nullptr)) > 1u) {
        CharActionFsm_SetFlag(actionFsm, nullptr, actionSlot == 0u ? 1u : 0u);
        ((void(__fastcall*)(void*, void*, int, int, int, int))(CSF_AT(void**, resolvedObject, 0)[35]))(
            resolvedObject, nullptr, 0, 0, -1, 0);
    }
    return CharActionFsm_ApplyIndex(actionFsm, nullptr, static_cast<int>(actionSlot));
}

HOOK(0x00460940, ApplyCharActionFromHandleRef, "auto");
