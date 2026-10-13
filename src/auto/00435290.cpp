#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/vc71_vector.h"
// Binary-derived _Insert_n, cluster None; 48-byte elements.
using Value = legacy::vc71::element<48>;
using Vector = legacy::vc71::vector<Value>;
CSF_FUNC(uint32_t, __fastcall, CopyNew, 0x00434400, (Vector* self, void* edx, uint32_t begin, uint32_t end, uint32_t dest));
CSF_FUNC(uint32_t, __fastcall, Fill, 0x004347D0, (Vector* self, void* edx, uint32_t dest, uint32_t count, const Value* value));
CSF_FUNC(uint32_t, __cdecl, CopySpare, 0x00433F10, (uint32_t begin, uint32_t end, uint32_t dest, Vector* context, uint32_t tag));
CSF_FUNC(uint32_t, __cdecl, CopyBackward, 0x00434480, (uint32_t begin, uint32_t end, uint32_t dest));
CSF_FUNC(uint32_t, __cdecl, Assign, 0x00434420, (uint32_t begin, uint32_t end, const Value* value));
CSF_FUNC(uint32_t, __fastcall, GrowthSize, 0x0042E1E0, (Vector* self, void* edx));
CSF_FUNC(uint32_t, __fastcall, Construct0, 0x004109E0, (void* self, void* edx, uint32_t a0, uint32_t a1, uint32_t a2));
CSF_FUNC(uint32_t, __cdecl, Finish0, 0x00690E60, (uint32_t a0));
CSF_FUNC(uint32_t, __cdecl, DestroyOld, 0x00433B90, (uint32_t begin, uint32_t end, Vector* context, uint32_t tag));
CSF_FUNC(uint32_t, __fastcall, DestroyFailed, 0x00433FC0, (Vector* self, void* edx, uint32_t begin, uint32_t end));
struct Operations {
    using value_type = Value;
    static __forceinline uint32_t split(uint32_t last, uint32_t count) { return last + 4u * (static_cast<uint32_t>(-12) * count); }
    static constexpr bool owns_value = false;
    static __forceinline void construct(Value* dest, const Value* source) { *reinterpret_cast<uint32_t*>(reinterpret_cast<uint8_t*>(dest) + 24) = 0xFu;
        *reinterpret_cast<uint32_t*>(reinterpret_cast<uint8_t*>(dest) + 20) = 0x0u;
        *reinterpret_cast<uint8_t*>(reinterpret_cast<uint8_t*>(dest) + 4) = 0x0u;
        Construct0(reinterpret_cast<uint8_t*>(dest), nullptr, legacy::vc71::address(source), 0x0u, 0xFFFFFFFFu);
        *reinterpret_cast<uint32_t*>(reinterpret_cast<uint8_t*>(dest) + 28) = *reinterpret_cast<const uint32_t*>(reinterpret_cast<const uint8_t*>(source) + 28);
        *reinterpret_cast<uint32_t*>(reinterpret_cast<uint8_t*>(dest) + 32) = *reinterpret_cast<const uint32_t*>(reinterpret_cast<const uint8_t*>(source) + 32);
        *reinterpret_cast<uint32_t*>(reinterpret_cast<uint8_t*>(dest) + 36) = *reinterpret_cast<const uint32_t*>(reinterpret_cast<const uint8_t*>(source) + 36);
        *reinterpret_cast<uint32_t*>(reinterpret_cast<uint8_t*>(dest) + 40) = *reinterpret_cast<const uint32_t*>(reinterpret_cast<const uint8_t*>(source) + 40);
        *reinterpret_cast<uint32_t*>(reinterpret_cast<uint8_t*>(dest) + 44) = *reinterpret_cast<const uint32_t*>(reinterpret_cast<const uint8_t*>(source) + 44); }
    static __forceinline uint32_t release_value(Value*) { return 0; }
    static __forceinline uint32_t copy_new(Vector* self, uint32_t begin, uint32_t end, uint32_t dest) { return CopyNew(self, nullptr, begin, end, dest); }
    static __forceinline uint32_t copy_spare(Vector* self, uint32_t begin, uint32_t end, uint32_t dest) { return CopySpare(begin, end, dest, self, 0); }
    static __forceinline uint32_t fill(Vector* self, uint32_t dest, uint32_t count, const Value* value) { return Fill(self, nullptr, dest, count, value); }
    static __forceinline uint32_t copy_backward(uint32_t begin, uint32_t end, uint32_t dest) { return CopyBackward(begin, end, dest); }
    static __forceinline uint32_t assign(uint32_t begin, uint32_t end, const Value* value) { return Assign(begin, end, value); }
    static __forceinline void destroy_old(Vector* self, uint32_t begin, uint32_t end) { DestroyOld(begin, end, self, 0); }
    static __forceinline void destroy_failed(Vector* self, uint32_t begin, uint32_t end) { DestroyFailed(self, nullptr, begin, end); }
    static __forceinline uint32_t growth_size(Vector* self) { return GrowthSize(self, nullptr); }
static __forceinline uint32_t finish_value(Value* value, uint32_t result) { uint32_t eax = result;
        if (*reinterpret_cast<uint32_t*>(reinterpret_cast<uint8_t*>(value) + 24) < 0x10u) goto L_4355b4;
        eax = *reinterpret_cast<uint32_t*>(reinterpret_cast<uint8_t*>(value) + 4);
        if ((eax & eax) == 0) goto L_4355b4;
        eax = Finish0(eax);
        L_4355b4:;
        return eax;
    }
static __forceinline uint32_t initial_result(Vector* self, uint32_t) {
        return self->first ? (legacy::vc71::address(self->end) - legacy::vc71::address(self->first)) * 0x2AAAAAABu : 0;
    }
static __forceinline uint32_t allocated_result(uint32_t block, uint32_t) { return block; }
};
static uint32_t __fastcall StlInsert_00435290(Vector* self, void*, Value* pos, uint32_t count, const Value* value) {
    return legacy::vc71::vector_insert_n<Operations>(self, pos, count, value);
}
HOOK(0x00435290, StlInsert_00435290, "stl");
