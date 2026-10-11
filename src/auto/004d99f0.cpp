#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_00831ac0_InitializeAttackTaskGroup, 0x0047F640, (C_00831ac0* self, void* , int parameter));
CSF_FUNC(int, __fastcall, Game_AppendArrayValue, 0x0048FEB0, (void* self, void* , int value));
CSF_FUNC(uint32_t*, __fastcall, C_00834fa8_Construct, 0x004A84E0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, C_0083aa80_Construct, 0x004BDDD0, (C_0083aa80* self, void* , int owner, int configuration));
CSF_FUNC(void*, __fastcall, C_004d1610_ClearHeader, 0x004D1610, (void* self, void*));
CSF_FUNC(uint32_t*, __fastcall, AiAutonomous_ConstructTaskBase, 0x004D1660, (void* self, void* edx, int a2));
CSF_FUNC(uint32_t*, __fastcall, AiAutonomous_ConstructAutonomousTask, 0x004D7870, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, AiAutonomous_ConstructThrowAtPositionTask, 0x004D7920, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, AiAutonomous_ConstructConversationTask, 0x004D98C0, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, CharTask_InitBase, 0x004DB4C0, (void* self, void* edx));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));

static int __fastcall C_0083dbb0_Construct(
    C_0083dbb0* self, void* /*edx*/, uint32_t* owner)
{
    AiAutonomous_ConstructTaskBase(self, nullptr, reinterpret_cast<int>(owner));
    self->vtbl = CSF_VTBL(0x0083DBB0);
    self->field_0004 = reinterpret_cast<uint32_t>(CSF_VTBL(0x0083DB94));

    auto* members = reinterpret_cast<uint8_t*>(self);

    CharTask_InitBase(members + 0x40, nullptr);
    CSF_AT(void**, self, 0x40) = CSF_VTBL(0x0083C568);
    CSF_AT(void**, self, 0x44) = CSF_VTBL(0x0083C54C);

    CharTask_InitBase(members + 0x54, nullptr);
    CSF_AT(void**, self, 0x54) = CSF_VTBL(0x0083C620);
    CSF_AT(void**, self, 0x58) = CSF_VTBL(0x0083C600);

    CharTask_InitBase(members + 0x68, nullptr);
    CSF_AT(void**, self, 0x68) = CSF_VTBL(0x0083C6D8);
    CSF_AT(void**, self, 0x6C) = CSF_VTBL(0x0083C6B8);

    CharTask_InitBase(members + 0x7C, nullptr);
    CSF_AT(void**, self, 0x7C) = CSF_VTBL(0x0083C790);
    CSF_AT(void**, self, 0x80) = CSF_VTBL(0x0083C770);

    CharTask_InitBase(members + 0x90, nullptr);
    CSF_AT(void**, self, 0x90) = CSF_VTBL(0x0083C850);
    CSF_AT(void**, self, 0x94) = CSF_VTBL(0x0083C830);

    CharTask_InitBase(members + 0xA4, nullptr);
    CSF_AT(void**, self, 0xA4) = CSF_VTBL(0x0083C910);
    CSF_AT(void**, self, 0xA8) = CSF_VTBL(0x0083C8F0);

    CharTask_InitBase(members + 0xB8, nullptr);
    CSF_AT(void**, self, 0xB8) = CSF_VTBL(0x0083C9D0);
    CSF_AT(void**, self, 0xBC) = CSF_VTBL(0x0083C9B4);

    CharTask_InitBase(members + 0xCC, nullptr);
    CSF_AT(void**, self, 0xCC) = CSF_VTBL(0x0083CA98);
    CSF_AT(void**, self, 0xD0) = CSF_VTBL(0x0083CA7C);
    CSF_AT(uint32_t, self, 0xE0) = 0;

    CharTask_InitBase(members + 0xE4, nullptr);
    CSF_AT(void**, self, 0xE4) = CSF_VTBL(0x0083CB60);
    CSF_AT(void**, self, 0xE8) = CSF_VTBL(0x0083CB40);

    CharTask_InitBase(members + 0xF8, nullptr);
    CSF_AT(void**, self, 0xF8) = CSF_VTBL(0x0083CC28);
    CSF_AT(void**, self, 0xFC) = CSF_VTBL(0x0083CC08);

    CharTask_InitBase(members + 0x10C, nullptr);
    CSF_AT(void**, self, 0x10C) = CSF_VTBL(0x0083CCF0);
    CSF_AT(void**, self, 0x110) = CSF_VTBL(0x0083CCD0);

    CharTask_InitBase(members + 0x120, nullptr);
    CSF_AT(void**, self, 0x120) = CSF_VTBL(0x0083CDA8);
    CSF_AT(void**, self, 0x124) = CSF_VTBL(0x0083CD88);

    CharTask_InitBase(members + 0x134, nullptr);
    CSF_AT(void**, self, 0x134) = CSF_VTBL(0x0083CE60);
    CSF_AT(void**, self, 0x138) = CSF_VTBL(0x0083CE40);

    CharTask_InitBase(members + 0x148, nullptr);
    CSF_AT(void**, self, 0x148) = CSF_VTBL(0x0083CF28);
    CSF_AT(void**, self, 0x14C) = CSF_VTBL(0x0083CF08);
    CSF_AT(uint32_t, self, 0x15C) = 0;
    CSF_AT(uint32_t, self, 0x160) = 0;

    CharTask_InitBase(members + 0x164, nullptr);
    CSF_AT(void**, self, 0x164) = CSF_VTBL(0x0083D8B8);
    CSF_AT(void**, self, 0x168) = CSF_VTBL(0x0083D898);

    CharTask_InitBase(members + 0x178, nullptr);
    CSF_AT(void**, self, 0x178) = CSF_VTBL(0x0083CFF0);
    CSF_AT(void**, self, 0x17C) = CSF_VTBL(0x0083CFD0);

    CharTask_InitBase(members + 0x18C, nullptr);
    CSF_AT(void**, self, 0x18C) = CSF_VTBL(0x0083D0A8);
    CSF_AT(void**, self, 0x190) = CSF_VTBL(0x0083D088);

    AiAutonomous_ConstructConversationTask(members + 0x1A0, nullptr);
    AiAutonomous_ConstructAutonomousTask(members + 0x224, nullptr);
    AiAutonomous_ConstructThrowAtPositionTask(members + 0x2AC, nullptr);

    self->field_0318 = 0;
    self->field_031c = 0;
    self->field_0320 = 0;
    self->field_0324 = 0;
    self->field_0328 = -1;
    self->field_032c = 0;
    self->field_032d = 1;
    self->field_032e = 0;
    C_004d1610_ClearHeader(members + 0x330, nullptr);

    self->field_02f4 = 1;
    self->field_02f8 = 0;
    self->field_02fc = 0;
    self->field_0300 = 0;
    self->field_0304 = 0;
    self->field_0314 = 0;
    self->field_033c = 0;

    auto* attackTasks = static_cast<C_00831ac0*>(Mem_Alloc(752));
    self->field_0308 = attackTasks
        ? C_00831ac0_InitializeAttackTaskGroup(
              attackTasks, nullptr, reinterpret_cast<int>(owner))
        : 0;

    auto* configuredTasks = static_cast<C_0083aa80*>(Mem_Alloc(1820));
    self->field_030c = configuredTasks
        ? C_0083aa80_Construct(configuredTasks, nullptr,
                              reinterpret_cast<int>(owner),
                              static_cast<int>(self->field_0308))
        : 0;

    void* additionalTaskStorage = Mem_Alloc(284);
    uint32_t* additionalTask = additionalTaskStorage
        ? C_00834fa8_Construct(
              additionalTaskStorage, nullptr, reinterpret_cast<int>(owner))
        : nullptr;

    const uint32_t attackTaskValue = self->field_0308;
    self->field_0310 = reinterpret_cast<uint32_t>(additionalTask);
    Game_AppendArrayValue(owner, nullptr, static_cast<int>(attackTaskValue));
    Game_AppendArrayValue(owner, nullptr, static_cast<int>(self->field_030c));
    Game_AppendArrayValue(owner, nullptr, static_cast<int>(self->field_0310));

    self->field_0008 = reinterpret_cast<uint32_t>(self);
    self->field_0048 = reinterpret_cast<uint32_t>(self);
    CSF_AT(C_0083dbb0*, reinterpret_cast<void*>(self->field_0008), 0x22C) = self;
    return reinterpret_cast<int>(self);
}

HOOK(0x004D99F0, C_0083dbb0_Construct, "auto");
