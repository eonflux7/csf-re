#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(int, __cdecl, FreeBlock_690F70, 0x00690F70, (void* block));
CSF_FUNC(void, __fastcall, C_0085e65c_InitVtable, 0x006923A0, (C_0085e65c* self, void*));

// Destructor body of C_00867260 (derived from C_008670cc). Releases the owned
// sub-object at +0x04 (C_0085e65c), the owner's registrations, the heap block,
// and the RwMatrix, then restores the base vtable.
static void __fastcall C_00867260_Destroy(C_00867260* self, void* /*edx*/)
{
    std::uint32_t registeredHandle = CSF_AT(std::uint32_t, self, 0x18);
    C_0085e65c* padObject = reinterpret_cast<C_0085e65c*>(reinterpret_cast<std::uint8_t*>(self) + 0x04);

    *reinterpret_cast<void***>(self) = CSF_VTBL(0x00867260);
    *reinterpret_cast<void***>(padObject) = CSF_VTBL(0x0086725C);

    if (registeredHandle) {
        void* owner = CSF_AT(void*, self, 0x0C);
        CSF_AT(void**, owner, 0)[136 / 4]
            ? reinterpret_cast<void(__fastcall*)(void*, void*, std::uint32_t)>(CSF_AT(void**, owner, 0)[136 / 4])(owner, nullptr, registeredHandle)
            : void();
        CSF_AT(std::uint32_t, self, 0x18) = 0;
    }

    if (CSF_AT(std::uint32_t, self, 0x1C)) {
        void* owner = CSF_AT(void*, self, 0x0C);
        reinterpret_cast<void(__fastcall*)(void*, void*, std::uint32_t)>(CSF_AT(void**, owner, 0)[140 / 4])(
            owner, nullptr, CSF_AT(std::uint32_t, self, 0x1C));
        CSF_AT(std::uint32_t, self, 0x1C) = 0;
    }

    FreeBlock_690F70(reinterpret_cast<void*>(CSF_AT(std::uint32_t, self, 0x14)));

    {
        void* owner = CSF_AT(void*, self, 0x0C);
        reinterpret_cast<void(__fastcall*)(void*, void*, void*)>(CSF_AT(void**, owner, 0)[8 / 4])(
            owner, nullptr, static_cast<void*>(self));
    }

    if (CSF_AT(std::uint8_t, self, 0x88)) {
        RwMatrixDestroy(reinterpret_cast<std::uint32_t*>(CSF_AT(std::uint32_t, self, 0x84)));
    }

    *reinterpret_cast<void***>(self) = CSF_VTBL(0x008671D8);
    *reinterpret_cast<void***>(padObject) = CSF_VTBL(0x008671D4);
    C_0085e65c_InitVtable(padObject, nullptr);

    *reinterpret_cast<void***>(self) = CSF_VTBL(0x008670CC);
}

HOOK(0x00737360, C_00867260_Destroy, "auto");
