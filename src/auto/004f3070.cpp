#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/vc71_vector.h"

// Binary-derived vector::reserve; survey cluster 83, stride 124.
using Value = legacy::vc71::element<124>;
using Vector = legacy::vc71::vector<Value>;
CSF_FUNC(uint32_t, __fastcall, Copy, 0x004F2EA0, (Vector* self, void* edx, uint32_t begin, uint32_t end, uint32_t dest));
CSF_FUNC(void, __cdecl, Destroy, 0x004F23A0, (uint32_t begin, uint32_t end));

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

static uint32_t __fastcall Replica_004f3070(Vector* self, void*, uint32_t count) {
    return legacy::vc71::vector_reserve<Operations>(self, count);
}

HOOK(0x004F3070, Replica_004f3070, "stl");
