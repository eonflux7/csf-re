#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// __cdecl GetGlobalInt() -> int
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
// __fastcall LookupObjectById(this = id) -> object pointer
CSF_FUNC(void*, __fastcall, LookupObjectById, 0x00652160, (void* self, void* edx));

// Returns a HRESULT-style code describing why the raster lock on this emitter failed.
static int __fastcall C_00861a00_GetLockErrorCode(C_00861a00* self, void* /*edx*/)
{
    int globalId = GetGlobalInt();
    void* lookedUp = LookupObjectById(reinterpret_cast<void*>(static_cast<uintptr_t>(globalId)), nullptr);

    // Virtual slot 6 (offset 0x18) on the looked-up object returns its state object.
    void* stateObject = ((void* (__fastcall*)(void*, void*))CSF_AT(void**, lookedUp, 0)[6])(lookedUp, nullptr);
    if (CSF_AT(uint32_t, stateObject, 0x10) == 32)
        return static_cast<int>(0x80000003u);

    // Selector byte is stored in the high byte of a 16-bit value.
    uint8_t selectorByte = CSF_AT(uint8_t, reinterpret_cast<void*>(self->field_0014), 0x23);
    uint16_t selector = static_cast<uint16_t>(selectorByte << 8);

    if (selector > 0x300u)
    {
        if (selector == 0x400u)
            return static_cast<int>(0x80020000u);
        if (selector == 0xA00u)
            return static_cast<int>(0x80000009u);
    }
    else
    {
        switch (selector)
        {
        case 0x300u:
            return static_cast<int>(0x80000007u);
        case 0x100u:
            return static_cast<int>(0x80000008u);
        case 0x200u:
            return static_cast<int>(0x8000000Au);
        }
    }
    return static_cast<int>(0x80000000u);
}

HOOK(0x702BE0, C_00861a00_GetLockErrorCode, "auto");
