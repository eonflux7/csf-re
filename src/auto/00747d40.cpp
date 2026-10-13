#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, LookupObjectByIndex, 0x0064AD00, (int a1));
CSF_FUNC(void**, __fastcall, NetCogibleDdl_FindProperty, 0x006622F0, (void* self, void* edx, void** a2, uint8_t* a3));
CSF_FUNC(void, __fastcall, NetDdl_SendSubField, 0x0066CF50, (void* self, void* edx, void* a2, int a3, int a4, int a5, int a6));
CSF_FUNC(char, __stdcall, NetDdl_FallbackDispatch, 0x0065E720, (int a1, int* a2));

// __thiscall(this, arg0, a3) with ret 8 -> fastcall-style with two stack args.
static char __fastcall C_00858180_DispatchSubFieldToOwner(C_00858180* self, void* /*edx*/, C_00858180* arg0, int* a3)
{
    C_00858180* originalArg0 = arg0;
    C_00858180* selfSubFieldAddr = reinterpret_cast<C_00858180*>(&self->field_0044);

    if (!arg0 || arg0 == selfSubFieldAddr)
    {
        int objectBase = LookupObjectByIndex(static_cast<int>(self->field_002c >> 22));

        // The original writes 1 into the high byte of its stack argument slot
        // (ebp+0xB, which is arg0's byte 3) and passes that address on.
        uint8_t* flagByte = reinterpret_cast<uint8_t*>(&arg0) + 3;
        *flagByte = 1;

        uint8_t* findResult = nullptr;
        NetCogibleDdl_FindProperty(reinterpret_cast<uint8_t*>(objectBase) + 0xC, nullptr,
                                   reinterpret_cast<void**>(&findResult), flagByte);

        C_00858180* ownerObj = CSF_AT(C_00858180*, findResult, 0x10);
        NetDdl_SendSubField(ownerObj, nullptr, self,
                            static_cast<int>(reinterpret_cast<uintptr_t>(selfSubFieldAddr)),
                            1, a3[0], a3[1]);

        if (originalArg0 == selfSubFieldAddr)
        {
            return 1;
        }
    }

    return NetDdl_FallbackDispatch(static_cast<int>(reinterpret_cast<uintptr_t>(originalArg0)), a3);
}

HOOK(0x00747D40, C_00858180_DispatchSubFieldToOwner, "auto");
