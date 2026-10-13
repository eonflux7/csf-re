#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/vc71_vector.h"
// Binary-derived _Insert_n, cluster 28; 200-byte elements.
using Value = legacy::vc71::element<200>;
using Vector = legacy::vc71::vector<Value>;
CSF_FUNC(uint32_t, __fastcall, CopyNew, 0x005134A0, (Vector* self, void* edx, uint32_t begin, uint32_t end, uint32_t dest));
CSF_FUNC(uint32_t, __fastcall, Fill, 0x00513680, (Vector* self, void* edx, uint32_t dest, uint32_t count, const Value* value));
CSF_FUNC(uint32_t, __cdecl, CopySpare, 0x00512AA0, (uint32_t begin, uint32_t end, uint32_t dest, Vector* context, uint32_t tag));
CSF_FUNC(uint32_t, __cdecl, CopyBackward, 0x005134F0, (uint32_t begin, uint32_t end, uint32_t dest));
CSF_FUNC(uint32_t, __cdecl, Assign, 0x005134C0, (uint32_t begin, uint32_t end, const Value* value));
CSF_FUNC(uint32_t, __fastcall, GrowthSize, 0x00511FD0, (Vector* self, void* edx));
CSF_FUNC(uint32_t, __fastcall, Construct0, 0x00512820, (void* self, void* edx, uint32_t a0));
CSF_FUNC(uint32_t, __fastcall, Finish0, 0x00500A80, (void* self, void* edx));
CSF_FUNC(uint32_t, __cdecl, DestroyOld, 0x005129A0, (uint32_t begin, uint32_t end, Vector* context, uint32_t tag));
CSF_FUNC(uint32_t, __fastcall, DestroyFailed, 0x00513480, (Vector* self, void* edx, uint32_t begin, uint32_t end));
struct Operations {
    using value_type = Value;
    static __forceinline uint32_t split(uint32_t last, uint32_t count) { return last + 4u * (static_cast<uint32_t>(-50) * count); }
    static constexpr bool owns_value = false;
    static __forceinline void construct(Value* dest, const Value* source) { Construct0(reinterpret_cast<uint8_t*>(dest), nullptr, legacy::vc71::address(source)); }
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
        uint32_t ecx;
        ecx = legacy::vc71::address(reinterpret_cast<uint8_t*>(value) + 0);
        eax = Finish0(legacy::vc71::pointer<void>(ecx), nullptr);
        return eax;
    }
static __forceinline uint32_t initial_result(Vector* self, uint32_t) {
        return self->first ? (legacy::vc71::address(self->end) - legacy::vc71::address(self->first)) * 0x51EB851Fu : 0;
    }
static __forceinline uint32_t allocated_result(uint32_t block, uint32_t) { return block; }
};
static uint32_t __fastcall StlInsert_005136b0(Vector* self, void*, Value* pos, uint32_t count, const Value* value) {
    return legacy::vc71::vector_insert_n<Operations>(self, pos, count, value);
}
HOOK(0x005136B0, StlInsert_005136b0, "stl");
