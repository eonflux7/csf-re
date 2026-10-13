#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/vc71_vector.h"

// Binary-derived vector::reserve; survey cluster 26, stride 52.
using Value = legacy::vc71::element<52>;
using Vector = legacy::vc71::vector<Value>;
CSF_FUNC(uint32_t, __fastcall, Copy, 0x00775770, (Vector* self, void* edx, uint32_t begin, uint32_t end, uint32_t dest));
CSF_FUNC(void, __cdecl, Destroy, 0x00775200, (uint32_t begin, uint32_t end));

struct Operations {
    using value_type = Value;
    static constexpr bool returns_last = false;
    static constexpr bool returns_end = false;
    static __forceinline uint32_t copy(Vector* self, uint32_t begin, uint32_t end, uint32_t dest) {
        return Copy(self, nullptr, begin, end, dest);
    }
    static __forceinline void destroy(uint32_t begin, uint32_t end) {
        Destroy(begin, end);
    }
};

static uint32_t __fastcall Replica_007760b0(Vector* self, void*, uint32_t count) {
    return legacy::vc71::vector_reserve<Operations>(self, count);
}

HOOK(0x007760B0, Replica_007760b0, "stl");
