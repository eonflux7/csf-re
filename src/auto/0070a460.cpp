#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(std::uint32_t*, __cdecl, RwMatrixCreate, 0x00624D40, ());
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __fastcall, Water_BaseInit, 0x006FF510, (void* self, void* edx, int a2));
CSF_FUNC(float*, __cdecl, Water_ApplyTransform, 0x00803770, (int a1, uint16_t a2));

static int __fastcall C_00863588_Attach(C_00863588* self, void* /*edx*/, std::uint32_t* a2)
{
    Water_BaseInit(self, nullptr, reinterpret_cast<int>(a2));

    // virtual call slot 6 (+0x18) on the object referenced by *a2
    void** srcObj = reinterpret_cast<void**>(*a2);
    typedef std::uint32_t(__fastcall * GetFn)(void*, void*);
    self->field_0030 = reinterpret_cast<GetFn>(CSF_AT(void**, srcObj, 0)[0x18 / 4])(srcObj, nullptr);

    if (CSF_AT(std::uint8_t, self, 0xD) != 0) {
        if (self->field_0034 == 0) {
            std::uint32_t v3 = reinterpret_cast<std::uint32_t>(Mem_Alloc(8));
            if (v3) {
                CSF_AT(std::uint32_t*, reinterpret_cast<void*>(v3), 0) = RwMatrixCreate();
                CSF_AT(std::uint8_t, reinterpret_cast<void*>(v3), 4) = 1;
            } else {
                v3 = 0;
            }
            self->field_0034 = v3;
        }

        alignas(8) std::uint8_t outBytes[8];
        void** matObj = reinterpret_cast<void**>(self->field_0030);
        typedef std::uint32_t**(__fastcall * GetMatrixFn)(void*, void*, void*);
        std::uint32_t** res = reinterpret_cast<GetMatrixFn>(CSF_AT(void**, matObj, 0)[0x8C / 4])(matObj, nullptr, outBytes);

        void* dst = reinterpret_cast<void*>(CSF_AT(std::uint32_t, reinterpret_cast<void*>(self->field_0034), 0));
        std::memcpy(dst, reinterpret_cast<const void*>(*res), 0x40);

        if (CSF_AT(std::uint8_t, outBytes, 4) != 0) {
            RwMatrixDestroy(reinterpret_cast<std::uint32_t*>(CSF_AT(std::uint32_t, outBytes, 0)));
        }
    }

    std::uint32_t inner = CSF_AT(std::uint32_t, reinterpret_cast<void*>(self->field_0004), 0x10);
    CSF_AT(std::uint32_t, reinterpret_cast<void*>(inner), 8) =
        CSF_AT(std::uint32_t, reinterpret_cast<void*>(self->field_0030), 0xC) + 16;

    return reinterpret_cast<int>(Water_ApplyTransform(static_cast<int>(self->field_0004), 0));
}

HOOK(0x0070A460, C_00863588_Attach, "auto");
