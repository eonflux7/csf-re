#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __fastcall, CScene_RemoveObjectSlot, 0x00453390, (CScene* self, void* , int a2));
CSF_FUNC(int, __cdecl, GetGlobalObjectField184, 0x00475470, ());
CSF_FUNC(uint32_t, __fastcall, DispatchRecordToGlobalObject, 0x004758B0, (void* self, void* edx, void* record, char flag));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());

static int __fastcall C_00833cf8_RemoveFromScene(C_00833cf8* self, void* /*edx*/)
{
    CScene_RemoveObjectSlot(
        static_cast<CScene*>(g_pCurrentScene), nullptr,
        reinterpret_cast<int>(&self->field_001c));

    int state = GetGlobalState_50D190();
    if (!CSF_AT(uint8_t, reinterpret_cast<void*>(state), 0xD0))
        return state;

    uint32_t record[13];
    record[2] = reinterpret_cast<uint32_t>(CSF_VTBL(0x0082D448));
    record[5] = reinterpret_cast<uint32_t>(CSF_VTBL(0x0082D448));
    record[6] = -1;
    record[7] = -1;
    uint32_t slotValue = CSF_AT(uint32_t, self, 0x20);
    record[8] = 0;
    record[9] = 0;
    record[11] = 0;
    record[10] = 0;
    record[12] = 0;
    uint32_t slotAuxiliary = CSF_AT(uint32_t, self, 0x24);
    record[0] = reinterpret_cast<uint32_t>(CSF_VTBL(0x0082D478));
    record[1] = 23;
    record[3] = slotValue;
    record[4] = slotAuxiliary;

    void* dispatcher = reinterpret_cast<void*>(GetGlobalObjectField184());
    return static_cast<int>(DispatchRecordToGlobalObject(dispatcher, nullptr, record, 1));
}

HOOK(0x0048FBF0, C_00833cf8_RemoveFromScene, "auto");
