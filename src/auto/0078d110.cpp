#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"

// Callee bound by address. Destroys the callable storage embedded at this+8.
CSF_FUNC(void, __fastcall, ReleaseCallableStorage, 0x0068ABC0, (void* self, void* edx));

// std::_Func_impl<lambda, ...>::~_Func_impl (__thiscall, ret 0).
// Sets the vtable to the derived impl, releases the embedded callable at +8,
// then sets the vtable to the base-class vtable.
static int __fastcall NetPerf_FuncImplDestructor(void* self, void* /*edx*/) {
    CSF_AT(void*, self, 0) = CSF_VTBL(0x872D08);
    ReleaseCallableStorage(static_cast<uint8_t*>(self) + 8, nullptr);
    CSF_AT(void*, self, 0) = CSF_VTBL(0x8590A0);
    return 0;
}

HOOK(0x0078D110, NetPerf_FuncImplDestructor, "auto");
