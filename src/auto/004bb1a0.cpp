#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(void, __stdcall, PrepareStaticAiState, 0x004DC3C0, (void* a1));
CSF_FUNC(uint32_t, __fastcall, S_004dca10_DispatchWithArgBlock, 0x004DD060, (S_004dca10* self, void* , uint32_t* args));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(uint32_t*, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void*));

static int __stdcall AiDistracted_DispatchLinkedObjectPosition(void* state)
{
    alignas(8) uint8_t argumentBytes[0x1C];
    auto* arguments = reinterpret_cast<uint32_t*>(argumentBytes);

    PrepareStaticAiState(state);

    int linkedValue = GetLinkedObjectValue_4DB5C0(state, nullptr);
    void* firstOwner = linkedValue
        ? reinterpret_cast<void*>(static_cast<uint32_t>(linkedValue) - 4)
        : nullptr;

    linkedValue = GetLinkedObjectValue_4DB5C0(state, nullptr);
    void* secondOwner = linkedValue
        ? reinterpret_cast<void*>(static_cast<uint32_t>(linkedValue) - 4)
        : nullptr;

    auto* positionObject = CSF_AT(C_008342e8*, firstOwner, 0x2C);
    auto* identityObject = CSF_AT(C_008342e8*, secondOwner, 0x2C);
    uint32_t* position = GetObjectPositionVector(positionObject, nullptr);

    arguments[0] = 1;
    arguments[1] = reinterpret_cast<uint32_t>(CSF_VTBL(0x0082D448));
    arguments[2] = identityObject->field_0020;
    arguments[3] = identityObject->field_0024;
    arguments[4] = position[0];
    arguments[5] = position[1];
    arguments[6] = position[2];

    void* context = reinterpret_cast<void*>(GetValue8B0554());
    auto* dispatcher = reinterpret_cast<S_004dca10*>(
        CSF_AT(uint8_t*, context, 0x1FD8) + 0x54);
    S_004dca10_DispatchWithArgBlock(dispatcher, nullptr, arguments);

    linkedValue = GetLinkedObjectValue_4DB5C0(state, nullptr);
    void* owner;
    if (linkedValue) {
        owner = CSF_AT(void*, reinterpret_cast<void*>(linkedValue), 0x28);
    } else {
        void* nullOwner = nullptr;
        owner = CSF_AT(void*, nullOwner, 0x2C);
    }

    void* recipient = CSF_AT(void*, owner, 0x30);
    return reinterpret_cast<int(__fastcall*)(void*, void*)>(
        CSF_AT(void**, recipient, 0)[0xA0 / 4])(recipient, nullptr);
}

HOOK(0x004BB1A0, AiDistracted_DispatchLinkedObjectPosition, "auto");
