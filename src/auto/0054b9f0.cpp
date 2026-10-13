#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/vc71_vector.h"

// VC7.1 vector::_Insert_n, binary clone cluster 4; 8-byte elements.
using Value = legacy::vc71::element<8>;
using Vector = legacy::vc71::vector<Value>;

CSF_FUNC(uint32_t, __fastcall, CopyNew, 0x0054B3A0, (Vector* self, void* edx, uint32_t begin, uint32_t end, uint32_t dest));
CSF_FUNC(uint32_t, __fastcall, Fill, 0x0054B420, (Vector* self, void* edx, uint32_t dest, uint32_t count, const Value* value));
CSF_FUNC(uint32_t, __cdecl, CopySpare, 0x0054AF40, (uint32_t begin, uint32_t end, uint32_t dest, Vector* context, uint32_t unused));
CSF_FUNC(uint32_t, __cdecl, CopyBackward, 0x004DC700, (uint32_t begin, uint32_t end, uint32_t dest));
CSF_FUNC(uint32_t, __cdecl, Assign, 0x0070CE20, (uint32_t begin, uint32_t end, const Value* value));

struct Operations {
    using value_type = Value;
    static constexpr bool owns_value = false;
    static __forceinline void construct(Value* dest, const Value* source) {
        auto* out = reinterpret_cast<uint32_t*>(dest);
        const auto* in = reinterpret_cast<const uint32_t*>(source);
        out[0] = in[0];
        out[1] = in[1];
    }
    static __forceinline uint32_t release_value(Value* value) {
        (void)value;
        return 0;
    }
    static __forceinline uint32_t copy_new(Vector* self, uint32_t begin, uint32_t end, uint32_t dest) {
        return CopyNew(self, nullptr, begin, end, dest);
    }
    static __forceinline uint32_t copy_spare(Vector* self, uint32_t begin, uint32_t end, uint32_t dest) {
        return CopySpare(begin, end, dest, self, 0);
    }
    static __forceinline uint32_t fill(Vector* self, uint32_t dest, uint32_t count, const Value* value) {
        return Fill(self, nullptr, dest, count, value);
    }
    static __forceinline uint32_t copy_backward(uint32_t begin, uint32_t end, uint32_t dest) {
        return CopyBackward(begin, end, dest);
    }
    static __forceinline uint32_t assign(uint32_t begin, uint32_t end, const Value* value) {
        return Assign(begin, end, value);
    }
    static __forceinline void destroy_old(Vector* self, uint32_t begin, uint32_t end) {
        (void)self; (void)begin; (void)end;
    }
    static __forceinline void destroy_failed(Vector* self, uint32_t begin, uint32_t end) {
        (void)self; (void)begin; (void)end;
    }
};

static uint32_t __fastcall StlVectorInsert_0054b9f0(Vector* self, void*, Value* position, uint32_t count, const Value* value) {
    return legacy::vc71::vector_insert_n<Operations>(self, position, count, value);
}

HOOK(0x0054B9F0, StlVectorInsert_0054b9f0, "stl");
