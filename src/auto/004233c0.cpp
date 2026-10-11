#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(unsigned int, __fastcall, DispatchWithArgBlock, 0x004DD060, (void* self, void* edx, uint32_t* a2));

static unsigned int __fastcall DispatchCurrentTaskState(uint32_t* self, void* /*edx*/)
{
    // v1 = self->field_0008 (owner object)
    uint8_t* owner = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, self, 0x08));

    // Build the 0x1C-byte argument block passed to DispatchWithArgBlock.
    uint32_t argBlock[7] = {};
    uint8_t* ownerInfo = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, owner, 0x14));
    argBlock[0] = 0;
    argBlock[1] = reinterpret_cast<uint32_t>(CSF_VTBL(0x0082D448));
    argBlock[2] = CSF_AT(uint32_t, ownerInfo, 0x20);
    argBlock[3] = CSF_AT(uint32_t, ownerInfo, 0x24);
    argBlock[4] = CSF_AT(uint32_t, owner, 0x224);
    argBlock[5] = CSF_AT(uint32_t, owner, 0x228);
    argBlock[6] = CSF_AT(uint32_t, owner, 0x22C);

    int valueObj = GetValue8B0554();

    // Dispatcher lives at valueObj->field_1FD8 + 0x34.
    uint8_t* managerRaw = reinterpret_cast<uint8_t*>(valueObj);
    uint8_t* dispatcher = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, managerRaw, 0x1FD8)) + 0x34;

    return DispatchWithArgBlock(dispatcher, nullptr, argBlock);
}

HOOK(0x004233C0, DispatchCurrentTaskState, "auto");
