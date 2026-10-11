#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(std::uint32_t*, __cdecl, RwMatrixCreate, 0x00624D40, ());

// Negative ids are resolved through the owner object's lookup chain and the
// result is handed back through the callback at slot 0x1C. Otherwise a new
// matrix is created and stored at a2, with the flag byte at a2+4 set to 1.
static int __fastcall C_0082d844_ResolveIdOrCreateMatrix(C_0082d844* self, void* /*edx*/, int a2, int a3)
{
    if (a3 < 0) {
        void* ownerObj = reinterpret_cast<void*>(self->field_0008);
        void* v3 = reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, ownerObj, 0)[0x28 / 4])(ownerObj, nullptr);

        void* v4 = reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, v3, 0)[0x68 / 4])(v3, nullptr);

        if (v4 != nullptr) {
            void* v5 = reinterpret_cast<void* (__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, v4, 0)[0x6C / 4])(v4, nullptr, a3 & 0x7FFFFFFF);

            if (v5 != nullptr) {
                reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
                    CSF_AT(void**, v5, 0)[0x1C / 4])(v5, nullptr, a2);
                return a2;
            }
        }
    }

    std::uint32_t* matrix = RwMatrixCreate();
    *reinterpret_cast<std::uint32_t**>(static_cast<std::uintptr_t>(a2)) = matrix;
    *reinterpret_cast<std::uint8_t*>(static_cast<std::uintptr_t>(a2) + 4) = 1;
    return a2;
}

HOOK(0x409590, C_0082d844_ResolveIdOrCreateMatrix, "auto");
