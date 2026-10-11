#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, AiTaskStates_ConstructBase, 0x0041E1D0, (void* self, void* edx, int a2));
CSF_FUNC(uint32_t*, __fastcall, AiTaskStates_ConstructTaskMember, 0x0041E730, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, AiTaskStates_ConstructFreeMove, 0x004213E0, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, AiTaskStates_ConstructNavigation, 0x00421450, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, AiTaskStates_ConstructTurnFunction, 0x004214F0, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, AiTaskStates_ConstructCharacterTask, 0x004DB4C0, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, C_0086c12c_Construct, 0x0075A360, (void* self, void* edx));

static int __fastcall C_0082f9f8_Construct(C_0082f9f8* self, void* /*edx*/, int owner)
{
    AiTaskStates_ConstructBase(self, nullptr, owner);
    self->vtbl = CSF_VTBL(0x0082F9F8);
    self->field_0004 = reinterpret_cast<uint32_t>(CSF_VTBL(0x0082F9DC));

    auto* storage = reinterpret_cast<uint8_t*>(self);

    AiTaskStates_ConstructCharacterTask(storage + 0x20, nullptr);
    CSF_AT(void**, self, 0x20) = CSF_VTBL(0x0082E8C0);
    CSF_AT(void**, self, 0x24) = CSF_VTBL(0x0082E8A0);

    AiTaskStates_ConstructCharacterTask(storage + 0x34, nullptr);
    CSF_AT(void**, self, 0x34) = CSF_VTBL(0x0082E980);
    CSF_AT(void**, self, 0x38) = CSF_VTBL(0x0082E960);

    AiTaskStates_ConstructCharacterTask(storage + 0x48, nullptr);
    CSF_AT(void**, self, 0x48) = CSF_VTBL(0x0082EA40);
    CSF_AT(void**, self, 0x4C) = CSF_VTBL(0x0082EA24);

    AiTaskStates_ConstructCharacterTask(storage + 0x60, nullptr);
    CSF_AT(void**, self, 0x60) = CSF_VTBL(0x0082EB00);
    CSF_AT(void**, self, 0x64) = CSF_VTBL(0x0082EAE4);

    AiTaskStates_ConstructCharacterTask(storage + 0x74, nullptr);
    CSF_AT(void**, self, 0x74) = CSF_VTBL(0x0082F6F8);
    CSF_AT(void**, self, 0x78) = CSF_VTBL(0x0082F6D8);

    AiTaskStates_ConstructCharacterTask(storage + 0x88, nullptr);
    CSF_AT(void**, self, 0x88) = CSF_VTBL(0x0082EBC0);
    CSF_AT(void**, self, 0x8C) = CSF_VTBL(0x0082EBA4);

    AiTaskStates_ConstructCharacterTask(storage + 0x9C, nullptr);
    CSF_AT(void**, self, 0x9C) = CSF_VTBL(0x0082EC80);
    CSF_AT(void**, self, 0xA0) = CSF_VTBL(0x0082EC60);

    AiTaskStates_ConstructFreeMove(storage + 0xB0, nullptr);
    AiTaskStates_ConstructNavigation(storage + 0x100, nullptr);

    AiTaskStates_ConstructCharacterTask(storage + 0x180, nullptr);
    CSF_AT(void**, self, 0x180) = CSF_VTBL(0x0082F340);
    CSF_AT(void**, self, 0x184) = CSF_VTBL(0x0082F320);

    AiTaskStates_ConstructCharacterTask(storage + 0x194, nullptr);
    CSF_AT(void**, self, 0x194) = CSF_VTBL(0x0082F400);
    CSF_AT(void**, self, 0x198) = CSF_VTBL(0x0082F3E0);

    AiTaskStates_ConstructTurnFunction(storage + 0x1A8, nullptr);

    self->field_020c = 0;
    self->field_0210 = 0;
    self->field_0214 = 0;
    self->field_0218 = 0;
    self->field_021c = 0;
    self->field_0220 = 0;
    self->field_0224 = 0;
    self->field_0228 = 0;
    self->field_022c = 0;
    self->field_0244 = 1;
    self->field_0250 = 0;
    self->field_0248 = 0;
    self->field_024c = 0;
    self->field_0258 = 0;
    self->field_0254 = 0;

    C_0086c12c_Construct(storage + 0x25C, nullptr);

    self->field_0278 = 0;
    self->field_027c = 0;
    self->field_0280 = 0;
    self->field_0284 = 0;
    self->field_0288 = 0;
    self->field_028c = 0;
    self->field_0290 = 0;
    self->field_0294 = 0;
    self->field_0298 = 0;

    AiTaskStates_ConstructTaskMember(storage + 0x29C, nullptr);

    self->field_0270 = -1;
    self->field_0238 = -1;
    auto* ownerObject = reinterpret_cast<void*>(self->field_0014);
    self->field_0274 = 0;
    self->field_0204 = 0;
    self->field_0208 = 0;
    self->field_0200 = 0;
    self->field_0230 = 0;
    self->field_0234 = 0;
    self->field_023c = 0;
    self->field_0240 = 0;
    self->field_02b8 = 0;
    self->field_0205 = 1;
    self->field_02bc = CSF_AT(uint32_t, ownerObject, 0x1D8);
    self->field_02c0 = CSF_AT(uint32_t, ownerObject, 0x1C4);
    self->field_02c4 = CSF_AT(uint32_t, ownerObject, 0x1E0);
    self->field_0008 = reinterpret_cast<uint32_t>(self);
    return reinterpret_cast<int>(self);
}

HOOK(0x00421E80, C_0082f9f8_Construct, "auto");
