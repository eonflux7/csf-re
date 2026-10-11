#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(std::uint32_t*, __cdecl, RwMatrixCreate, 0x00624D40, ());

// int __thiscall C_0082d510::ResolveSubmatrixInto(int a2, int a3), ret 8
static int __fastcall C_0082d510_ResolveSubmatrixInto(C_0082d510* self, void* /*edx*/, int a2, int a3)
{
    // Object reached through the field at +0x008; its type is not declared, so slots are called raw.
    void* fieldObj = reinterpret_cast<void*>(self->field_0008);
    void* v3 = reinterpret_cast<void* (__fastcall*)(void*, void*)>(CSF_AT(void**, fieldObj, 0)[0x28 / 4])(fieldObj, nullptr);
    void* v4 = reinterpret_cast<void* (__fastcall*)(void*, void*)>(CSF_AT(void**, v3, 0)[0x68 / 4])(v3, nullptr);
    void* v5 = v4;

    if (v4 != nullptr) {
        void* v6 = reinterpret_cast<void* (__fastcall*)(void*, void*, int)>(CSF_AT(void**, v4, 0)[0x68 / 4])(v4, nullptr, a3);
        void* v7 = reinterpret_cast<void* (__fastcall*)(void*, void*, void*)>(CSF_AT(void**, v5, 0)[0x6C / 4])(v5, nullptr, v6);
        if (v7 != nullptr) {
            reinterpret_cast<void (__fastcall*)(void*, void*, int)>(CSF_AT(void**, v7, 0)[0x1C / 4])(v7, nullptr, a2);
            return a2;
        }
    }

    // Fallback: create a fresh matrix into the output block.
    *reinterpret_cast<std::uint32_t*>(a2) = reinterpret_cast<std::uint32_t>(RwMatrixCreate());
    *(reinterpret_cast<std::uint8_t*>(a2) + 4) = 1;
    return a2;
}

HOOK(0x00409190, C_0082d510_ResolveSubmatrixInto, "auto");
