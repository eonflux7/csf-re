#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, PairLink_PrepareForRebind, 0x004E55A0, (void* self, void* edx));
CSF_FUNC(char, __fastcall, DecrementByteCounter, 0x004E55B0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CharacterDefs_AssignEmbeddedMember, 0x006CAF70, (void* self, void* edx, int a2));

// Selects one of four embedded member slots of the object by index (0..3),
// clears the uint32 at +8 of the caller-supplied output block, and reports
// whether a member was selected.
static bool __fastcall C_0083e9d0_CheckInnerBlockFlagLevel(C_0083e9d0* self, void* /*edx*/, int memberIndex, void* outBlock)
{
    PairLink_PrepareForRebind(self, nullptr);

    void* memberSlot = nullptr;
    CSF_AT(uint32_t, outBlock, 8) = 0;

    switch (memberIndex)
    {
    case 0:
        memberSlot = reinterpret_cast<uint8_t*>(self) + 0x80;
        break;
    case 1:
        memberSlot = reinterpret_cast<uint8_t*>(self) + 0x70;
        break;
    case 2:
        memberSlot = reinterpret_cast<uint8_t*>(self) + 0x60;
        break;
    case 3:
        memberSlot = reinterpret_cast<uint8_t*>(self) + 0x50;
        break;
    default:
        break;
    }

    bool hasMember = memberSlot != nullptr;
    if (hasMember)
        CharacterDefs_AssignEmbeddedMember(nullptr, nullptr, reinterpret_cast<int>(memberSlot));

    DecrementByteCounter(self, nullptr);
    return hasMember;
}

HOOK(0x004E72C0, C_0083e9d0_CheckInnerBlockFlagLevel, "auto");
