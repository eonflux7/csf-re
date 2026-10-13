#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/vc71_vector.h"
// Binary-derived _Insert_n, cluster 60; 32-byte elements.
using Value = legacy::vc71::element<32>;
using Vector = legacy::vc71::vector<Value>;
CSF_FUNC(uint32_t, __fastcall, CopyNew, 0x0046D9B0, (Vector* self, void* edx, uint32_t begin, uint32_t end, uint32_t dest));
CSF_FUNC(uint32_t, __fastcall, Fill, 0x0046DAD0, (Vector* self, void* edx, uint32_t dest, uint32_t count, const Value* value));
CSF_FUNC(uint32_t, __cdecl, CopySpare, 0x0046C820, (uint32_t begin, uint32_t end, uint32_t dest, Vector* context, uint32_t tag));
CSF_FUNC(uint32_t, __cdecl, CopyBackward, 0x0046D690, (uint32_t begin, uint32_t end, uint32_t dest));
CSF_FUNC(uint32_t, __cdecl, Assign, 0x0046D650, (uint32_t begin, uint32_t end, const Value* value));
CSF_FUNC(uint32_t, __fastcall, Construct0, 0x004109E0, (void* self, void* edx, uint32_t a0, uint32_t a1, uint32_t a2));
CSF_FUNC(uint32_t, __cdecl, Finish0, 0x00690E60, (uint32_t a0));
CSF_FUNC(uint32_t, __cdecl, DestroyOld, 0x00697A80, (uint32_t begin, uint32_t end, Vector* context, uint32_t tag));
CSF_FUNC(uint32_t, __fastcall, DestroyFailed, 0x00697CE0, (Vector* self, void* edx, uint32_t begin, uint32_t end));
struct Operations {
    using value_type = Value;
    static __forceinline uint32_t split(uint32_t last, uint32_t count) { return last + 4u * (static_cast<uint32_t>(-8) * count); }
    static constexpr bool owns_value = false;
    static __forceinline void construct(Value* dest, const Value* source) { *reinterpret_cast<uint32_t*>(reinterpret_cast<uint8_t*>(dest) + 24) = 0xFu;
        *reinterpret_cast<uint32_t*>(reinterpret_cast<uint8_t*>(dest) + 20) = 0x0u;
        *reinterpret_cast<uint8_t*>(reinterpret_cast<uint8_t*>(dest) + 4) = 0x0u;
        Construct0(reinterpret_cast<uint8_t*>(dest), nullptr, legacy::vc71::address(source), 0x0u, 0xFFFFFFFFu);
        *reinterpret_cast<uint32_t*>(reinterpret_cast<uint8_t*>(dest) + 28) = *reinterpret_cast<const uint32_t*>(reinterpret_cast<const uint8_t*>(source) + 28); }
    static __forceinline uint32_t release_value(Value*) { return 0; }
    static __forceinline uint32_t copy_new(Vector* self, uint32_t begin, uint32_t end, uint32_t dest) { return CopyNew(self, nullptr, begin, end, dest); }
    static __forceinline uint32_t copy_spare(Vector* self, uint32_t begin, uint32_t end, uint32_t dest) { return CopySpare(begin, end, dest, self, 0); }
    static __forceinline uint32_t fill(Vector* self, uint32_t dest, uint32_t count, const Value* value) { return Fill(self, nullptr, dest, count, value); }
    static __forceinline uint32_t copy_backward(uint32_t begin, uint32_t end, uint32_t dest) { return CopyBackward(begin, end, dest); }
    static __forceinline uint32_t assign(uint32_t begin, uint32_t end, const Value* value) { return Assign(begin, end, value); }
    static __forceinline void destroy_old(Vector* self, uint32_t begin, uint32_t end) { DestroyOld(begin, end, self, 0); }
    static __forceinline void destroy_failed(Vector* self, uint32_t begin, uint32_t end) { DestroyFailed(self, nullptr, begin, end); }
    static __forceinline uint32_t finish_value(Value* value, uint32_t result) { uint32_t eax = result;
        if (*reinterpret_cast<uint32_t*>(reinterpret_cast<uint8_t*>(value) + 24) < 0x10u) goto L_46ddd8;
        eax = *reinterpret_cast<uint32_t*>(reinterpret_cast<uint8_t*>(value) + 4);
        if ((eax & eax) == 0) goto L_46ddd8;
        eax = Finish0(eax);
        L_46ddd8:;
        return eax;
    }
};
static uint32_t __fastcall StlInsert_0046db30(Vector* self, void*, Value* pos, uint32_t count, const Value* value) {
    return legacy::vc71::vector_insert_n<Operations>(self, pos, count, value);
}
HOOK(0x0046DB30, StlInsert_0046db30, "stl");
