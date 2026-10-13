#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/vc71_vector.h"
// Binary-derived _Insert_n, cluster 27; 16-byte elements.
using Value = legacy::vc71::element<16>;
using Vector = legacy::vc71::vector<Value>;
CSF_FUNC(uint32_t, __fastcall, CopyNew, 0x004DCFA0, (Vector* self, void* edx, uint32_t begin, uint32_t end, uint32_t dest));
CSF_FUNC(uint32_t, __fastcall, Fill, 0x004DC9B0, (Vector* self, void* edx, uint32_t dest, uint32_t count, const Value* value));
CSF_FUNC(uint32_t, __cdecl, CopySpare, 0x004DC880, (uint32_t begin, uint32_t end, uint32_t dest, Vector* context, uint32_t tag));
CSF_FUNC(uint32_t, __cdecl, CopyBackward, 0x006DA2D0, (uint32_t begin, uint32_t end, uint32_t dest));
CSF_FUNC(uint32_t, __cdecl, Assign, 0x0055AA50, (uint32_t begin, uint32_t end, const Value* value));
struct Operations {
    using value_type = Value;
    static __forceinline uint32_t split(uint32_t last, uint32_t count) { return last + 4u * (static_cast<uint32_t>(-4) * count); }
    static constexpr bool owns_value = false;
    static __forceinline void construct(Value* dest, const Value* source) { auto* out = reinterpret_cast<uint32_t*>(dest);
        const auto* in = reinterpret_cast<const uint32_t*>(source);
        out[0] = in[0];
        out[1] = in[1];
        out[2] = in[2];
        out[3] = in[3];
 }
    static __forceinline uint32_t release_value(Value*) { return 0; }
    static __forceinline uint32_t copy_new(Vector* self, uint32_t begin, uint32_t end, uint32_t dest) { return CopyNew(self, nullptr, begin, end, dest); }
    static __forceinline uint32_t copy_spare(Vector* self, uint32_t begin, uint32_t end, uint32_t dest) { return CopySpare(begin, end, dest, self, 0); }
    static __forceinline uint32_t fill(Vector* self, uint32_t dest, uint32_t count, const Value* value) { return Fill(self, nullptr, dest, count, value); }
    static __forceinline uint32_t copy_backward(uint32_t begin, uint32_t end, uint32_t dest) { return CopyBackward(begin, end, dest); }
    static __forceinline uint32_t assign(uint32_t begin, uint32_t end, const Value* value) { return Assign(begin, end, value); }
    static __forceinline void destroy_old(Vector* self, uint32_t begin, uint32_t end) { (void)self; (void)begin; (void)end; }
    static __forceinline void destroy_failed(Vector* self, uint32_t begin, uint32_t end) { (void)self; (void)begin; (void)end; }
    
};
static uint32_t __fastcall StlInsert_004dd270(Vector* self, void*, Value* pos, uint32_t count, const Value* value) {
    return legacy::vc71::vector_insert_n<Operations>(self, pos, count, value);
}
HOOK(0x004DD270, StlInsert_004dd270, "stl");
