#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, C_0082f9f8_StartMovementAndResetFlags, 0x0047BF90, (C_0082f9f8* self, void* , int));
CSF_FUNC(const uint32_t*, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, GetLinkedObjectField10, 0x004DB5D0, (void* self, void*));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static char __stdcall AiOrder_HandleMovementTargetRequest15(int request)
{
    void* requestObject = reinterpret_cast<void*>(request);

    int linkedObject = GetLinkedObjectValue_4DB5C0(requestObject, nullptr);
    uint8_t* owner = linkedObject
        ? reinterpret_cast<uint8_t*>(linkedObject - 4)
        : nullptr;

    uint8_t* fields = reinterpret_cast<uint8_t*>(
        GetLinkedObjectField10(requestObject, nullptr));
    C_0082f9f8_StartMovementAndResetFlags(
        CSF_AT(C_0082f9f8*, owner, 0x718), nullptr,
        reinterpret_cast<int>(fields + 0x94));

    fields = reinterpret_cast<uint8_t*>(
        GetLinkedObjectField10(requestObject, nullptr));
    CSF_AT(uint8_t, fields, 0xB0) = 1;

    fields = reinterpret_cast<uint8_t*>(
        GetLinkedObjectField10(requestObject, nullptr));
    if (CSF_AT(int32_t, fields, 0x9C) >= 0)
    {
        void* scene = g_pCurrentScene;
        fields = reinterpret_cast<uint8_t*>(
            GetLinkedObjectField10(requestObject, nullptr));
        int target = CHandleTable_Resolve(
            scene, nullptr, reinterpret_cast<int>(fields + 0x94));

        if (target)
        {
            const uint32_t* position = GetObjectPositionVector(
                reinterpret_cast<void*>(target), nullptr);

            fields = reinterpret_cast<uint8_t*>(
                GetLinkedObjectField10(requestObject, nullptr));
            CSF_AT(uint32_t, fields, 0xB4) = position[0];
            CSF_AT(uint32_t, fields, 0xB8) = position[1];
            CSF_AT(uint32_t, fields, 0xBC) = position[2];

            fields = reinterpret_cast<uint8_t*>(
                GetLinkedObjectField10(requestObject, nullptr));
            CSF_AT(uint8_t, fields, 0xB1) = 1;

            linkedObject = GetLinkedObjectValue_4DB5C0(requestObject, nullptr);
            owner = linkedObject
                ? reinterpret_cast<uint8_t*>(linkedObject - 4)
                : nullptr;

            fields = reinterpret_cast<uint8_t*>(
                GetLinkedObjectField10(requestObject, nullptr));
            CSF_AT(uint32_t, owner, 0x38C) = 1;
            uint8_t* handle = fields + 0x94;
            CSF_AT(uint32_t, owner, 0x394) = 0;
            CSF_AT(uint32_t, owner, 0x398) = 0;
            CSF_AT(uint32_t, owner, 0x3A0) = CSF_AT(uint32_t, handle, 0x4);
            CSF_AT(uint32_t, owner, 0x3A4) = CSF_AT(uint32_t, handle, 0x8);

            linkedObject = GetLinkedObjectValue_4DB5C0(requestObject, nullptr);
            owner = linkedObject
                ? reinterpret_cast<uint8_t*>(linkedObject - 4)
                : nullptr;
            CSF_AT(float, owner, 0x394) = 0.5f;
            CSF_AT(float, owner, 0x398) = 0.5f;
        }
    }

    return 0;
}

HOOK(0x004AC7A0, AiOrder_HandleMovementTargetRequest15, "auto");
