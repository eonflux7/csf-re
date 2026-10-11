#include <cstdint>
#include "globals.h"
#include <cstddef>

#include "hook.h"

// Task record built on the stack and handed to the sub-object's vtable slot 22.
// Layout (offsets from the buffer start, matching the original frame):
//   +0x00 0, +0x04 3 (type tag), +0x08 vtable C_0082d448, +0x0C -1, +0x10 -1,
//   +0x14 a2[0], +0x18 a2[1], +0x1C a2[2], +0x20 1, +0x21 0, +0x22 1
static int __fastcall AiAttackTasks_SubmitAttackRequest(void* self, void* /*edx*/, uint32_t* requestArgs)
{
    // Sub-object pointer stored at self+0x14; the virtual call goes to (sub-object + 4).
    void* ownerObject = CSF_AT(void*, self, 0x14);
    uint8_t* subObject = (uint8_t*)ownerObject + 4;

    alignas(8) uint8_t taskBuffer[0x24];

    CSF_AT(uint32_t, taskBuffer, 0x00) = 0;
    CSF_AT(uint8_t, taskBuffer, 0x21) = 0;
    CSF_AT(uint32_t, taskBuffer, 0x14) = requestArgs[0];
    CSF_AT(uint32_t, taskBuffer, 0x18) = requestArgs[1];
    CSF_AT(uint32_t, taskBuffer, 0x1C) = requestArgs[2];
    CSF_AT(uint32_t, taskBuffer, 0x0C) = 0xFFFFFFFFu;
    CSF_AT(uint32_t, taskBuffer, 0x10) = 0xFFFFFFFFu;
    CSF_AT(void*, taskBuffer, 0x08) = CSF_VTBL(0x0082D448);
    CSF_AT(uint8_t, taskBuffer, 0x22) = 1;
    CSF_AT(uint32_t, taskBuffer, 0x04) = 3;
    CSF_AT(uint8_t, taskBuffer, 0x20) = 1;

    // Virtual call: slot 0x58 / 4 = 22 on the sub-object's vtable, __thiscall with one argument.
    return ((int(__fastcall*)(void*, void*, void*))CSF_AT(void**, subObject, 0)[0x58 / 4])(
        subObject, nullptr, taskBuffer);
}

HOOK(0x0047CC40, AiAttackTasks_SubmitAttackRequest, "auto");
