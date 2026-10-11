#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, AiCore_ConstructBase, 0x004A9C20, (void* self, void* edx, int a2));
CSF_FUNC(uint32_t*, __fastcall, AiCore_ConstructStateData, 0x004A9D80, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, AiCore_ConstructTrackingTask, 0x004AE340, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, AiCore_ConstructSearchTask, 0x004AE3D0, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, AiCore_ConstructAttackFromPositionTask, 0x004AE430, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, AiCore_ConstructMoveToCoverTask, 0x004AE510, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, AiCore_ConstructWatchFromCoverTask, 0x004AE580, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, AiCore_ConstructAttackFromCoverTask, 0x004AE600, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, AiCore_ConstructDistractedState, 0x004B2530, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, AiCore_ConstructSuspiciousState, 0x004B25A0, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, AiCore_ConstructAlertState, 0x004B2600, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, AiCore_ConstructInteractiveTask, 0x004B2990, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, AiCore_ConstructCombatState, 0x004B93E0, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, AiCore_ConstructMacroOrderTask, 0x004B94C0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, AiCore_ConstructTaskController, 0x004BD6C0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, AiCore_InitializeTaskController, 0x004BD7A0, (void* self, void* edx, int a2));
CSF_FUNC(void*, __fastcall, C_004d1610_ClearHeader, 0x004D1610, (void* self, void*));
CSF_FUNC(TAiState*, __fastcall, TAiState_Constructor, 0x004DBF70, (TAiState* self, void*));

static int __fastcall C_0083aa80_Construct(C_0083aa80* self, void* /*edx*/,
                                         int owner, int configuration)
{
    auto* bytes = reinterpret_cast<uint8_t*>(self);

    AiCore_ConstructBase(self, nullptr, owner);
    self->vtbl = CSF_VTBL(0x0083AA80);
    self->field_0004 = reinterpret_cast<uint32_t>(CSF_VTBL(0x0083A9F8));

    TAiState_Constructor(reinterpret_cast<TAiState*>(bytes + 0x30), nullptr);
    CSF_AT(void*, self, 0x30) = CSF_VTBL(0x00835448);
    TAiState_Constructor(reinterpret_cast<TAiState*>(bytes + 0x34), nullptr);
    CSF_AT(void*, self, 0x34) = CSF_VTBL(0x008354D0);
    TAiState_Constructor(reinterpret_cast<TAiState*>(bytes + 0x38), nullptr);
    CSF_AT(void*, self, 0x38) = CSF_VTBL(0x00835558);
    AiCore_ConstructDistractedState(bytes + 0x3C, nullptr);
    AiCore_ConstructSuspiciousState(bytes + 0x50, nullptr);
    AiCore_ConstructAlertState(bytes + 0x84, nullptr);
    AiCore_ConstructCombatState(bytes + 0x10C, nullptr);

    TAiState_Constructor(reinterpret_cast<TAiState*>(bytes + 0x19C), nullptr);
    CSF_AT(void*, self, 0x19C) = CSF_VTBL(0x008370B8);
    TAiState_Constructor(reinterpret_cast<TAiState*>(bytes + 0x1A0), nullptr);
    CSF_AT(void*, self, 0x1A0) = CSF_VTBL(0x00837148);
    TAiState_Constructor(reinterpret_cast<TAiState*>(bytes + 0x1A4), nullptr);
    CSF_AT(void*, self, 0x1A4) = CSF_VTBL(0x008371D0);
    TAiState_Constructor(reinterpret_cast<TAiState*>(bytes + 0x1AC), nullptr);
    CSF_AT(void*, self, 0x1AC) = CSF_VTBL(0x00837268);
    TAiState_Constructor(reinterpret_cast<TAiState*>(bytes + 0x1B0), nullptr);
    CSF_AT(void*, self, 0x1B0) = CSF_VTBL(0x008372F0);
    CSF_AT(uint32_t, self, 0x1B4) = 0;
    CSF_AT(uint32_t, self, 0x1B8) = 0;
    CSF_AT(uint32_t, self, 0x1BC) = 0;
    AiCore_ConstructTrackingTask(bytes + 0x1C8, nullptr);
    AiCore_ConstructSearchTask(bytes + 0x21C, nullptr);
    AiCore_ConstructAttackFromPositionTask(bytes + 0x23C, nullptr);

    TAiState_Constructor(reinterpret_cast<TAiState*>(bytes + 0x258), nullptr);
    CSF_AT(uint32_t, self, 0x25C) = 0;
    CSF_AT(uint32_t, self, 0x260) = 0;
    CSF_AT(void*, self, 0x258) = CSF_VTBL(0x0083A480);
    CSF_AT(uint8_t, self, 0x26C) = 0;
    CSF_AT(uint8_t, self, 0x26D) = 1;
    CSF_AT(uint32_t, self, 0x270) = 0;
    CSF_AT(uint32_t, self, 0x274) = 0;
    CSF_AT(uint32_t, self, 0x278) = 0;
    self->field_0280 = 0;
    CSF_AT(uint32_t, self, 0x284) = 0;
    CSF_AT(uint32_t, self, 0x288) = 0;
    CSF_AT(uint32_t, self, 0x28C) = 10;
    AiCore_ConstructStateData(bytes + 0x290, nullptr);

    TAiState_Constructor(reinterpret_cast<TAiState*>(bytes + 0x2A4), nullptr);
    CSF_AT(uint32_t, self, 0x2A8) = 0;
    CSF_AT(uint32_t, self, 0x2AC) = 0;
    CSF_AT(void*, self, 0x2A4) = CSF_VTBL(0x008399B0);
    CSF_AT(uint32_t, self, 0x2B8) = 0;
    CSF_AT(uint32_t, self, 0x2BC) = 0;
    CSF_AT(uint32_t, self, 0x2C0) = 0;
    self->field_02c4 = 0;
    CSF_AT(uint32_t, self, 0x2C8) = 0;
    CSF_AT(uint32_t, self, 0x2CC) = 0;
    CSF_AT(uint32_t, self, 0x2D0) = 10;

    TAiState_Constructor(reinterpret_cast<TAiState*>(bytes + 0x2D4), nullptr);
    CSF_AT(uint32_t, self, 0x2D8) = 0;
    CSF_AT(uint32_t, self, 0x2DC) = 0;
    CSF_AT(void*, self, 0x2D4) = CSF_VTBL(0x0083A520);
    CSF_AT(void*, self, 0x2E8) = CSF_VTBL(0x0082D448);
    CSF_AT(int32_t, self, 0x2EC) = -1;
    CSF_AT(int32_t, self, 0x2F0) = -1;

    TAiState_Constructor(reinterpret_cast<TAiState*>(bytes + 0x2F4), nullptr);
    CSF_AT(uint32_t, self, 0x2F8) = 0;
    CSF_AT(uint32_t, self, 0x2FC) = 0;
    CSF_AT(void*, self, 0x2F4) = CSF_VTBL(0x00839A50);
    TAiState_Constructor(reinterpret_cast<TAiState*>(bytes + 0x310), nullptr);
    CSF_AT(uint32_t, self, 0x314) = 0;
    CSF_AT(uint32_t, self, 0x318) = 0;
    CSF_AT(void*, self, 0x310) = CSF_VTBL(0x00839AF0);
    TAiState_Constructor(reinterpret_cast<TAiState*>(bytes + 0x32C), nullptr);
    CSF_AT(uint32_t, self, 0x330) = 0;
    CSF_AT(uint32_t, self, 0x334) = 0;
    CSF_AT(void*, self, 0x32C) = CSF_VTBL(0x00839B90);
    self->field_0340 = 0;
    CSF_AT(uint32_t, self, 0x344) = 0;
    CSF_AT(uint32_t, self, 0x348) = 0;
    CSF_AT(uint32_t, self, 0x34C) = 10;
    CSF_AT(uint32_t, self, 0x358) = 0;
    CSF_AT(uint32_t, self, 0x35C) = 0;
    CSF_AT(uint32_t, self, 0x360) = 0;

    TAiState_Constructor(reinterpret_cast<TAiState*>(bytes + 0x364), nullptr);
    CSF_AT(uint32_t, self, 0x368) = 0;
    CSF_AT(uint32_t, self, 0x36C) = 0;
    CSF_AT(void*, self, 0x364) = CSF_VTBL(0x00839C30);
    TAiState_Constructor(reinterpret_cast<TAiState*>(bytes + 0x388), nullptr);
    CSF_AT(uint32_t, self, 0x38C) = 0;
    CSF_AT(uint32_t, self, 0x390) = 0;
    CSF_AT(void*, self, 0x388) = CSF_VTBL(0x0083A5C0);
    CSF_AT(void*, self, 0x39C) = CSF_VTBL(0x0082D448);
    CSF_AT(int32_t, self, 0x3A0) = -1;
    CSF_AT(int32_t, self, 0x3A4) = -1;
    CSF_AT(uint32_t, self, 0x3A8) = 0;
    CSF_AT(uint32_t, self, 0x3AC) = 0;
    CSF_AT(uint32_t, self, 0x3B0) = 0;

    TAiState_Constructor(reinterpret_cast<TAiState*>(bytes + 0x3B4), nullptr);
    CSF_AT(uint32_t, self, 0x3B8) = 0;
    CSF_AT(uint32_t, self, 0x3BC) = 0;
    CSF_AT(void*, self, 0x3B4) = CSF_VTBL(0x00839CD0);
    CSF_AT(uint32_t, self, 0x3C8) = 0;
    CSF_AT(uint32_t, self, 0x3CC) = 0;
    CSF_AT(uint32_t, self, 0x3D0) = 0;
    CSF_AT(uint32_t, self, 0x3D4) = 0;
    AiCore_ConstructMoveToCoverTask(bytes + 0x3D8, nullptr);
    AiCore_ConstructWatchFromCoverTask(bytes + 0x404, nullptr);
    AiCore_ConstructAttackFromCoverTask(bytes + 0x418, nullptr);
    AiCore_ConstructMacroOrderTask(bytes + 0x440, nullptr);

    TAiState_Constructor(reinterpret_cast<TAiState*>(bytes + 0x53C), nullptr);
    CSF_AT(void*, self, 0x53C) = CSF_VTBL(0x0083A6F0);
    TAiState_Constructor(reinterpret_cast<TAiState*>(bytes + 0x540), nullptr);
    CSF_AT(void*, self, 0x540) = CSF_VTBL(0x00839FD0);
    CSF_AT(uint32_t, self, 0x544) = 0;
    CSF_AT(uint32_t, self, 0x548) = 0;
    CSF_AT(uint32_t, self, 0x54C) = 0;
    CSF_AT(uint32_t, self, 0x550) = 10;

    TAiState_Constructor(reinterpret_cast<TAiState*>(bytes + 0x55C), nullptr);
    CSF_AT(void*, self, 0x55C) = CSF_VTBL(0x00838968);
    TAiState_Constructor(reinterpret_cast<TAiState*>(bytes + 0x560), nullptr);
    CSF_AT(void*, self, 0x560) = CSF_VTBL(0x0083A058);
    TAiState_Constructor(reinterpret_cast<TAiState*>(bytes + 0x564), nullptr);
    CSF_AT(void*, self, 0x564) = CSF_VTBL(0x008389F0);
    TAiState_Constructor(reinterpret_cast<TAiState*>(bytes + 0x568), nullptr);
    CSF_AT(void*, self, 0x568) = CSF_VTBL(0x00838A78);
    TAiState_Constructor(reinterpret_cast<TAiState*>(bytes + 0x56C), nullptr);
    CSF_AT(void*, self, 0x56C) = CSF_VTBL(0x00838B00);
    TAiState_Constructor(reinterpret_cast<TAiState*>(bytes + 0x570), nullptr);
    CSF_AT(void*, self, 0x570) = CSF_VTBL(0x00838B88);
    AiCore_ConstructInteractiveTask(bytes + 0x574, nullptr);

    TAiState_Constructor(reinterpret_cast<TAiState*>(bytes + 0x5A0), nullptr);
    CSF_AT(void*, self, 0x5A0) = CSF_VTBL(0x0083A820);
    TAiState_Constructor(reinterpret_cast<TAiState*>(bytes + 0x5A4), nullptr);
    CSF_AT(void*, self, 0x5A4) = CSF_VTBL(0x0083A0F8);
    CSF_AT(uint32_t, self, 0x5A8) = 0;
    CSF_AT(uint32_t, self, 0x5AC) = 0;
    CSF_AT(uint32_t, self, 0x5B0) = 0;
    CSF_AT(uint32_t, self, 0x5B4) = 10;

    TAiState_Constructor(reinterpret_cast<TAiState*>(bytes + 0x5C0), nullptr);
    CSF_AT(void*, self, 0x5C0) = CSF_VTBL(0x00838E58);
    TAiState_Constructor(reinterpret_cast<TAiState*>(bytes + 0x5C4), nullptr);
    CSF_AT(void*, self, 0x5C4) = CSF_VTBL(0x0083A180);
    TAiState_Constructor(reinterpret_cast<TAiState*>(bytes + 0x5C8), nullptr);
    CSF_AT(void*, self, 0x5C8) = CSF_VTBL(0x00838EE0);
    TAiState_Constructor(reinterpret_cast<TAiState*>(bytes + 0x5CC), nullptr);
    CSF_AT(void*, self, 0x5CC) = CSF_VTBL(0x00838F68);
    CSF_AT(uint32_t, self, 0x5D0) = 0;
    CSF_AT(uint32_t, self, 0x5D4) = 0;
    CSF_AT(uint32_t, self, 0x5D8) = 0;
    AiCore_ConstructTaskController(bytes + 0x5DC, nullptr);

    C_004d1610_ClearHeader(self->pad_0690, nullptr);
    self->field_06b4 = 0;
    self->field_06b8 = 0;
    self->field_06bc = 0;
    self->field_06c0 = -1;
    self->field_06c4 = -1;
    CSF_AT(void*, self, 0x6D0) = CSF_VTBL(0x0082D448);
    self->field_06d4 = -1;
    self->field_06d8 = -1;
    self->field_070c = 15;
    self->field_0708 = 0;
    *reinterpret_cast<uint8_t*>(&self->field_06f8) = 0;
    self->field_0718 = configuration;
    AiCore_InitializeTaskController(bytes + 0x5DC, nullptr, owner);

    self->field_06ec = reinterpret_cast<uint32_t>(CSF_STR(0x0083A9F4, "?"));
    self->field_06f0 = reinterpret_cast<uint32_t>(CSF_STR(0x0083A9F4, "?"));
    self->field_06dc = 0;
    self->field_06a0 = 0;
    self->field_06a4 = 0;
    self->field_06c8 = 0;
    self->field_0710 = 0;
    self->field_0714 = 0;
    self->field_06e4 = 0;
    self->field_06e8 = 0;
    self->field_06e0 = 0;

    const uint32_t initialValue =
        CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0008), 0x2C);
    self->field_068c = initialValue;
    self->field_06a8 = initialValue;
    self->field_06ac = initialValue;
    return reinterpret_cast<int>(self);
}

HOOK(0x004BDDD0, C_0083aa80_Construct, "auto");
