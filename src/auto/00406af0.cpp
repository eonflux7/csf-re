#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(std::uint32_t*, __cdecl, RwMatrixCreate, 0x00624D40, ());

static int __fastcall C_0082d77c_ResolveChildOrCreateMatrix(C_0082d77c* self, void* /*edx*/, int a2, int a3)
{
    void* fieldObj = reinterpret_cast<void*>(static_cast<std::uintptr_t>(self->field_0008));

    // Slot 0x28 (no arguments) on the field object yields the resolver object.
    void* resolver = ((void* (__fastcall*)(void*, void*))CSF_AT(void**, fieldObj, 0)[0x28 / 4])(fieldObj, nullptr);

    if (a3 >= 0)
    {
        // Slot 0x80: look up child by index a3.
        void* child = ((void* (__fastcall*)(void*, void*, int))CSF_AT(void**, resolver, 0)[0x80 / 4])(resolver, nullptr, a3);
        if (child != nullptr)
        {
            // Slot 0x3C: invoke child with a2.
            ((void (__fastcall*)(void*, void*, int))CSF_AT(void**, child, 0)[0x3C / 4])(child, nullptr, a2);
            return a2;
        }
    }
    else
    {
        // Slot 0x68: obtain intermediate object from resolver.
        void* intermediate = ((void* (__fastcall*)(void*, void*))CSF_AT(void**, resolver, 0)[0x68 / 4])(resolver, nullptr);
        // Slot 0x6C: look up child by index with the sign bit cleared.
        void* leaf = ((void* (__fastcall*)(void*, void*, int))CSF_AT(void**, intermediate, 0)[0x6C / 4])(intermediate, nullptr, a3 & 0x7FFFFFFF);
        if (leaf != nullptr)
        {
            // Slot 0x1C: invoke leaf with a2.
            ((void (__fastcall*)(void*, void*, int))CSF_AT(void**, leaf, 0)[0x1C / 4])(leaf, nullptr, a2);
            return a2;
        }
    }

    // Fallback: create a new matrix and store it in the output block.
    std::uint32_t* matrix = RwMatrixCreate();
    *reinterpret_cast<std::uint32_t*>(static_cast<std::uintptr_t>(a2)) =
        static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(matrix));
    *reinterpret_cast<std::uint8_t*>(static_cast<std::uintptr_t>(a2) + 4) = 1;
    return a2;
}

HOOK(0x00406AF0, C_0082d77c_ResolveChildOrCreateMatrix, "auto");
