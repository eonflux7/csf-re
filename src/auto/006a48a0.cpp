#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

using CodeLengthOrder = std::uint8_t[19];
CSF_GLOBAL(CodeLengthOrder, g_CodeLengthOrder, 0x0085EF94);

// build_bl_tree: the deflate state arrives in eax; the result is the index of the last used bit-length code.
static std::uint32_t __cdecl CoreZlibLog_BuildBitLengthTree(hook::Regs* regs, const std::uint32_t* /*args*/)
{
    auto* state = reinterpret_cast<std::uint8_t*>(regs->eax);

    // scan_tree: eax = tree, ecx = its max code, stack = state (the caller pops)
    hook::Regs r{};
    r.esi = reinterpret_cast<std::uint32_t>(state);
    std::uint32_t stack[1] = {reinterpret_cast<std::uint32_t>(state)};
    r.eax = reinterpret_cast<std::uint32_t>(state + 0x8C);
    r.ecx = CSF_AT(std::uint32_t, state, 0xB14);
    hook::call_regs(CSF_ADDR_FN(0x006A3680), &r, stack, 1);
    r.eax = reinterpret_cast<std::uint32_t>(state + 0x980);
    r.ecx = CSF_AT(std::uint32_t, state, 0xB20);
    hook::call_regs(CSF_ADDR_FN(0x006A3680), &r, stack, 1);

    // build_tree: esi = state, stack = the bit-length tree descriptor
    r.eax = reinterpret_cast<std::uint32_t>(state + 0xB28);
    stack[0] = r.eax;
    hook::call_regs(CSF_ADDR_FN(0x006A46A0), &r, stack, 1);

    int index = 18;
    for (; index >= 3; --index)
    {
        if (CSF_AT(std::uint16_t, state, 4 * g_CodeLengthOrder[index] + 0xA76) != 0)
            break;
    }
    CSF_AT(std::uint32_t, state, 0x16A0) += 3 * index + 0x11;
    return static_cast<std::uint32_t>(index);
}

HOOK_REGS(0x006A48A0, CoreZlibLog_BuildBitLengthTree, 0, "auto");
