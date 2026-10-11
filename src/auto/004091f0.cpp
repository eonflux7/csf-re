#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(std::uint32_t*, __cdecl, RwMatrixCreate, 0x00624D40, ());

// Walks field_0008 -> (slot 0x28) -> (slot 0x68) -> (slot 0x6C), then hands a2 to the
// final object's slot 0x1C. If any step yields nothing, creates a fresh RwMatrix into *a2
// and sets the flag byte at a2+4.
static int __fastcall C_0082d510_FetchInnerObjectInto(C_0082d510* self, void* /*edx*/, int a2, int a3)
{
    void* ownerObj = reinterpret_cast<void*>(self->field_0008);
    std::uintptr_t v3 = ((std::uintptr_t(__fastcall*)(void*, void*))CSF_AT(void**, ownerObj, 0)[0x28 / 4])(ownerObj, nullptr);

    void* v3Obj = reinterpret_cast<void*>(v3);
    std::uintptr_t v4 = ((std::uintptr_t(__fastcall*)(void*, void*))CSF_AT(void**, v3Obj, 0)[0x68 / 4])(v3Obj, nullptr);

    if (v4 != 0)
    {
        void* v4Obj = reinterpret_cast<void*>(v4);
        std::uintptr_t v5 = ((std::uintptr_t(__fastcall*)(void*, void*, int))CSF_AT(void**, v4Obj, 0)[0x6C / 4])(v4Obj, nullptr, a3);
        if (v5 != 0)
        {
            void* v5Obj = reinterpret_cast<void*>(v5);
            ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, v5Obj, 0)[0x1C / 4])(v5Obj, nullptr, a2);
            return a2;
        }
    }

    std::uint8_t* out = reinterpret_cast<std::uint8_t*>(static_cast<std::uintptr_t>(a2));
    CSF_AT(std::uint32_t, out, 0) = static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(RwMatrixCreate()));
    CSF_AT(std::uint8_t, out, 4) = 1;
    return a2;
}

HOOK(0x004091F0, C_0082d510_FetchInnerObjectInto, "auto");
