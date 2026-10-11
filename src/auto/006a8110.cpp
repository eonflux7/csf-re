#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, NetZ_OperatorNew, 0x00647140, (unsigned int size));
CSF_FUNC(void, __fastcall, NetGatheringDdl_ProcessGatheringList, 0x007A1240,
         (void* self, void* edx, int a2));
CSF_FUNC(uint32_t*, __fastcall, NetGatheringDdl_ConstructGatheringOperation, 0x007A18E0,
         (void* self, void* edx, int a2));

static int __fastcall S_006aaae0_UpdateGatheringOperation(
    S_006aaae0* self, void* /*edx*/, int /*elapsedTime*/)
{
    void* operation = CSF_AT(void*, self, 0x38);
    const uint32_t status = CSF_AT(uint32_t, operation, 0x0C);

    if (status == 1)
        return 0;

    if (status == 2)
    {
        if (operation)
        {
            ((int(__fastcall*)(void*, void*, int))
                CSF_AT(void**, operation, 0)[0])(operation, nullptr, 1);
            CSF_AT(void*, self, 0x38) = nullptr;
        }

        void* storage = NetZ_OperatorNew(16);
        uint32_t* nextOperation = nullptr;
        if (storage)
        {
            nextOperation = NetGatheringDdl_ConstructGatheringOperation(
                storage, nullptr, CSF_AT(int, self, 0x00));
        }

        self->field_0028 =
            static_cast<uint32_t>(reinterpret_cast<uintptr_t>(nextOperation));
        NetGatheringDdl_ProcessGatheringList(
            nextOperation, nullptr,
            static_cast<int>(reinterpret_cast<uintptr_t>(
                reinterpret_cast<uint8_t*>(self) + 8)));

        self->field_002c = 5.0f;
        self->field_0004 = 3;

        // The original leaves the void callee's incidental EAX here.
        return 0;
    }

    int result = static_cast<int>(status - 3);
    if (operation)
    {
        result = ((int(__fastcall*)(void*, void*, int))
            CSF_AT(void**, operation, 0)[0])(operation, nullptr, 1);
        CSF_AT(void*, self, 0x38) = nullptr;
    }
    self->field_0004 = 0;
    return result;
}

HOOK(0x006A8110, S_006aaae0_UpdateGatheringOperation, "auto");
