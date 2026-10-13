#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/vc71_vector.h"

// Binary-derived vector::reserve; survey cluster 20, stride 4.
using Value = legacy::vc71::element<4>;
using Vector = legacy::vc71::vector<Value>;
CSF_FUNC(uint32_t, __fastcall, Copy, 0x006E6320, (Vector* self, void* edx, uint32_t begin, uint32_t end, uint32_t dest));

struct Operations {
    using value_type = Value;
    static constexpr bool returns_last = false;
    static constexpr bool returns_end = true;
    static __forceinline uint32_t copy(Vector* self, uint32_t begin, uint32_t end, uint32_t dest) {
        return Copy(self, nullptr, begin, end, dest);
    }
    static __forceinline void destroy(uint32_t begin, uint32_t end) {
        (void)begin; (void)end;
    }
};

static uint32_t __fastcall Replica_006e66d0(Vector* self, void*, uint32_t count) {
    return legacy::vc71::vector_reserve<Operations>(self, count);
}

HOOK(0x006E66D0, Replica_006e66d0, "stl");
