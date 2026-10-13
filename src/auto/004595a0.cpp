#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/vc71_vector.h"
// Binary-derived _Insert_n, cluster None; 40-byte elements.
using Value = legacy::vc71::element<40>;
using Vector = legacy::vc71::vector<Value>;
CSF_FUNC(uint32_t, __fastcall, CopyNew, 0x00458E90, (Vector* self, void* edx, uint32_t begin, uint32_t end, uint32_t dest));
CSF_FUNC(uint32_t, __fastcall, Fill, 0x00459030, (Vector* self, void* edx, uint32_t dest, uint32_t count, const Value* value));
CSF_FUNC(uint32_t, __cdecl, CopySpare, 0x004587A0, (uint32_t begin, uint32_t end, uint32_t dest, Vector* context, uint32_t tag));
CSF_FUNC(uint32_t, __cdecl, CopyBackward, 0x00458610, (uint32_t begin, uint32_t end, uint32_t dest));
CSF_FUNC(uint32_t, __cdecl, Assign, 0x00458320, (uint32_t begin, uint32_t end, const Value* value));
CSF_FUNC(uint32_t, __fastcall, GrowthSize, 0x0043AB40, (Vector* self, void* edx));
CSF_FUNC(uint32_t, __stdcall, DestroyOld, 0x00458E20, (uint32_t begin, uint32_t end));
CSF_FUNC(uint32_t, __stdcall, DestroyFailed, 0x00458E20, (uint32_t begin, uint32_t end));
struct Operations {
    using value_type = Value;
    static __forceinline uint32_t split(uint32_t last, uint32_t count) { return last + 4u * (static_cast<uint32_t>(-10) * count); }
    static constexpr bool owns_value = false;
    static __forceinline void construct(Value* dest, const Value* source) { auto* out = reinterpret_cast<uint32_t*>(dest);
        const auto* in = reinterpret_cast<const uint32_t*>(source);
        out[0] = reinterpret_cast<uint32_t>(CSF_VTBL(0x0082D448));
        out[1] = in[1];
        out[2] = in[2];
        out[3] = in[3];
        out[4] = in[4];
        out[5] = in[5];
        out[6] = in[6];
        out[7] = in[7];
        out[8] = in[8];
        out[9] = in[9];
 }
    static __forceinline uint32_t release_value(Value*) { return 0; }
    static __forceinline uint32_t copy_new(Vector* self, uint32_t begin, uint32_t end, uint32_t dest) { return CopyNew(self, nullptr, begin, end, dest); }
    static __forceinline uint32_t copy_spare(Vector* self, uint32_t begin, uint32_t end, uint32_t dest) { return CopySpare(begin, end, dest, self, 0); }
    static __forceinline uint32_t fill(Vector* self, uint32_t dest, uint32_t count, const Value* value) { return Fill(self, nullptr, dest, count, value); }
    static __forceinline uint32_t copy_backward(uint32_t begin, uint32_t end, uint32_t dest) { return CopyBackward(begin, end, dest); }
    static __forceinline uint32_t assign(uint32_t begin, uint32_t end, const Value* value) { return Assign(begin, end, value); }
    static __forceinline void destroy_old(Vector* self, uint32_t begin, uint32_t end) { DestroyOld(begin, end); }
    static __forceinline void destroy_failed(Vector* self, uint32_t begin, uint32_t end) { DestroyFailed(begin, end); }
    static __forceinline uint32_t growth_size(Vector* self) { return GrowthSize(self, nullptr); }
static __forceinline uint32_t initial_result(Vector* self, uint32_t) {
        return self->first ? (legacy::vc71::address(self->end) - legacy::vc71::address(self->first)) * 0x66666667u : 0;
    }
static __forceinline uint32_t allocated_result(uint32_t block, uint32_t) { return block; }
};
static uint32_t __fastcall StlInsert_004595a0(Vector* self, void*, Value* pos, uint32_t count, const Value* value) {
    return legacy::vc71::vector_insert_n<Operations>(self, pos, count, value);
}
HOOK(0x004595A0, StlInsert_004595a0, "stl");
