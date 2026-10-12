#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Forwarder at 0x48E4D0: ecx = inner object, one stack argument, callee pops it (no esp fixup after the call).
CSF_FUNC(int, __fastcall, DispatchSceneObjectWithPayload, 0x48E4D0, (void* self, void* edx, int payloadArg));

// The this-object (C_00833ee8**) and its inner object have no declared class in auto_classes.h,
// so both are accessed by raw offset.
static int __fastcall C_00833ee8_DispatchByObjectType(void* self, void* /*edx*/, int a2, int a3, int a4)
{
    void* inner = CSF_AT(void*, self, 0x14);
    uint32_t objectTypeId = CSF_AT(uint32_t, inner, 0x50);

    switch (objectTypeId)
    {
    case 8u:
    case 9u:
    case 0xAu:
    case 0xBu:
    case 0xEu:
    case 0xFu:
    case 0x10u:
    case 0x11u:
    {
        // inner->vtbl[0x28 / 4](inner) -> childObject
        void* childObject = ((void* (__fastcall*)(void*, void*))CSF_AT(void**, inner, 0)[10])(inner, nullptr);
        // childObject->vtbl[0x18 / 4](childObject) -> handler
        void* handler = ((void* (__fastcall*)(void*, void*))CSF_AT(void**, childObject, 0)[6])(childObject, nullptr);

        int argBlock[3];
        argBlock[0] = a2;
        argBlock[1] = a3;
        argBlock[2] = a4;

        // handler->vtbl[0x4C / 4](handler, argBlock)
        return ((int (__fastcall*)(void*, void*, int*))CSF_AT(void**, handler, 0)[19])(handler, nullptr, argBlock);
    }
    case 0x12u:
        return DispatchSceneObjectWithPayload(inner, nullptr, a3);
    default:
        return static_cast<int>(reinterpret_cast<uintptr_t>(self));
    }
}

HOOK(0x424ED0, C_00833ee8_DispatchByObjectType, "auto");
