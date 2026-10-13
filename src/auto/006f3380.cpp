#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, GlobalInt_ResolveAccessor, 0x006B13A0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_00860a30_SetupRenderTargetFromDesc, 0x006CA410, (C_00860a30* self, void* , int a2));

// Forwards a query to the accessor returned by the global int's resolver.
// Descriptor at a2: if the length field at +0x1C is below 0x10 the inline
// buffer at +0x08 is passed, otherwise the pointer stored at +0x08 is passed.
static int __fastcall C_00862640_SetupRenderTargetAndForwardQuery(C_00862640* self, void* /*edx*/, int a2)
{
    C_00860a30_SetupRenderTargetFromDesc(self, nullptr, a2);

    int globalIntValue = GetGlobalInt();
    int accessor = GlobalInt_ResolveAccessor(reinterpret_cast<void*>(globalIntValue), nullptr);

    // vtable slot 4 (+0x10) of the accessor
    int forwarder = ((int(__fastcall*)(void*, void*))CSF_AT(void**, reinterpret_cast<void*>(accessor), 0)[4])(
        reinterpret_cast<void*>(accessor), nullptr);

    uint8_t* desc = reinterpret_cast<uint8_t*>(a2);
    if (CSF_AT(uint32_t, desc, 0x1C) < 0x10u)
    {
        // vtable slot 7 (+0x1C) of the forwarder, passing the inline buffer address
        return ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, reinterpret_cast<void*>(forwarder), 0)[7])(
            reinterpret_cast<void*>(forwarder), nullptr, a2 + 8);
    }
    else
    {
        int bufferPointer = static_cast<int>(CSF_AT(uint32_t, desc, 0x08));
        return ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, reinterpret_cast<void*>(forwarder), 0)[7])(
            reinterpret_cast<void*>(forwarder), nullptr, bufferPointer);
    }
}

HOOK(0x006F3380, C_00862640_SetupRenderTargetAndForwardQuery, "auto");
