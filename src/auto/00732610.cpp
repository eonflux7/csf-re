#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/vc71_vector.h"
using Value = legacy::vc71::element<16>;
using Vector = legacy::vc71::vector<Value>;
CSF_FUNC(uint32_t, __cdecl, CopyNew, 0x007323D0, (uint32_t begin, uint32_t end, uint32_t dest, Vector* context, uint32_t tag));
CSF_FUNC(uint32_t, __cdecl, CopySpare, 0x007323D0, (uint32_t begin, uint32_t end, uint32_t dest, Vector* context, uint32_t tag));
CSF_FUNC(uint32_t, __cdecl, FillNew, 0x00732530, (uint32_t dest, uint32_t count, const Value* value, Vector* context, uint32_t tag));
CSF_FUNC(uint32_t, __cdecl, FillSpare, 0x00732530, (uint32_t dest, uint32_t count, const Value* value, Vector* context, uint32_t tag));

struct Operations {
    using value_type = Value;
    static constexpr bool owns_value = false;
    static __forceinline void construct(Value* dest, const Value* source) {
        legacy::vc71::trivial_assignment<16>::assign(dest, source);
    }
    static __forceinline uint32_t release_value(Value*) { return 0; }
    static __forceinline uint32_t allocated_result(uint32_t block, uint32_t) { return block; }
    static __forceinline uint32_t copy_new(Vector* self, uint32_t begin, uint32_t end, uint32_t dest) { return CopyNew(begin, end, dest, self, 0); }
    static __forceinline uint32_t copy_spare(Vector* self, uint32_t begin, uint32_t end, uint32_t dest) { return CopySpare(begin, end, dest, self, 0); }
    static __forceinline uint32_t fill(Vector* self, uint32_t dest, uint32_t count, const Value* value) { FillNew(dest, count, value, self, 0); return dest + 16 * count; }
    static __forceinline uint32_t fill_spare(Vector* self, uint32_t dest, uint32_t count, const Value* value) { FillSpare(dest, count, value, self, 0); return dest + 16 * count; }
    static __forceinline uint32_t copy_backward(uint32_t begin, uint32_t end, uint32_t dest) { return legacy::vc71::vector_copy_backward<16>(begin, end, dest); }
    static __forceinline uint32_t assign(uint32_t begin, uint32_t end, const Value* value) {
        legacy::vc71::vector_assign_value<legacy::vc71::trivial_assignment<16>>(legacy::vc71::pointer<Value>(begin), legacy::vc71::pointer<Value>(end), value);
        return end;
    }
    static __forceinline uint32_t assign_spare(uint32_t begin, uint32_t end, const Value* value, uint32_t result) {
        while (begin != end) {
            result = begin;
            legacy::vc71::trivial_assignment<16>::assign(legacy::vc71::pointer<Value>(begin), value);
            begin += 16;
        }
        return result;
    }
    static __forceinline void destroy_old(Vector*, uint32_t, uint32_t) {}
    static __forceinline void destroy_failed(Vector*, uint32_t, uint32_t) {}
    static __forceinline void release_failed(void* block, Vector*, uint32_t count) {
        legacy::vc71::release_failed_buffer(block, legacy::vc71::pointer<void>(count));
    }
};
static uint32_t __fastcall StlInsert_00732610(Vector* self, void*, Value* position, uint32_t count, const Value* value) {
    return legacy::vc71::vector_insert_n<Operations>(self, position, count, value);
}
HOOK(0x00732610, StlInsert_00732610, "stl");
