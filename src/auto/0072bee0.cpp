#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/vc71_vector.h"

// Four-byte pointer range insertion. The fourth stack argument is the empty
// dispatch tag (read only by the allocation failure cleanup).
using Value = legacy::vc71::element<4>;
using Vector = legacy::vc71::vector<Value>;
CSF_FUNC(uint32_t, __fastcall, Copy, 0x0072BAD0, (Vector* self, void* edx, uint32_t begin, uint32_t end, uint32_t dest));
CSF_FUNC(uint32_t, __fastcall, CopyRange, 0x0072BB00, (Vector* self, void* edx, uint32_t begin, uint32_t end, uint32_t dest));
CSF_FUNC(uint32_t, __cdecl, CopyBackward, 0x00480DC0, (uint32_t begin, uint32_t end, uint32_t dest));
CSF_FUNC(uint32_t, __cdecl, Assign, 0x0072BB20, (uint32_t begin, uint32_t end, uint32_t dest));
struct Operations {
    using value_type = Value;
    static __forceinline uint32_t copy(Vector* self, uint32_t begin, uint32_t end, uint32_t dest) {
        return Copy(self, nullptr, begin, end, dest);
    }
    static __forceinline uint32_t copy_range(Vector* self, uint32_t begin, uint32_t end, uint32_t dest) {
        return CopyRange(self, nullptr, begin, end, dest);
    }
    static __forceinline void copy_backward(uint32_t begin, uint32_t end, uint32_t dest) {
        CopyBackward(begin, end, dest);
    }
    static __forceinline uint32_t assign(uint32_t begin, uint32_t end, uint32_t dest) {
        return Assign(begin, end, dest);
    }
};
static uint8_t __fastcall Replica_0072bee0(Vector* self, void*, Value* position, Value* begin, Value* end, uint32_t tag) {
    return legacy::vc71::vector_insert_range<Operations>(self, position, begin, end, tag);
}

HOOK(0x0072BEE0, Replica_0072bee0, "stl");
