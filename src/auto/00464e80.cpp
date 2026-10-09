#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(uint8_t*, __fastcall, EventBuffer_Begin, 0x004725C0, (void* self, void* edx, int a2));
CSF_FUNC(unsigned int, __fastcall, EventBuffer_AppendIndexed, 0x00473490, (void* self, void* edx, unsigned int a2, int a3));
CSF_FUNC(char*, __fastcall, Object_PrepareForDestruction, 0x004734D0, (char** self, void* edx));
CSF_FUNC(unsigned int, __fastcall, Collection_SetCapacity, 0x00473D60, (void* self, void* edx, unsigned int a2));
CSF_FUNC(int, __fastcall, Entry_GetPayloadPointer, 0x00476060, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CharFsm_GetEntryAt, 0x00485940, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, IsObjectTypeEnabled, 0x00755810, (unsigned int a1));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static char* __fastcall BuildFsmActionBufferForHandle(void* self, void* /*edx*/, int /*a2*/, int /*a3*/, uint32_t* requestArgs)
{
    const uint32_t requestRecord = *requestArgs;
    const uint32_t recordPtr = *reinterpret_cast<uint32_t*>(requestRecord + 4);
    const int recordKind = *reinterpret_cast<int*>(recordPtr + 12);
    char* result = reinterpret_cast<char*>(recordPtr + 4);
    if (recordKind < 0) {
        return result;
    }

    result = reinterpret_cast<char*>(CHandleTable_Resolve(g_pCurrentScene, nullptr, static_cast<int>(recordPtr + 4)));
    char* ownerObject = result;
    if (ownerObject == nullptr) {
        return result;
    }

    int objectTypeFlags = IsObjectTypeEnabled(CSF_AT(uint32_t, ownerObject, 0x50));
    if (static_cast<uint8_t>(objectTypeFlags) == 0) {
        return reinterpret_cast<char*>(objectTypeFlags);
    }

    CCharActionFsm* fsm = CSF_AT(CCharActionFsm*, ownerObject, 0x44);
    uint32_t actionCount = fsm->vSlotA0();
    uint32_t vectorEntryCount = 0;
    if (CSF_AT(uint32_t, ownerObject, 0x50) == 0) {
        uint32_t vectorBegin = fsm->field_0078;
        if (vectorBegin != 0) {
            vectorEntryCount = static_cast<uint32_t>(static_cast<int>(fsm->field_007c - vectorBegin) >> 2);
        }
    }
    if (actionCount == 0 && vectorEntryCount == 0) {
        return nullptr;
    }

    uint32_t tableKey = *reinterpret_cast<uint32_t*>(requestArgs[1] + 8);
    uint32_t owningRoot = CSF_AT(uint32_t, self, 0x20);
    if ((tableKey & 0x800000u) != 0) {
        owningRoot = CSF_AT(uint32_t, reinterpret_cast<void*>(CSF_AT(uint32_t, g_pCurrentScene, 0xBC)), 0x38);
        tableKey &= ~0x800000u;
    }
    uint32_t lookupTableBase = *reinterpret_cast<uint32_t*>(owningRoot + 8);
    S_0069b1c0* targetCollection = *reinterpret_cast<S_0069b1c0**>(lookupTableBase + 12 * tableKey + 4);
    Collection_SetCapacity(targetCollection, nullptr, actionCount + vectorEntryCount);

    C_008313f8 builder;
    builder.vtbl = CSF_VTBL(0x008313F8);
    builder.field_0004 = 0;
    builder.field_0008 = static_cast<uint32_t>(-1);
    EventBuffer_Begin(&builder, nullptr, 47);

    if (actionCount != 0) {
        uint32_t actionIndex = 0;
        do {
            C_00862490* actionEntry = reinterpret_cast<C_00862490*>(fsm->vSlotCC(actionIndex));
            uint32_t bufferHandle = builder.field_0004;
            uint32_t payloadWord = *reinterpret_cast<uint32_t*>(Entry_GetPayloadPointer(actionEntry, nullptr));
            *reinterpret_cast<uint32_t*>(bufferHandle + 4) = payloadWord;
            EventBuffer_AppendIndexed(targetCollection, nullptr, actionIndex, reinterpret_cast<int>(&builder));
            ++actionIndex;
        } while (actionIndex < actionCount);
    }

    if (vectorEntryCount != 0) {
        for (uint32_t i = 0; i < vectorEntryCount; ++i) {
            C_00862490* sourceEntry = reinterpret_cast<C_00862490*>(CharFsm_GetEntryAt(fsm, nullptr, static_cast<int>(i)));
            uint32_t bufferHandle = builder.field_0004;
            uint32_t payloadWord = *reinterpret_cast<uint32_t*>(Entry_GetPayloadPointer(sourceEntry, nullptr));
            *reinterpret_cast<uint32_t*>(bufferHandle + 4) = payloadWord;
            EventBuffer_AppendIndexed(targetCollection, nullptr, actionCount + i, reinterpret_cast<int>(&builder));
        }
    }

    return Object_PrepareForDestruction(reinterpret_cast<char**>(&builder), nullptr);
}

HOOK(0x00464E80, BuildFsmActionBufferForHandle, "auto");
