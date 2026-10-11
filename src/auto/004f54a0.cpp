#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, S_004f5420_ClearModelSlot, 0x004F5310, (void* self, void* edx, int a3));
CSF_FUNC(void, __fastcall, S_004f5420_SetModelState, 0x004F5420, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, GlobalInt_ResolveAccessor, 0x006B13A0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, AccessorState_ResolveSlot, 0x006C8EE0, (void* self, void* edx, int a2));

static char __fastcall S_004f5420_SelectModelSlot(
    S_004f5420* self, void* /*edx*/, int slotId,
    char flag, char optionA, char optionB)
{
    (void)CSF_VTBL(0x004F5420);

    alignas(8) uint8_t parameters[0x20]{};
    CSF_AT(float, parameters, 0x00) = 0.0f;
    CSF_AT(float, parameters, 0x04) = 0.2f;
    CSF_AT(int, parameters, 0x08) = 0;
    CSF_AT(char, parameters, 0x0C) = flag;
    CSF_AT(char, parameters, 0x0D) = optionA;
    CSF_AT(char, parameters, 0x0E) = optionB;
    CSF_AT(float, parameters, 0x10) = 1.0f;
    CSF_AT(float, parameters, 0x14) = 1.0f;
    CSF_AT(float, parameters, 0x18) = 1.0f;
    CSF_AT(char, parameters, 0x1C) = 0;
    CSF_AT(char, parameters, 0x1D) = 1;

    void* model = reinterpret_cast<void*>(self->field_0010);
    void* controller =
        ((void* (__fastcall*)(void*, void*))
            CSF_AT(void**, model, 0)[0x68 / 4])(model, nullptr);

    if (slotId == -1)
    {
        if (self->field_0014 == static_cast<uint32_t>(-1))
            return static_cast<char>(reinterpret_cast<uintptr_t>(controller));

        if (!self->field_0018)
        {
            bool active =
                ((bool (__fastcall*)(void*, void*, int))
                    CSF_AT(void**, controller, 0)[0x40 / 4])(
                        controller, nullptr, 0);
            if (!active)
                return 0;
        }

        return S_004f5420_ClearModelSlot(
            self, nullptr, reinterpret_cast<int>(parameters));
    }

    int global = GetGlobalInt();
    int accessor = GlobalInt_ResolveAccessor(
        reinterpret_cast<void*>(global), nullptr);
    void* accessorObject = reinterpret_cast<void*>(accessor);
    void* state =
        ((void* (__fastcall*)(void*, void*))
            CSF_AT(void**, accessorObject, 0)[0x14 / 4])(
                accessorObject, nullptr);

    if (self->field_0014 == static_cast<uint32_t>(slotId)
        && self->field_000c == 1)
    {
        return static_cast<char>(reinterpret_cast<uintptr_t>(state));
    }

    int slot = AccessorState_ResolveSlot(state, nullptr, slotId);
    if (!slot)
        return 0;

    if (self->field_0008 == 3)
    {
        S_004f5420_SetModelState(self, nullptr, 2);
        CSF_AT(float, parameters, 0x00) = 0.04f;
        CSF_AT(float, parameters, 0x04) = 0.0f;
    }

    char result =
        ((char (__fastcall*)(void*, void*, int, int, void*))
            CSF_AT(void**, controller, 0)[0x14 / 4])(
                controller, nullptr, 0, slot, parameters);

    self->field_0014 = static_cast<uint32_t>(slotId);
    self->field_0018 = static_cast<uint8_t>(flag);
    return result;
}

HOOK(0x004F54A0, S_004f5420_SelectModelSlot, "auto");
