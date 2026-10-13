#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/vc71_vector.h"

// VC7.1 vector::_Insert_n, binary clone cluster 11; 16-byte elements.
using Value = legacy::vc71::element<16>;
using Vector = legacy::vc71::vector<Value>;

CSF_FUNC(uint32_t, __fastcall, CopyNew, 0x00468EC0, (Vector* self, void* edx, uint32_t begin, uint32_t end, uint32_t dest));
CSF_FUNC(uint32_t, __fastcall, Fill, 0x00469200, (Vector* self, void* edx, uint32_t dest, uint32_t count, const Value* value));
CSF_FUNC(uint32_t, __cdecl, CopySpare, 0x00468970, (uint32_t begin, uint32_t end, uint32_t dest, Vector* context, uint32_t unused));
CSF_FUNC(uint32_t, __cdecl, CopyBackward, 0x00441AE0, (uint32_t begin, uint32_t end, uint32_t dest));
CSF_FUNC(uint32_t, __cdecl, Assign, 0x00441B00, (uint32_t begin, uint32_t end, const Value* value));
CSF_FUNC(Value*, __fastcall, Construct, 0x004419F0, (Value* dest, void* edx, const Value* source));
CSF_FUNC(uint32_t, __cdecl, DestroyOld, 0x00468AF0, (uint32_t begin, uint32_t end, Vector* context, uint32_t unused));
CSF_FUNC(uint32_t, __fastcall, DestroyFailed, 0x00441EC0, (Vector* self, void* edx, uint32_t begin, uint32_t end));
CSF_FUNC(uint32_t, __cdecl, ReleaseValue, 0x00691110, (void* block));

struct Operations {
    using value_type = Value;
    static constexpr bool owns_value = true;
    static __forceinline void construct(Value* dest, const Value* source) {
        Construct(dest, nullptr, source);
    }
    static __forceinline uint32_t release_value(Value* value) {
        uint32_t owner = reinterpret_cast<uint32_t*>(value)[0];
        return owner ? ReleaseValue(reinterpret_cast<void*>(owner)) : owner;
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
        DestroyOld(begin, end, self, 0);
    }
    static __forceinline void destroy_failed(Vector* self, uint32_t begin, uint32_t end) {
        DestroyFailed(self, nullptr, begin, end);
    }
};

static uint32_t __fastcall StlVectorInsert_00469d10(Vector* self, void*, Value* position, uint32_t count, const Value* value) {
    return legacy::vc71::vector_insert_n<Operations>(self, position, count, value);
}

HOOK(0x00469D10, StlVectorInsert_00469d10, "stl");
