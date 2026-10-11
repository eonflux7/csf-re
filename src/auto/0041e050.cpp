#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Game heap allocator (cdecl, caller pops nothing extra: original does `add esp, 4`).
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));

// Constructor-like initializer of the 8-byte binding object; thiscall with one stack arg (ret 4).
CSF_FUNC(uint8_t*, __fastcall, InputBinding_Construct, 0x004022A0, (void* self, void* edx, int a2));

// Vtable of the object created by InputBinding_Construct. Referenced by the original's call site
// (0x41E070); no runtime store happens here, the binding is only kept so the address is bound.
[[maybe_unused]] static auto* const s_InputBindingVtable = CSF_VTBL(0x004022A0);

static char __fastcall C_0082e520_PrepareInputForBinding(C_0082e520* self, void* /*edx*/, int a2, int a3)
{
    (void)a3;

    self->field_0004 = static_cast<uint32_t>(a2);

    void* bindingMemory = Mem_Alloc(8);
    if (bindingMemory != nullptr) {
        // Original reloads field_0004 after the allocation call.
        uint8_t* bindingObject = InputBinding_Construct(bindingMemory, nullptr, static_cast<int>(self->field_0004));
        self->field_0008 = reinterpret_cast<uint32_t>(bindingObject);
    } else {
        self->field_0008 = 0;
    }

    return 1;
}

HOOK(0x0041E050, C_0082e520_PrepareInputForBinding, "auto");
