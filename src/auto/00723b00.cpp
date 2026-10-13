#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/vc71_vector.h"

// Binary-derived vector::copy_construct; survey cluster 116, stride 60.
using Value = legacy::vc71::element<60>;
using Vector = legacy::vc71::vector<Value>;
CSF_FUNC(uint32_t, __fastcall, Copy, 0x00723930, (Vector* self, void* edx, uint32_t begin, uint32_t end, uint32_t dest));

CSF_FUNC(void, __fastcall, Cleanup, 0x00723750, (Vector* self, void* edx));

struct Operations {
    using value_type = Value;
    static __forceinline void cleanup(Vector* self) { Cleanup(self, nullptr); }
    static constexpr bool returns_last = false;
    static constexpr bool returns_end = false;
    static __forceinline uint32_t copy(Vector* self, uint32_t begin, uint32_t end, uint32_t dest) {
        return Copy(self, nullptr, begin, end, dest);
    }
    static __forceinline void destroy(uint32_t begin, uint32_t end) {
        (void)begin; (void)end;
    }
};

static Vector* __fastcall Replica_00723b00(Vector* self, void*, const Vector* source) {
    return legacy::vc71::vector_copy_construct<Operations>(self, source);
}

HOOK(0x00723B00, Replica_00723b00, "stl");
