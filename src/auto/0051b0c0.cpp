#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/vc71_vector.h"
// Binary-derived _Insert_n, cluster None; 1-byte elements.
using Value = uint8_t;
using Vector = legacy::vc71::vector<Value>;
CSF_FUNC(uint32_t, __fastcall, CopyNew, 0x0051AEE0, (Vector* self, void* edx, uint32_t begin, uint32_t end, uint32_t dest));
CSF_FUNC(uint32_t, __fastcall, Fill, 0x0051AF50, (Vector* self, void* edx, uint32_t dest, uint32_t count, const Value* value));
CSF_FUNC(uint32_t, __cdecl, CopyBackward, 0x007AEE60, (uint32_t begin, uint32_t end, uint32_t dest));
CSF_FUNC(uint32_t, __cdecl, Assign, 0x0051AD90, (uint32_t begin, uint32_t end, const Value* value));
struct Operations {
    using value_type = Value;
    static constexpr bool owns_value = false;
    static __forceinline void construct(Value* dest, const Value* source) { dest[0] = source[0]; }
    static __forceinline uint32_t release_value(Value*) { return 0; }
    static __forceinline uint32_t copy_new(Vector* self, uint32_t begin, uint32_t end, uint32_t dest) { return CopyNew(self, nullptr, begin, end, dest); }
    static __forceinline uint32_t copy_spare(Vector* self, uint32_t begin, uint32_t end, uint32_t dest) { return CopyNew(self, nullptr, begin, end, dest); }
    static __forceinline uint32_t fill(Vector* self, uint32_t dest, uint32_t count, const Value* value) { return Fill(self, nullptr, dest, count, value); }
    static __forceinline uint32_t copy_backward(uint32_t begin, uint32_t end, uint32_t dest) { return CopyBackward(begin, end, dest); }
    static __forceinline uint32_t assign(uint32_t begin, uint32_t end, const Value* value) { return Assign(begin, end, value); }
    static __forceinline void destroy_old(Vector* self, uint32_t begin, uint32_t end) { (void)self; (void)begin; (void)end; }
    static __forceinline void destroy_failed(Vector* self, uint32_t begin, uint32_t end) { (void)self; (void)begin; (void)end; }
    
};
static uint32_t __fastcall StlInsert_0051b0c0(Vector* self, void*, Value* pos, uint32_t count, const Value* value) {
    return legacy::vc71::vector_insert_n<Operations>(self, pos, count, value);
}
HOOK(0x0051B0C0, StlInsert_0051b0c0, "stl");
