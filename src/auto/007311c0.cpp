#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/vc71_vector.h"

// Binary-derived vector helper; survey cluster 201, stride 16.
using Value = legacy::vc71::element<16>;
CSF_FUNC(void, __fastcall, Operation, 0x00730EA0, (Value* self, void* edx, const Value* value));
struct Operations {
    using value_type = Value;
    static __forceinline void assign(Value* dest, const Value* source) { Operation(dest, nullptr, source); }
};
static void __cdecl Replica_007311c0(Value* begin, Value* end, const Value* value) {
    legacy::vc71::vector_assign_value<Operations>(begin, end, value);
}

HOOK(0x007311C0, Replica_007311c0, "stl");
