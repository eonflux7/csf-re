#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void* edx));
CSF_FUNC(bool, __fastcall, CoverTask_IsLinkedObjectReady, 0x004AF3D0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CoverAttack_SetState_4DC150, 0x004DC150, (void* self, void* edx, int a2));

static int __fastcall TEstatico_SetStateByLinkedReadiness(TEstatico* self, void* /*edx*/, TAiState* a2)
{
    int linkedObjectValue = GetLinkedObjectValue_4DB5C0(a2, nullptr);

    void* linkedObjectHeader = nullptr;
    if (linkedObjectValue)
        linkedObjectHeader = reinterpret_cast<void*>(linkedObjectValue - 4);

    if (CoverTask_IsLinkedObjectReady(linkedObjectHeader, nullptr))
    {
        int readyStateArg = reinterpret_cast<int>(reinterpret_cast<uint8_t*>(self) + 0x14);
        return CoverAttack_SetState_4DC150(a2, nullptr, readyStateArg);
    }

    int notReadyStateArg = reinterpret_cast<int>(reinterpret_cast<uint8_t*>(self) + 0x4);
    return CoverAttack_SetState_4DC150(a2, nullptr, notReadyStateArg);
}

HOOK(0x004AFE70, TEstatico_SetStateByLinkedReadiness, "auto");
