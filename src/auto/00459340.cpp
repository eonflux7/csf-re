#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/vc71_vector.h"

// Binary-derived vector::reserve; survey cluster 86, stride 40.
using Value = legacy::vc71::element<40>;
using Vector = legacy::vc71::vector<Value>;
CSF_FUNC(uint32_t, __fastcall, Copy, 0x00458E90, (Vector* self, void* edx, uint32_t begin, uint32_t end, uint32_t dest));
CSF_FUNC(void, __stdcall, Destroy, 0x00458E20, (uint32_t begin, uint32_t end));

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

static uint32_t __fastcall Replica_00459340(Vector* self, void*, uint32_t count) {
    return legacy::vc71::vector_reserve<Operations>(self, count);
}

HOOK(0x00459340, Replica_00459340, "stl");
