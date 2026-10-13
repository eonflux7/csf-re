#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pRwEngineInstance, 0x008B68C0);

// One call the original makes with ecx and edx loaded as given and esi = this. The stack words are the callee's
// arguments; hook::call_regs drops them after the call, whether the callee pops them or the caller does.
static uint32_t CallWithRegs(C_00867358* self, uintptr_t fn, uint32_t ecx, uint32_t edx,
                             const uint32_t* stack, unsigned count)
{
    hook::Regs r{};
    r.ecx = ecx;
    r.edx = edx;
    r.esi = reinterpret_cast<uint32_t>(self);
    return hook::call_regs(fn, &r, stack, count);
}

// The original does `mov ecx, [g_pRwEngineInstance]` (and/or edx) right before each render-state call.
static uint32_t EngineValue()
{
    return reinterpret_cast<uint32_t>(g_pRwEngineInstance);
}

static uintptr_t EngineSlot(uint32_t off)
{
    return reinterpret_cast<uintptr_t>(CSF_AT(void*, g_pRwEngineInstance, off));
}

// call [engine + 0x20] with ecx = edx = engine instance, two cdecl stack arguments.
static void RwStateCall(C_00867358* self, uint32_t a0, uint32_t a1)
{
    const uint32_t stack[2] = {a0, a1};
    const uint32_t engine = EngineValue();
    CallWithRegs(self, EngineSlot(0x20), engine, engine, stack, 2);
}

static int __fastcall C_00867358_RenderPrimitives(C_00867358* self, void* /*edx*/)
{
    if (static_cast<int>(self->field_000c) <= 0)
        return static_cast<int>(self->field_000c);

    // mov eax, [g]; push 1; push 0xc; call [eax+0x20]: the first call; ecx is still this from entry.
    const uint32_t firstArgs[2] = {12, 1};
    CallWithRegs(self, EngineSlot(0x20), reinterpret_cast<uint32_t>(self), 0, firstArgs, 2);
    RwStateCall(self, 10, 5);
    RwStateCall(self, 14, 0);

    // mov eax, [esi+0x20]; test; je; mov eax, [eax]: the texture's first dword, or 0 without a texture.
    const uint32_t raster = self->field_0020
        ? CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0020), 0)
        : 0;
    RwStateCall(self, 1, raster);
    RwStateCall(self, 11, 6);
    RwStateCall(self, 9, 2);
    RwStateCall(self, 6, 0);
    RwStateCall(self, 8, 0);
    RwStateCall(self, 20, 1);

    // GetState(2, &saved): mov ecx, [g]; lea eax, [ebp-4]; push eax; push 2; call [ecx+0x24]
    // The prologue's `push ecx` (0x738873) leaves this in [ebp-4]; GetState overwrites it with the saved mode. Start
    // from the same word the original's frame holds, so a GetState that leaves it untouched gives the same value.
    uint32_t savedAddressing = reinterpret_cast<uint32_t>(self);
    const uint32_t getArgs[2] = {2, reinterpret_cast<uint32_t>(&savedAddressing)};
    const uint32_t engine = EngineValue();
    CallWithRegs(self, EngineSlot(0x24), engine, engine, getArgs, 2);

    RwStateCall(self, 2, 3);

    if (static_cast<int>(self->field_0018) > 0)
    {
        // Pushed in order field_18, field_10, field_0c, field_04, field_1c, so the first argument is field_1c.
        // mov edx, [esi+4]; mov ecx, [g]; call [ecx+0x34]: cdecl, the caller pops the five words.
        const uint32_t args[5] = {self->field_001c, self->field_0004, self->field_000c, self->field_0010,
                                  self->field_0018};
        CallWithRegs(self, EngineSlot(0x34), EngineValue(), self->field_0004, args, 5);
    }
    else
    {
        // mov ecx, [esi+0x24]; mov edx, [ecx]; call [edx+0x24]: this = the object, edx = its vtable; the callee
        // pops its three arguments (no add esp after the call).
        void* object = reinterpret_cast<void*>(self->field_0024);
        const uint32_t vtable = CSF_AT(uint32_t, object, 0);
        const uint32_t args[3] = {self->field_001c, self->field_0004, self->field_000c};
        CallWithRegs(self, reinterpret_cast<uintptr_t>(CSF_AT(void**, object, 0)[0x24 / 4]),
                     reinterpret_cast<uint32_t>(object), vtable, args, 3);
    }

    // mov ecx, [ebp-4]; mov edx, [g]; push ecx; push 2; call [edx+0x20]: ecx is the saved value here.
    const uint32_t restoreArgs[2] = {2, savedAddressing};
    return static_cast<int>(CallWithRegs(self, EngineSlot(0x20), savedAddressing, EngineValue(), restoreArgs, 2));
}

HOOK(0x00738870, C_00867358_RenderPrimitives, "auto");
