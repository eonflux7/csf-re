#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(std::uint32_t*, __cdecl, RwMatrixCreate, 0x00624D40, ());

// Resolves a handle from a3 through the owner object, or creates a new RW matrix.
// a3 >= 0: owner vslot 0x80 (stdcall, ecx left as the owner) resolves the id; the result's vslot 0x3C receives a2.
// a3 < 0: owner vslot 0x68 (ecx = owner, no args) yields a sub-object; its vslot 0x6C resolves (a3 & 0x7FFFFFFF);
//         the result's vslot 0x1C receives a2.
// Fallback: a2 points to a new matrix pointer, and the byte at a2+4 is set to 1.
static int __fastcall C_00861d48_ResolveOrCreateMatrix(C_00861d48* self, void* /*edx*/, int a2, int a3)
{
    void* owner = reinterpret_cast<void*>(self->field_0008);

    if (a3 >= 0)
    {
        // Stdcall slot: the original leaves ecx as the owner object and pushes one argument.
        // Declared fastcall so that ecx = owner and the single stack argument is a3.
        void* found = ((void*(__fastcall*)(void*, void*, int))CSF_AT(void**, owner, 0)[32])(owner, nullptr, a3);
        if (found)
        {
            ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, found, 0)[15])(found, nullptr, a2);
            return a2;
        }
    }
    else
    {
        void* subObject = ((void*(__fastcall*)(void*, void*))CSF_AT(void**, owner, 0)[26])(owner, nullptr);
        void* found = ((void*(__fastcall*)(void*, void*, int))CSF_AT(void**, subObject, 0)[27])(
            subObject, nullptr, a3 & 0x7FFFFFFF);
        if (found)
        {
            ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, found, 0)[7])(found, nullptr, a2);
            return a2;
        }
    }

    *reinterpret_cast<std::uint32_t**>(a2) = RwMatrixCreate();
    *reinterpret_cast<std::uint8_t*>(a2 + 4) = 1;
    return a2;
}

HOOK(0x006E6D90, C_00861d48_ResolveOrCreateMatrix, "auto");
