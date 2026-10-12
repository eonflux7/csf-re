#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, RenderHandle_FromObject, 0x006B2D50, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GetRtDictSchema, 0x006D6360, (void* self, void*));
CSF_FUNC(uint32_t*, __cdecl, RpTeamDictSetFlags, 0x007FE6B0, (uint32_t* a1, uint16_t* a2, unsigned int a3, char a4));
CSF_FUNC(int, __cdecl, RpTeamPlayerSetActive, 0x007FEE10, (int a1, int a2, int a3));
CSF_FUNC(int, __cdecl, _rpTeamPlayerSelect, 0x007FEE60, (uint32_t* a1, int a2));

static int __fastcall C_008630e0_RefreshTeamPlayerSelection(C_008630e0* self, void* /*edx*/)
{
    int globalInt = GetGlobalInt();
    int renderHandle = RenderHandle_FromObject(reinterpret_cast<void*>(static_cast<uintptr_t>(globalInt)), nullptr);

    uint8_t flags = 0;
    if (CSF_AT(uint8_t, self, 0x0C) != 0)
        flags = 3;
    if (CSF_AT(uint8_t, self, 0x0D) != 0)
        flags |= 1;

    int dictSchema = GetRtDictSchema(reinterpret_cast<void*>(static_cast<uintptr_t>(renderHandle)), nullptr);
    RpTeamDictSetFlags(reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(dictSchema)),
                       reinterpret_cast<uint16_t*>(static_cast<uintptr_t>(self->field_0004)),
                       0xFFFFFFFFu,
                       static_cast<char>(flags));

    if (CSF_AT(uint8_t, self, 0x0C) != 0) {
        bool slotResult = self->vSlot2C();
        uint8_t notResult = slotResult ? 0 : 1;
        self->field_0012 = notResult;
        if (notResult == 0) {
            uint8_t* playerPtr = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(self->field_0004));
            _rpTeamPlayerSelect(reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(CSF_AT(uint32_t, playerPtr, 0x24))),
                                static_cast<int>(self->field_0004));
            playerPtr = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(self->field_0004));
            RpTeamPlayerSetActive(static_cast<int>(CSF_AT(uint32_t, playerPtr, 0x24)),
                                  static_cast<int>(self->field_0004),
                                  1);
        }
        uint8_t finalFlag = self->field_0012;
        self->field_0011 = 1;
        return finalFlag == 0;
    } else {
        self->field_0011 = 1;
        return 1;
    }
}

HOOK(0x6FF680, C_008630e0_RefreshTeamPlayerSelection, "auto");
