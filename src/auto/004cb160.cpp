#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CharacterTask_Prepare_492720, 0x00492720, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, Character_SetActionMode, 0x00492760, (void* self, void* edx, int a2));
CSF_FUNC(void, __stdcall, CoverAttack_Notify_4C72A0, 0x004C72A0, (int a1, char arg4, char a3));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, AiState_Init_4DC150, 0x004DC150, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, GetStateValue, 0x0050DD30, (void* self, void* edx, int a2));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));

// Returns the owner object of the linked value (value - 4) or null.
static uint8_t* GetLinkedOwner(int state)
{
    int v = GetLinkedObjectValue_4DB5C0(reinterpret_cast<void*>(state), nullptr);
    return v ? reinterpret_cast<uint8_t*>(v - 4) : nullptr;
}

static void __fastcall TFncAtacarDesdePosicion_EnterAttackPosition(TFncAtacarDesdePosicion* self, void* /*edx*/, int a2)
{
    uint8_t* owner = GetLinkedOwner(a2);
    CharacterTask_Prepare_492720(CSF_AT(void*, owner, 0x2C), nullptr, 3);

    owner = GetLinkedOwner(a2);
    void* character = CSF_AT(void*, owner, 0x2C);
    if (CSF_AT(int, character, 0x1C4) == 0) {
        owner = GetLinkedOwner(a2);
        Character_SetActionMode(CSF_AT(void*, owner, 0x2C), nullptr, 1);
    } else {
        owner = GetLinkedOwner(a2);
        Character_SetActionMode(CSF_AT(void*, owner, 0x2C), nullptr, 0);
    }

    owner = GetLinkedOwner(a2);
    CSF_AT(uint8_t, owner, 0x6B0) = 0;

    owner = GetLinkedOwner(a2);
    uint32_t* pos = reinterpret_cast<uint32_t*>(
        GetObjectPositionVector(CSF_AT(void*, owner, 0x2C), nullptr));
    self->field_0010 = pos[0];
    CSF_AT(uint32_t, self, 0x14) = pos[1];
    CSF_AT(uint32_t, self, 0x18) = pos[2];

    owner = GetLinkedOwner(a2);
    CSF_AT(int, owner, 0x2F8) = 1;
    CSF_AT(int, owner, 0x300) = 0;
    CSF_AT(int, owner, 0x304) = 0;
    CSF_AT(int, owner, 0x30C) = 0;
    CSF_AT(int, owner, 0x308) = 3;

    owner = GetLinkedOwner(a2);
    CSF_AT(int, owner, 0x300) = 0x3F000000;
    CSF_AT(int, owner, 0x304) = 0x3F000000;

    AiState_Init_4DC150(reinterpret_cast<void*>(a2), nullptr,
                        static_cast<int>(reinterpret_cast<uintptr_t>(reinterpret_cast<uint8_t*>(self) + 4)));

    void* provider = GetCachedObjectProvider(reinterpret_cast<void*>(a2), nullptr);

    void* gs = reinterpret_cast<void*>(GetGlobalState_50D190());
    char flagA = GetStateValue(gs, nullptr, 10) > 6;
    gs = reinterpret_cast<void*>(GetGlobalState_50D190());
    char flagB = GetStateValue(gs, nullptr, 10) > 6;

    CoverAttack_Notify_4C72A0(static_cast<int>(reinterpret_cast<uintptr_t>(provider)), flagB, flagA);
}

HOOK(0x004CB160, TFncAtacarDesdePosicion_EnterAttackPosition, "auto");
