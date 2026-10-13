#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/vc71_vector.h"
// Binary-derived _Insert_n, cluster None; 8-byte elements.
using Value = legacy::vc71::element<8>;
using Vector = legacy::vc71::vector<Value>;
CSF_FUNC(uint32_t, __fastcall, CopyNew, 0x006B4E10, (Vector* self, void* edx, uint32_t begin, uint32_t end, uint32_t dest));
CSF_FUNC(uint32_t, __fastcall, Fill, 0x006B4A20, (Vector* self, void* edx, uint32_t dest, uint32_t count, const Value* value));
CSF_FUNC(uint32_t, __cdecl, CopySpare, 0x006B3EA0, (uint32_t begin, uint32_t end, uint32_t dest, Vector* context, uint32_t tag));
CSF_FUNC(uint32_t, __cdecl, CopyBackward, 0x006B3DC0, (uint32_t begin, uint32_t end, uint32_t dest));
CSF_FUNC(uint32_t, __cdecl, Assign, 0x006B3370, (uint32_t begin, uint32_t end, const Value* value));
CSF_FUNC(void*, __cdecl, MatrixCreate, 0x00624D40, ());
CSF_FUNC(uint32_t, __cdecl, MatrixDestroy, 0x00624D10, (void* matrix));
CSF_FUNC(uint32_t, __stdcall, DestroyOld, 0x006B4DC0, (uint32_t begin, uint32_t end));
CSF_FUNC(uint32_t, __stdcall, DestroyFailed, 0x006B4DC0, (uint32_t begin, uint32_t end));
struct Operations {
    using value_type = Value;
    static constexpr bool owns_value = false;
    static __forceinline void construct(Value* dest, const Value* source) { auto* words = reinterpret_cast<uint32_t*>(dest);
        auto* matrix = static_cast<uint32_t*>(MatrixCreate());
        words[0] = legacy::vc71::address(matrix);
        auto* original = *reinterpret_cast<const uint32_t* const*>(source);
        for (uint32_t i = 0; i < 16; ++i) matrix[i] = original[i];
        dest->bytes[4] = 1; }
    static __forceinline uint32_t release_value(Value*) { return 0; }
    static __forceinline uint32_t copy_new(Vector* self, uint32_t begin, uint32_t end, uint32_t dest) { return CopyNew(self, nullptr, begin, end, dest); }
    static __forceinline uint32_t copy_spare(Vector* self, uint32_t begin, uint32_t end, uint32_t dest) { return CopySpare(begin, end, dest, self, 0); }
    static __forceinline uint32_t fill(Vector* self, uint32_t dest, uint32_t count, const Value* value) { return Fill(self, nullptr, dest, count, value); }
    static __forceinline uint32_t copy_backward(uint32_t begin, uint32_t end, uint32_t dest) { return CopyBackward(begin, end, dest); }
    static __forceinline uint32_t assign(uint32_t begin, uint32_t end, const Value* value) { return Assign(begin, end, value); }
    static __forceinline void destroy_old(Vector* self, uint32_t begin, uint32_t end) { DestroyOld(begin, end); }
    static __forceinline void destroy_failed(Vector* self, uint32_t begin, uint32_t end) { DestroyFailed(begin, end); }
    static __forceinline uint32_t finish_value(Value* value, uint32_t result) { uint32_t eax = result;
        return MatrixDestroy(legacy::vc71::pointer<void>(*reinterpret_cast<uint32_t*>(value)));
    }
};
static uint32_t __fastcall StlInsert_006b4f70(Vector* self, void*, Value* pos, uint32_t count, const Value* value) {
    return legacy::vc71::vector_insert_n<Operations>(self, pos, count, value);
}
HOOK(0x006B4F70, StlInsert_006b4f70, "stl");
