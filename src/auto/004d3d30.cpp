#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, C_0082d844_SetDirectionalValue, 0x004093F0, (C_0082d844* self, void* , float a2, char a3));
CSF_FUNC(char, __fastcall, CharActionFsm_SetControllerPosition, 0x004096D0, (void* self, void* , float* position));
CSF_FUNC(int, __cdecl, GetGlobalObjectField184, 0x00475470, ());
CSF_FUNC(uint32_t, __fastcall, DispatchRecordToGlobalObject, 0x004758B0, (void* self, void* edx, void* record, char flag));

static int __fastcall CharActionFsm_ResetControllerDirectionAndNotify(
    void* self, void* /*edx*/)
{
    void* provider = CSF_AT(void*, CSF_AT(void*, self, 0x14), 0x4C);
    C_0082d844* controller =
        ((C_0082d844* (__fastcall*)(void*, void*))
            CSF_AT(void**, provider, 0)[0x3C / 4])(provider, nullptr);

    if ((CSF_AT(uint8_t, self, 0x44) & 0x20) != 0)
    {
        CharActionFsm_SetControllerPosition(
            controller, nullptr,
            reinterpret_cast<float*>(static_cast<uint8_t*>(self) + 0x58));
    }

    C_0082d844_SetDirectionalValue(controller, nullptr, 0.0f, 0);

    uint32_t flags = CSF_AT(uint32_t, self, 0x44);
    int result = CSF_AT(int, self, 0x6C);
    CSF_AT(uint32_t, self, 0x44) = (flags & -131) | 0x80;

    if (result >= 0)
    {
        alignas(8) uint8_t recordBytes[0x34];

        CSF_AT(int, recordBytes, 0x0C) = -1;
        CSF_AT(int, recordBytes, 0x10) = -1;
        CSF_AT(int, recordBytes, 0x18) = -1;
        CSF_AT(int, recordBytes, 0x1C) = -1;

        uint8_t* ownerRecord =
            static_cast<uint8_t*>(CSF_AT(void*, self, 0x14)) + 0x1C;

        CSF_AT(void**, recordBytes, 0x08) = CSF_VTBL(0x0082D448);
        CSF_AT(void**, recordBytes, 0x14) = CSF_VTBL(0x0082D448);
        int firstValue = CSF_AT(int, self, 0x68);

        CSF_AT(void**, recordBytes, 0x00) = CSF_VTBL(0x0082D478);
        CSF_AT(int, recordBytes, 0x04) = 38;
        CSF_AT(int, recordBytes, 0x20) = 0;
        CSF_AT(int, recordBytes, 0x24) = 0;
        CSF_AT(int, recordBytes, 0x2C) = 0;
        CSF_AT(int, recordBytes, 0x28) = 0;
        CSF_AT(uint8_t, recordBytes, 0x30) = 0;

        CSF_AT(int, recordBytes, 0x0C) = CSF_AT(int, ownerRecord, 0x04);
        int ownerValue = CSF_AT(int, ownerRecord, 0x08);
        int secondValue = CSF_AT(int, self, 0x6C);
        CSF_AT(int, recordBytes, 0x10) = ownerValue;
        CSF_AT(int, recordBytes, 0x18) = firstValue;
        CSF_AT(int, recordBytes, 0x1C) = secondValue;

        void* dispatcher = reinterpret_cast<void*>(GetGlobalObjectField184());
        result = static_cast<int>(
            DispatchRecordToGlobalObject(dispatcher, nullptr, recordBytes, 0));
    }

    void* pendingState = CSF_AT(void*, self, 0x54);
    if (pendingState)
        CSF_AT(uint32_t, pendingState, 0x04) |= 1u;

    return result;
}

HOOK(0x004D3D30, CharActionFsm_ResetControllerDirectionAndNotify, "auto");
