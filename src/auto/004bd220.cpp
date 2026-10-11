#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void*, __fastcall, AiCore_PrepareLinkedObject, 0x004AF050, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, GetLinkedObjectField10, 0x004DB5D0, (void* self, void*));

static int __stdcall AiCore_SubmitLinkedPositionRequest(void* link)
{
    int linkedValue = GetLinkedObjectValue_4DB5C0(link, nullptr);
    void* owner = linkedValue
        ? reinterpret_cast<uint8_t*>(linkedValue) - 4
        : nullptr;
    void* component = CSF_AT(void*, owner, 0x2C);
    void* controller = CSF_AT(void*, component, 0x44);

    if (!((bool(__fastcall*)(void*, void*))
            CSF_AT(void**, controller, 0)[0x90 / 4])(controller, nullptr))
    {
        linkedValue = GetLinkedObjectValue_4DB5C0(link, nullptr);
        owner = linkedValue
            ? reinterpret_cast<uint8_t*>(linkedValue) - 4
            : nullptr;
        component = CSF_AT(void*, owner, 0x2C);
        controller = CSF_AT(void*, component, 0x44);

        ((void(__fastcall*)(void*, void*))
            CSF_AT(void**, controller, 0)[0x8C / 4])(controller, nullptr);
    }

    linkedValue = GetLinkedObjectValue_4DB5C0(link, nullptr);
    owner = linkedValue
        ? reinterpret_cast<uint8_t*>(linkedValue) - 4
        : nullptr;
    AiCore_PrepareLinkedObject(owner, nullptr);

    linkedValue = GetLinkedObjectValue_4DB5C0(link, nullptr);
    owner = linkedValue
        ? reinterpret_cast<uint8_t*>(linkedValue) - 4
        : nullptr;
    component = CSF_AT(void*, owner, 0x2C);
    controller = CSF_AT(void*, component, 0x44);

    alignas(8) uint8_t requestBytes[0x24]{};
    CSF_AT(int, requestBytes, 0x0C) = -1;
    CSF_AT(int, requestBytes, 0x10) = -1;
    CSF_AT(int, requestBytes, 0x00) = 0;
    CSF_AT(void**, requestBytes, 0x08) = CSF_VTBL(0x0082D448);
    CSF_AT(int, requestBytes, 0x14) = 0;
    CSF_AT(int, requestBytes, 0x18) = 0;
    CSF_AT(int, requestBytes, 0x1C) = 0;
    CSF_AT(uint8_t, requestBytes, 0x20) = 1;
    CSF_AT(uint8_t, requestBytes, 0x21) = 0;
    CSF_AT(uint8_t, requestBytes, 0x22) = 1;
    CSF_AT(int, requestBytes, 0x04) = 3;

    void* linkedField = reinterpret_cast<void*>(
        GetLinkedObjectField10(link, nullptr));
    CSF_AT(int, requestBytes, 0x14) = CSF_AT(int, linkedField, 0x0C);
    CSF_AT(int, requestBytes, 0x18) = CSF_AT(int, linkedField, 0x10);
    CSF_AT(int, requestBytes, 0x1C) = CSF_AT(int, linkedField, 0x14);
    CSF_AT(uint8_t, requestBytes, 0x20) = 0;
    CSF_AT(uint8_t, requestBytes, 0x22) = 0;

    return ((int(__fastcall*)(void*, void*, void*))
        CSF_AT(void**, controller, 0)[0x58 / 4])(
            controller, nullptr, requestBytes);
}

HOOK(0x004BD220, AiCore_SubmitLinkedPositionRequest, "auto");
