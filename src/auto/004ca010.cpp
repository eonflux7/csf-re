#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, TMovil_sub_492720, 0x00492720, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, Character_SetActionMode, 0x00492760, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, TMovil_sub_4C6350, 0x004C6350, (void* self, void* edx, int a1));
CSF_FUNC(int, __fastcall, C_004D10F0_ResetFields, 0x004D10F0, (void* self, void*));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, TMovil_sub_4DC150, 0x004DC150, (void* self, void* edx, int a2));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));

// Returns the owner object (linked value - 4) or null.
static uint8_t* GetLinkedOwner(void* state)
{
    int linked = GetLinkedObjectValue_4DB5C0(state, nullptr);
    if (linked)
        return reinterpret_cast<uint8_t*>(linked - 4);
    return nullptr;
}

static int __fastcall TMovilAPuntoParapetandose_OnEnter(void* self, void* /*edx*/, int a2)
{
    void* state = reinterpret_cast<void*>(a2);
    uint8_t* owner;

    owner = GetLinkedOwner(state);
    CSF_AT(int, owner, 0x6DC) = 0;

    owner = GetLinkedOwner(state);
    CSF_AT(uint8_t, owner, 0x6B0) = 0;

    owner = GetLinkedOwner(state);
    C_004D10F0_ResetFields(owner + 0x690, nullptr);

    owner = GetLinkedOwner(state);
    TMovil_sub_492720(CSF_AT(void*, owner, 0x2C), nullptr, 3);

    owner = GetLinkedOwner(state);
    void* character = CSF_AT(void*, owner, 0x2C);
    ((void(__fastcall*)(void*, void*, int, int, int, int))CSF_AT(void**, character, 0)[0x8C / 4])(
        character, nullptr, 0, 0, -1, 0);

    owner = GetLinkedOwner(state);
    Character_SetActionMode(CSF_AT(void*, owner, 0x2C), nullptr, 1);

    uint8_t* sub = reinterpret_cast<uint8_t*>(self) + 4;
    TMovil_sub_4DC150(state, nullptr, reinterpret_cast<int>(sub));

    void* provider = GetCachedObjectProvider(state, nullptr);
    return TMovil_sub_4C6350(sub, nullptr, reinterpret_cast<int>(provider));
}

HOOK(0x004CA010, TMovilAPuntoParapetandose_OnEnter, "auto");
