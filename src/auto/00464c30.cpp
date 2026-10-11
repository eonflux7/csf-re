#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __fastcall, GuiScene_FindControlById, 0x00452A30, (void* self, void* edx, unsigned int a2));
CSF_FUNC(void, __fastcall, StateRecord_Prepare, 0x00471E00, (uint32_t* a1));
CSF_FUNC(uint8_t*, __fastcall, PointBuffer_Reserve, 0x004725C0, (void* self, void* edx, int a2));
CSF_FUNC(unsigned int, __fastcall, StateRecord_SetPointFromBuffer, 0x00473490, (void* self, void* edx, unsigned int a2, int a3));
CSF_FUNC(char*, __fastcall, Object_PrepareForDestruction, 0x004734D0, (char** self, void* edx));
CSF_FUNC(unsigned int, __fastcall, StateRecord_SetPointCount, 0x00473D60, (void* self, void* edx, unsigned int a2));
CSF_FUNC(int, __fastcall, GuiControl_GetPathPointRecord, 0x004A0F30, (void* self, void* edx, int a2));

static char __fastcall StateRecord_SyncPathFromControl(
    void* self, void* /*edx*/, [[maybe_unused]] int a2,
    [[maybe_unused]] int a3, C_0085e7b8* a4)
{
    const uint32_t controlId =
        CSF_AT(uint32_t, CSF_AT(void**, a4, 0)[1], 0x20);
    if (controlId == 0xFFFFFFFFu) {
        return static_cast<char>(reinterpret_cast<std::uintptr_t>(a4));
    }

    uint32_t recordIndex =
        CSF_AT(uint32_t, reinterpret_cast<void*>(a4->field_0004), 0x08);
    void* ownerObject = CSF_AT(void*, self, 0x20);
    if ((recordIndex & 0x800000u) != 0) {
        ownerObject = CSF_AT(void*, CSF_AT(void*, g_pCurrentScene, 0xBC), 0x38);
        recordIndex &= ~0x800000u;
    }

    S_0069b1c0* stateRecord =
        CSF_AT(S_0069b1c0*, CSF_AT(void*, ownerObject, 8), 12 * recordIndex + 4);
    const uint8_t recordState = stateRecord->field_0000;
    if (recordState != 1 && recordState != 3) {
        return static_cast<char>(recordState);
    }

    StateRecord_Prepare(reinterpret_cast<uint32_t*>(stateRecord));

    void* controlObject = reinterpret_cast<void*>(
        GuiScene_FindControlById(g_pCurrentScene, nullptr, controlId));
    if (controlObject == nullptr) {
        return 0;
    }

    const uint32_t pointCount = CSF_AT(uint32_t, controlObject, 0xC);
    const unsigned int setCountResult =
        StateRecord_SetPointCount(stateRecord, nullptr, pointCount);
    if (pointCount == 0) {
        return static_cast<char>(setCountResult);
    }

    // sized by the original's frame (0x10 bytes): struct sizes in auto_classes.h are lower bounds
    alignas(8) uint8_t pointBufferBytes[0x10];
    C_008313f8& pointBuffer = *reinterpret_cast<C_008313f8*>(pointBufferBytes);
    pointBuffer.vtbl = CSF_VTBL(0x008313F8);
    pointBuffer.field_0004 = 0;
    pointBuffer.field_0008 = static_cast<uint32_t>(-1);
    PointBuffer_Reserve(&pointBuffer, nullptr, 4);

    for (uint32_t pointIndex = 0; pointIndex < pointCount; ++pointIndex) {
        uint8_t* bufferBase =
            reinterpret_cast<uint8_t*>(pointBuffer.field_0004);
        void* pointRecord = reinterpret_cast<void*>(
            GuiControl_GetPathPointRecord(
                controlObject, nullptr, static_cast<int>(pointIndex)));

        CSF_AT(uint32_t, bufferBase, 8) = CSF_AT(uint32_t, pointRecord, 4);
        CSF_AT(uint32_t, bufferBase, 12) = CSF_AT(uint32_t, pointRecord, 8);

        StateRecord_SetPointFromBuffer(
            stateRecord, nullptr, pointIndex,
            static_cast<int>(reinterpret_cast<std::intptr_t>(&pointBuffer)));
    }

    char* destructionResult = Object_PrepareForDestruction(
        reinterpret_cast<char**>(&pointBuffer), nullptr);
    return static_cast<char>(reinterpret_cast<std::uintptr_t>(destructionResult));
}

HOOK(0x00464C30, StateRecord_SyncPathFromControl, "auto");
