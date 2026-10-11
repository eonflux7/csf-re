#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(std::uint32_t*, __cdecl, RwMatrixCreate, 0x00624D40, ());

// Untyped object calls (slot numbers are byte offset / 4).
using NoArgGetter = void*(__fastcall*)(void* self, void* edx);
using PtrFromInt = void*(__fastcall*)(void* self, void* edx, int arg);
using IntFromInt = int(__fastcall*)(void* self, void* edx, int arg);
using VoidFromInt = void(__fastcall*)(void* self, void* edx, int arg);

// Returns the object held in this->field_0008, as an untyped pointer.
static void* FieldObjectOf(C_0082d77c* self)
{
    return reinterpret_cast<void*>(static_cast<std::uintptr_t>(self->field_0008));
}

// Calls vGetField016c (vtable slot 0x28) on the given object.
static void* CallGetField016c(void* obj)
{
    return reinterpret_cast<NoArgGetter>(CSF_AT(void**, obj, 0)[10])(obj, nullptr);
}

static int __fastcall C_0082d77c_ResolveSlotOrCreate(C_0082d77c* self, void* /*edx*/, int a2, int a3)
{
    void* v4 = CallGetField016c(FieldObjectOf(self));

    if (a3 >= 0)
    {
        // Slot 0x88 on v4 with a3 as argument.
        int v10 = reinterpret_cast<IntFromInt>(CSF_AT(void**, v4, 0)[34])(v4, nullptr, a3);

        void* v11 = CallGetField016c(FieldObjectOf(self));
        // Slot 0x80 on v11 with v10 as argument.
        void* v12 = reinterpret_cast<PtrFromInt>(CSF_AT(void**, v11, 0)[32])(v11, nullptr, v10);
        if (v12 != nullptr)
        {
            // Slot 0x3C on v12 with a2 as argument.
            reinterpret_cast<VoidFromInt>(CSF_AT(void**, v12, 0)[15])(v12, nullptr, a2);
            return a2;
        }
    }
    else
    {
        // Slot 0x68 on v4, no arguments.
        void* v6 = reinterpret_cast<NoArgGetter>(CSF_AT(void**, v4, 0)[26])(v4, nullptr);
        // Slot 0x68 on v6 with the index without the sign bit.
        int v7 = reinterpret_cast<IntFromInt>(CSF_AT(void**, v6, 0)[26])(v6, nullptr, a3 & 0x7FFFFFFF);
        // Slot 0x6C on v6 with v7 as argument.
        void* v8 = reinterpret_cast<PtrFromInt>(CSF_AT(void**, v6, 0)[27])(v6, nullptr, v7);
        if (v8 != nullptr)
        {
            // Slot 0x1C on v8 with a2 as argument.
            reinterpret_cast<VoidFromInt>(CSF_AT(void**, v8, 0)[7])(v8, nullptr, a2);
            return a2;
        }
    }

    // Fallback: create a new matrix into the caller's output block.
    void* out = reinterpret_cast<void*>(static_cast<std::uintptr_t>(a2));
    CSF_AT(std::uint32_t*, out, 0) = RwMatrixCreate();
    CSF_AT(std::uint8_t, out, 4) = 1;
    return a2;
}

HOOK(0x004062D0, C_0082d77c_ResolveSlotOrCreate, "auto");
