#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/vc71_vector.h"

// Binary-derived vector::reserve; survey cluster 257, stride 16.
using Value = legacy::vc71::element<16>;
using Vector = legacy::vc71::vector<Value>;
CSF_FUNC(uint32_t, __fastcall, Copy, 0x00515E60, (Vector* self, void* edx, uint32_t begin, uint32_t end, uint32_t dest));

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

static uint32_t __fastcall Replica_00516320(Vector* self, void*, uint32_t count) {
    return legacy::vc71::vector_reserve<Operations>(self, count);
}

HOOK(0x00516320, Replica_00516320, "stl");
