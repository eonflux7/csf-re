#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/vc71_vector.h"

// Binary-derived vector helper; survey cluster 439, stride 556.
using Value = legacy::vc71::element<556>;
CSF_FUNC(void, __fastcall, Operation, 0x00511040, (Value* self, void* edx));
struct Operations {
    using value_type = Value;
    static __forceinline void destroy(Value* dest) { Operation(dest, nullptr); }
};
static void __stdcall Replica_00762960(Value* begin, Value* end) {
    legacy::vc71::vector_destroy_range<Operations>(begin, end);
}

HOOK(0x00762960, Replica_00762960, "stl");
