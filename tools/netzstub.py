#!/usr/bin/env python3
"""Generate conservative standalone offline NetZ boundary stubs; never edit export/ or db/.

First run: --report work/linktest.json (freeze the original unresolved address set).
Subsequent runs use src/boundary/netz/targets.json; --check tests reproducibility.
Unknown pointer/ID-returning functions and unaudited constructors deliberately stay
unresolved. manifest.json contains every target, including deferred ones and callers.
"""
import argparse
import json
from pathlib import Path
import re
import struct
import subprocess

import api
import packet

ROOT = Path(__file__).resolve().parent.parent
OUT = ROOT / "src/boundary/netz"
ALLOC = {0x647070, 0x6470E0, 0x647140, 0x647160, 0x6471C0}
STRINGS = {0x64A690, 0x64A6B0, 0x64A6F0, 0x64A710}
CTOR = 0x798DF0
# Reviewed offline layouts/ownership, kept separate from name-based policy.
SUPPORT_NAMES = {
    0x646FF0: 'allocator_token',
    0x656DE0: 'list_head', 0x653E90: 'plain_list_head',
    0x6524B0: 'list_node', 0x652810: 'list_clear_strings',
    0x6508A0: 'list_clear_plain', 0x651160: 'list_count',
    0x66FBB0: 'empty_string_output', 0x64A8F0: 'empty_format_output',
    0x657590: 'offline_result',
    0x798E40: 'gathering_destruct', 0x798E60: 'gathering_type',
    0x798E80: 'gathering_name', 0x66A440: 'request_construct',
    0x651770: 'connection_construct',
    0x66F130: 'session_string_construct', 0x66F150: 'session_string_destruct',
    0x66F180: 'session_string_assign',
    0x6486C0: 'primary_endpoint', 0x6486D0: 'secondary_endpoint',
}
INIT_SUPPORT = {
    0x646E10: 'thread_map_construct', 0x646EF0: 'thread_map_destruct',
    0x647200: 'value_construct', 0x647220: 'value_destruct',
    0x6474D0: 'counter_sample', 0x647840: 'context_dword_647840',
    0x648FA0: 'critical_construct', 0x648FB0: 'critical_destruct',
    0x649AC0: 'thread_value_construct', 0x649BA0: 'thread_value_destruct',
    0x64A290: 'stopwatch_destruct', 0x64A310: 'stopwatch_construct',
    0x651D70: 'registry_base_construct', 0x651D80: 'registry_base_destruct',
    0x6523A0: 'context_dword_6523a0', 0x652680: 'context_pair_construct',
    0x653730: 'registry_construct', 0x653740: 'registry_destruct',
    0x653750: 'registry_append', 0x6537B0: 'registry_unlink',
    0x653840: 'settings_destruct', 0x657810: 'context_word_657810',
    0x658610: 'context_value_destruct', 0x6587B0: 'context_value_construct',
    0x668450: 'list_factory_668450', 0x6694A0: 'context_list_destruct',
    0x669FE0: 'context_list_construct', 0x66B390: 'tree_factory_66b390',
    0x66BBE0: 'tree_range_66bbe0', 0x66D400: 'list_factory_66d400',
    0x672620: 'settings_construct', 0x6727A0: 'factory_destruct',
    0x672960: 'factory_construct', 0x67AC40: 'context_dword_67ac40',
    0x67CFE0: 'random_construct', 0x67D010: 'root_construct',
    0x67D030: 'root_destruct', 0x67D100: 'reference_construct',
    0x686C10: 'provider_get', 0x6882D0: 'list_factory_6882d0',
    0x68ECE0: 'timeout_construct', 0x68F540: 'interpolation_construct',
    0x796490: 'list_factory_796490', 0x79C090: 'list_factory_79c090',
    0x7AA3C0: 'tree_factory_7aa3c0', 0x7AA730: 'tree_range_7aa730',
}
SUPPORT_NAMES.update(INIT_SUPPORT)
WINMAIN_SUPPORT = {
    0x6684F0: 'session_container_head', 0x6687B0: 'session_container_clear',
    0x670020: 'session_embedded_string', 0x671260: 'runtime_state',
    0x652020: 'clump_manager_get', 0x652160: 'object_manager_get',
    0x7A7B50: 'constant_enabled', 0x646D30: 'status_notice',
    0x64AD00: 'class_index_lookup', 0x64B250: 'selected_object_test',
    0x64B9B0: 'selection_construct', 0x651C70: 'selection_link_reset',
    0x651D00: 'selection_link_id', 0x65E250: 'selection_context',
    0x662BA0: 'selection_sentinel', 0x668AC0: 'callback_unregister',
    0x7A8660: 'singleton_lookup',
    0x669B40: 'callback_register_reject',
    # Existing result-only policies were insufficient for iterator ownership.
    0x65E130: 'selection_assign', 0x651B40: 'selection_link_release',
    # Original seven-slot selection-sentinel table: independently audited ABI.
    0x662BD0: 'selection_delete', 0x64FE50: 'selection_addref',
    0x64FE90: 'selection_release', 0x662C30: 'selection_reject_id',
    0x662C00: 'selection_reject_object', 0x662C60: 'selection_first',
    0x662D10: 'selection_last',
}
SUPPORT_NAMES.update(WINMAIN_SUPPORT)
AUDITED_FAILURES = {0x799220, 0x798FB0, 0x798FF0, 0x79FF30,
                    0x648770, 0x658240, 0x68F740, 0x676700,
                    0x6575D0, 0x799020}


def cleanup(ea):
    """Follow binary control flow, including tail thunks; ignore unreachable padding.

    No guessed uniform virtual ABI: fail closed on indirect tails or different ret N.
    Calls don't transfer to their target (only the fallthrough is ours).
    """
    todo, seen, pops = [ea], set(), set()
    while todo:
        pc = todo.pop()
        if pc in seen:
            continue
        if len(seen) > 8192 or not 0x401000 <= pc < 0x828000:
            raise ValueError("unbounded/out-of-code CFG")
        seen.add(pc)
        ins = packet.disasm(pc, 15, linear=True)
        if not ins:
            raise ValueError("no instruction")
        i = ins[0]
        if i.mnemonic == "ret":
            pops.add(int(i.op_str or "0", 0))
        elif i.mnemonic.startswith("j"):
            if not re.fullmatch(r"0x[0-9a-f]+", i.op_str):
                raise ValueError("indirect tail: cleanup unknown")
            todo.append(int(i.op_str, 16))
            if i.mnemonic != "jmp":
                todo.append(pc + i.size)
        elif i.mnemonic in ("int3", "ud2", "hlt"):
            raise ValueError("non-returning path")
        else:
            todo.append(pc + i.size)
    if len(pops) != 1:
        raise ValueError(f"nonuniform ret: {sorted(pops)}")
    return pops.pop()


def virtuals():
    # Exact table extent: eight code pointers followed by the MatchMakingClient string.
    packet.disasm(CTOR, 1)  # initialize the read-only original image cache
    targets = struct.unpack_from("<8I", packet._img, 0x874E80 - 0x400000)
    result = [(a, cleanup(a)) for a in targets]
    if [n for _, n in result[:3]] != [4, 0, 4]:
        raise ValueError("audited Gathering delete/factory/name ABI changed")
    return result


def family(a, contract, abi):
    name, ret = contract.get("name", ""), contract.get("ret", "")
    if a in SUPPORT_NAMES:
        return 'audited-offline-support', None, 'see audit.json: ' + SUPPORT_NAMES[a]
    if a in AUDITED_FAILURES:
        return 'audited-offline-rejection', 0, 'caller tests byte result or ignores endpoint setter result; see audit.json'
    if a in ALLOC:
        return "allocator", None, "slot supplied by boundary/alloc/netz_heap.cpp"
    if a in STRINGS:
        return "owned-string", None, "length-prefixed string on the game heap"
    if a == CTOR:
        return "audited-constructor", None, "0x30-byte offline base, eight ABI-specific virtuals"
    if name.startswith("ov_"):
        return "deferred", None, "Vorbis audio import, not an offline network operation"
    if re.search(r"Construct|Initializ|_Init|ResetBaseState", name):
        return "deferred", None, "constructor/layout/embedded ownership needs audit"
    if a == 0x64B110:
        return "offline-name", None, "static empty C string, not an owned NetZ String"
    if "*" in ret or "*" in abi.get("ret", ""):
        return "deferred", None, "pointer result may be dereferenced; no universal fake object"
    if not re.search(r"Net|Session|Spawn|Gathering|Prudp|Channel", name):
        return "deferred", None, "unnamed or potentially shared/non-network helper"
    if ret == "void":
        return "offline-noop", 0, "no callbacks, transport side effects, or state mutation"
    if ret in ("bool", "char", "unsigned char", "uint8_t", "std::uint8_t") or abi.get("ret") in ("bool", "char", "unsigned __int8"):
        return "offline-false", 0, "not connected / request rejected / predicate false"
    if re.search(r"GetLastResultCode|FailureResult", name):
        return "offline-error", -1, "nonzero error (not a successful status)"
    if re.search(r"Count|IsValid|IsDuplicate|IsReferenceValid|Matches|_Test|CheckGathering|WaitFor|IsSession", name):
        return "offline-zero", 0, "zero count / false predicate"
    return "deferred", None, "integer may encode pointer, ID, or status; failure not audited"


SUPPORT = r'''
#include "core/memory.h"
#include <cstring>
#include <limits>
namespace {
static void string_init(void* self, const char* text) {
    char* result = nullptr;
    if (text) {
        size_t n = std::strlen(text) + 1;
        if (n <= std::numeric_limits<size_t>::max() - 4) {
            auto* base = static_cast<unsigned char*>(game::Mem_Alloc(n + 4));
            if (base) {
                *reinterpret_cast<uint32_t*>(base) = static_cast<uint32_t>(n);
                result = reinterpret_cast<char*>(base + 4);
                std::memcpy(result, text, n);
            }
        }
    }
    *static_cast<char**>(self) = result;
}
static uint32_t __cdecl string_default(hook::Regs* r, const uint32_t*) {
    string_init(reinterpret_cast<void*>(r->ecx), CSF_STR(0x00858400, "")); return r->ecx;
}
static uint32_t __cdecl string_construct(hook::Regs* r, const uint32_t* a) {
    string_init(reinterpret_cast<void*>(r->ecx), reinterpret_cast<const char*>(a[0])); return r->ecx;
}
static void string_destroy(void* self) {
    char* p = *static_cast<char**>(self);
    if (p) game::Mem_Free(p - 4);
    *static_cast<char**>(self) = nullptr;
}
static uint32_t __cdecl string_destruct(hook::Regs* r, const uint32_t*) {
    string_destroy(reinterpret_cast<void*>(r->ecx)); return 0;
}
static uint32_t __cdecl string_assign(hook::Regs* r, const uint32_t* a) {
    void* self = reinterpret_cast<void*>(r->ecx);
    const char* text = reinterpret_cast<const char*>(a[0]);
    if (*static_cast<const char**>(self) != text) { string_destroy(self); string_init(self, text); }
    return r->ecx;
}
static uint32_t __cdecl session_string_assign(hook::Regs* r, const uint32_t* a) {
    const char* text = *reinterpret_cast<const char**>(a[0]);
    void* out = reinterpret_cast<void*>(r->ecx);
    if (*static_cast<const char**>(out) != text) { string_destroy(out); string_init(out, text); }
    return r->ecx;
}
static uint32_t __cdecl session_string_construct(hook::Regs* r, const uint32_t* a) {
    return string_construct(r, a);
}
static uint32_t __cdecl session_string_destruct(hook::Regs* r, const uint32_t* a) {
    return string_destruct(r, a);
}
static uint32_t __cdecl station_name(hook::Regs*, const uint32_t*) {
    return reinterpret_cast<uintptr_t>("");
}
// The alloc boundary ignores this token; no original allocator virtuals are used.
static uint32_t __cdecl allocator_token(hook::Regs*, const uint32_t*) {
    static uint32_t token = 0; return reinterpret_cast<uintptr_t>(&token);
}
static uint32_t __cdecl list_head(hook::Regs*, const uint32_t*) {
    auto* p = static_cast<uint32_t*>(game::Mem_Alloc(12));
    if (p) { p[0] = p[1] = reinterpret_cast<uintptr_t>(p); p[2] = 0; }
    return reinterpret_cast<uintptr_t>(p);
}
static uint32_t __cdecl plain_list_head(hook::Regs* r, const uint32_t* a) { return list_head(r, a); }
static uint32_t __cdecl list_node(hook::Regs*, const uint32_t* a) {
    auto* p = static_cast<uint32_t*>(game::Mem_Alloc(12));
    if (p) {
        p[0] = a[0]; p[1] = a[1];
        string_init(p + 2, ""); // offline URL payload, caller links the node
    }
    return reinterpret_cast<uintptr_t>(p);
}
static uint32_t list_clear(hook::Regs* r, bool strings) {
    auto* self = reinterpret_cast<uint32_t*>(r->ecx);
    auto* head = reinterpret_cast<uint32_t*>(self[1]);
    auto* node = reinterpret_cast<uint32_t*>(head[0]);
    head[0] = head[1] = reinterpret_cast<uintptr_t>(head); self[2] = 0;
    while (node != head) {
        auto* next = reinterpret_cast<uint32_t*>(node[0]);
        if (strings) string_destroy(node + 2);
        game::Mem_Free(node); node = next;
    }
    return reinterpret_cast<uintptr_t>(head);
}
static uint32_t __cdecl list_clear_strings(hook::Regs* r, const uint32_t*) { return list_clear(r, true); }
static uint32_t __cdecl list_clear_plain(hook::Regs* r, const uint32_t*) { return list_clear(r, false); }
static uint32_t __cdecl list_count(hook::Regs* r, const uint32_t* a) {
    reinterpret_cast<uint32_t*>(r->ecx)[2] += a[0]; return 0;
}
static uint32_t __cdecl empty_string_output(hook::Regs*, const uint32_t* a) {
    string_init(reinterpret_cast<void*>(a[0]), ""); return a[0];
}
static uint32_t __cdecl empty_format_output(hook::Regs*, const uint32_t* a) {
    void* out = reinterpret_cast<void*>(a[0]); string_destroy(out); string_init(out, "");
    // Original has no defined EAX result; game callers ignore it.
    return a[0];
}
static uint32_t __cdecl offline_result(hook::Regs* r, const uint32_t*) {
    auto* p = reinterpret_cast<uint32_t*>(r->ecx);
    p[0] = 0x80010001u; p[1] = reinterpret_cast<uintptr_t>("offline"); p[2] = 0;
    return r->ecx;
}
static uint32_t __cdecl gathering_destruct(hook::Regs* r, const uint32_t*) {
    auto* p = reinterpret_cast<unsigned char*>(r->ecx);
    string_destroy(p + 0x28); string_destroy(p + 0x24); return r->ecx;
}
static uint32_t __cdecl gathering_type(hook::Regs* r, const uint32_t* a) {
    auto* p = reinterpret_cast<uint32_t*>(r->ecx); p[5] = a[0]; p[6] = a[1]; return 0;
}
static uint32_t __cdecl gathering_name(hook::Regs* r, const uint32_t* a) {
    void* out = reinterpret_cast<unsigned char*>(r->ecx) + 0x24;
    const char* text = *reinterpret_cast<const char**>(a[0]);
    if (*static_cast<const char**>(out) != text) { string_destroy(out); string_init(out, text); }
    return reinterpret_cast<uintptr_t>(out);
}
} // namespace
'''


def generate(targets):
    contracts, abis = api.load(), api.ida_abi()
    audit = json.loads((OUT / 'audit.json').read_text())
    callers = {a: [] for a in targets}
    for p in sorted((ROOT / "src/auto").glob("*.cpp")):
        text = p.read_text()
        for m in api.DECL.finditer(api.strip_comments(text)):
            a = int(m.group(4), 16)
            if a in callers:
                owner = re.findall(r'HOOK(?:_REGS)?\([^,]+,\s*(\w+)', text)
                uses = [{'line': i + 1, 'text': line.strip()}
                        for i, line in enumerate(text.splitlines())
                        if re.search(r'\b' + re.escape(m.group(3)) + r'\s*\(', line)
                        and 'CSF_FUNC' not in line]
                callers[a].append({"file": str(p.relative_to(ROOT)), "name": m.group(3),
                                   "ret": api.ws(m.group(1)), "conv": m.group(2), "params": api.ws(m.group(5)),
                                   'caller_functions': owner, 'uses': uses})
        # Address-only callbacks were missed by the first generator's CSF_FUNC scan.
        for m in re.finditer(r'CSF_ADDR_FN\(\s*(0x[0-9a-fA-F]+)\s*\)', text):
            a = int(m.group(1), 16)
            if a in callers:
                callers[a].append({'file': str(p.relative_to(ROOT)), 'name': 'address-only callback',
                    'ret': '', 'conv': '', 'params': '',
                    'caller_functions': re.findall(r'HOOK\([^,]+,\s*(\w+)', text),
                    'uses': [{'line': text[:m.start()].count('\n') + 1, 'text': m.group(0)}]})
    manifest, lines = [], ["// Generated by tools/netzstub.py. Do not edit.", "#ifdef CSF_STANDALONE", '#include "globals.h"', '#include "hook.h"', SUPPORT, '#include "init_support.h"', '#include "winmain_support.h"']
    vt = virtuals()
    lines += ["static void gathering_init(void*);",
              "static void* __fastcall gathering_delete(void* self, void*, unsigned flags) {",
              "    string_destroy(static_cast<unsigned char*>(self) + 0x28);",
              "    string_destroy(static_cast<unsigned char*>(self) + 0x24);",
              "    if (flags & 1) game::Mem_Free(self); return self;", "}",
              "static void* __fastcall gathering_factory(void*, void*) {",
              "    void* p = game::Mem_Alloc(0x30); if (p) gathering_init(p); return p;", "}",
              "static void* __fastcall gathering_class_name(void*, void*, void* out) {",
              '    string_init(out, CSF_STR(0x00875080, "Gathering")); return out;', "}"]
    for n, (a, pop) in enumerate(vt):
        if n < 3:
            continue
        lines += [f"// Virtual slot {n}, original target 0x{a:08X}; binary ret {pop}.",
                  f"static __declspec(naked) void offline_virtual_{n}() {{",
                  "    __asm xor eax, eax", f"    __asm ret {pop}", "}"]
    methods = ["gathering_delete", "gathering_factory", "gathering_class_name"] + [f"offline_virtual_{n}" for n in range(3, len(vt))]
    lines += ["static void* const offline_gathering_vtable[] = {" + ", ".join(f"(void*)&{m}" for m in methods) + "};",
              "static void gathering_init(void* p) {",
              "    auto* self = static_cast<unsigned char*>(p);",
              "    std::memset(self, 0, 0x30);",
              "    *reinterpret_cast<void* const**>(self) = offline_gathering_vtable;",
              "    *reinterpret_cast<uint32_t*>(self + 0x14) = 1;",
              '    string_init(self + 0x24, ""); string_init(self + 0x28, "");',
              "}",
              "static uint32_t __cdecl gathering_construct(hook::Regs* r, const uint32_t*) { gathering_init(reinterpret_cast<void*>(r->ecx)); return r->ecx; }"]
    support = {0x64A690: "string_default", 0x64A6B0: "string_construct", 0x64A6F0: "string_destruct", 0x64A710: "string_assign", CTOR: "gathering_construct", 0x64B110: "station_name"}
    support.update(SUPPORT_NAMES)
    # Six-slot request vtable, followed by a non-code string in the image.
    request_vt = struct.unpack_from('<6I', packet._img, 0x85B020 - 0x400000)
    request_pops = [cleanup(a) for a in request_vt]
    after_request = struct.unpack_from('<I', packet._img, 0x85B038 - 0x400000)[0]
    if request_pops != [4, 0, 0, 0, 8, 0] or 0x401000 <= after_request < 0x828000:
        raise ValueError('audited six-slot request table extent/ABI changed')
    lines += ['static void* __fastcall request_delete(void* p, void*, unsigned flags) {',
              '    if (flags & 1) game::Mem_Free(p); return p;', '}']
    for i in range(1, 6):
        lines += [f'static __declspec(naked) void request_virtual_{i}() {{',
                  '    __asm xor eax, eax', f'    __asm ret {request_pops[i]}', '}']
    lines += ['static void* const offline_request_vtable[] = {' +
              ', '.join('(void*)&' + n for n in ['request_delete'] + [f'request_virtual_{i}' for i in range(1, 6)]) + '};',
              'static uint32_t __cdecl request_construct(hook::Regs* r, const uint32_t*) {',
              '    auto* p = reinterpret_cast<uint32_t*>(r->ecx); std::memset(p, 0, 0x70);',
              '    p[0] = reinterpret_cast<uintptr_t>(offline_request_vtable);',
              '    p[3] = 3; /* completed failure, never pending (1) or success (2) */',
              '    p[17] = 0x80010001u; p[18] = reinterpret_cast<uintptr_t>("offline"); return r->ecx;', '}']
    # Base connection is 92 bytes, not the derived request's 112 bytes.
    connection_vt = struct.unpack_from('<6I', packet._img, 0x858C3C - 0x400000)
    connection_pops = [cleanup(a) for a in connection_vt]
    after_connection = struct.unpack_from('<I', packet._img, 0x858C54 - 0x400000)[0]
    if connection_pops != [4, 0, 0, 0, 8, 0] or 0x401000 <= after_connection < 0x828000:
        raise ValueError('audited six-slot connection table extent/ABI changed')
    lines += ['static void* const offline_connection_vtable[] = {' +
              ', '.join('(void*)&' + n for n in ['request_delete'] + [f'request_virtual_{i}' for i in range(1, 6)]) + '};',
              'static uint32_t __cdecl connection_construct(hook::Regs* r, const uint32_t*) {',
              '    auto* p = reinterpret_cast<uint32_t*>(r->ecx); std::memset(p, 0, 92);',
              '    p[0] = reinterpret_cast<uintptr_t>(offline_connection_vtable); p[3] = 3;',
              '    p[17] = 0x80010001u; p[18] = reinterpret_cast<uintptr_t>("offline"); return r->ecx;', '}']
    # TransportProtocol endpoint: two slots followed immediately by text.
    endpoint_vt = struct.unpack_from('<2I', packet._img, 0x858244 - 0x400000)
    endpoint_pops = [cleanup(a) for a in endpoint_vt]
    after_endpoint = struct.unpack_from('<I', packet._img, 0x85824C - 0x400000)[0]
    if endpoint_pops != [4, 0] or 0x401000 <= after_endpoint < 0x828000:
        raise ValueError('audited two-slot endpoint table extent/ABI changed')
    lines += ['// Borrowed process-owned endpoints: slot 0 must never free static storage.',
              'static void* __fastcall endpoint_delete(void* p, void*, unsigned) { return p; }',
              'static __declspec(naked) void endpoint_reset() { __asm xor eax, eax __asm ret 0 }',
              'static void* const offline_endpoint_vtable[] = {(void*)&endpoint_delete, (void*)&endpoint_reset};',
              'struct OfflineEndpoint { void* const* vtable; uint32_t fields[6]; };',
              'static_assert(sizeof(OfflineEndpoint) == 0x1c);',
              'static OfflineEndpoint endpoints[2] = {',
              '    {offline_endpoint_vtable, {0, 0, 0xffffffffu, 0, 0, 0}},',
              '    {offline_endpoint_vtable, {0, 0, 0xffffffffu, 0, 0, 0}}};',
              'static uint32_t __cdecl primary_endpoint(hook::Regs*, const uint32_t*) { return reinterpret_cast<uintptr_t>(&endpoints[0]); }',
              'static uint32_t __cdecl secondary_endpoint(hook::Regs*, const uint32_t*) { return reinterpret_cast<uintptr_t>(&endpoints[1]); }']
    # Next selection table begins at 859fbc (stored by constructor 6630b0).
    selection_vt = struct.unpack_from('<7I', packet._img, 0x859FA0 - 0x400000)
    if list(selection_vt) != [0x662BD0, 0x64FE50, 0x64FE90, 0x662C30,
                             0x662C00, 0x662C60, 0x662D10]:
        raise ValueError('selection table identities changed')
    selection_pops = [cleanup(a) for a in selection_vt]
    if selection_pops != [4, 0, 0, 4, 4, 4, 4]:
        raise ValueError('selection table ABI changed')
    for a in targets:
        c, abi = contracts.get(a, {}), abis.get(a, {})
        fam, value, why = family(a, c, abi)
        entry = {"address": f"0x{a:08X}", "contract": c, "export_abi": {k: abi.get(k) for k in ("cc", "ret", "args", "stk")},
                 "family": fam, "reason": why, "callers": callers[a], "emitted": False}
        entry['audit'] = audit.get(f'0x{a:08X}')
        if a in SUPPORT_NAMES or a in AUDITED_FAILURES:
            if not entry['audit']:
                raise ValueError(f'missing reviewed ownership/caller evidence for {a:08X}')
        # Triage is explicitly NOT a proof of MP-only reachability. It distinguishes
        # shared callers from network-layer callers without emitting a guess.
        if c.get('name', '').startswith('ov_'):
            group = 'audio-import'
        elif a >= 0x82ADF0:
            group = 'exit-callback'
        elif any(not (0x6A6D00 <= int(Path(x['file']).stem, 16) < 0x6AB400 or
                      0x73BC00 <= int(Path(x['file']).stem, 16) < 0x753A00 or
                      0x78CEA0 <= int(Path(x['file']).stem, 16) <= 0x78D110)
                 for x in callers[a]):
            group = 'shared-game-caller-needs-boundary-audit'
        elif re.search(r'Construct|Initializ|_Init|ResetBaseState', c.get('name', '')):
            group = 'network-layout-and-ownership'
        elif '*' in c.get('ret', '') or '*' in abi.get('ret', ''):
            group = 'network-result'
        else:
            group = 'network-operation-or-result'
        entry['triage_group'] = group
        entry['mp_only_proven'] = False
        entry["result_dereference_review"] = bool("*" in c.get("ret", "") or "*" in abi.get("ret", "") or any("*" in caller["ret"] for caller in callers[a]))
        try:
            entry['binary_ret_bytes'] = cleanup(a)
        except ValueError as e:
            entry['binary_cleanup_error'] = str(e)
        if fam not in ("allocator", "deferred"):
            try:
                pop = cleanup(a)
                expected = api.stack_bytes(c.get("conv", "__cdecl"), c.get("params", ""))
                expected = 0 if c.get("conv") == "__cdecl" else expected
                if pop != expected and a not in INIT_SUPPORT and a not in WINMAIN_SUPPORT:
                    raise ValueError(f"contract pops {expected}, binary pops {pop}")
                entry["ret_bytes"] = pop
                if pop != expected:
                    entry['contract_cleanup_conflict'] = {'contract': expected, 'binary': pop,
                        'policy': 'audited process lifetime boundary uses binary cleanup'}
                entry["emitted"] = True
                lines.append(f"// {c.get('name')}: {fam}; {why}.")
                if a in support:
                    lines.append(f'HOOK_REGS(0x{a:08X}, {support[a]}, {pop}, "boundary-netz");')
                else:
                    name = f"offline_{a:08x}"
                    lines += [f"static __declspec(naked) void {name}() {{", f"    __asm mov eax, {value}"]
                    if c.get("ret") in ("int64_t", "uint64_t", "std::int64_t", "std::uint64_t"):
                        lines.append(f"    __asm mov edx, {-1 if value < 0 else 0}")
                    lines += [f"    __asm ret {pop}", "}",
                              f'HOOK(0x{a:08X}, {name}, "boundary-netz");']
            except ValueError as e:
                entry["family"] = "deferred"
                entry["reason"] = "ABI audit: " + str(e)
        manifest.append(entry)
    lines.append("#endif\n")
    return "\n".join(lines), json.dumps({"virtuals": [{"target": f"0x{a:08X}", "ret_bytes": n} for a, n in vt],
        "request_virtuals": [{"target": f"0x{a:08X}", "ret_bytes": n} for a, n in zip(request_vt, request_pops)],
        "connection_virtuals": [{"target": f"0x{a:08X}", "ret_bytes": n} for a, n in zip(connection_vt, connection_pops)],
        "endpoint_virtuals": [{"target": f"0x{a:08X}", "ret_bytes": n} for a, n in zip(endpoint_vt, endpoint_pops)],
        "selection_virtuals": [{"target": f"0x{a:08X}", "ret_bytes": n} for a, n in zip(selection_vt, selection_pops)],
        "targets": manifest}, indent=2) + "\n"


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument("--report", type=Path)
    p.add_argument("--targets", type=Path, help="bootstrap from an address-list JSON, e.g. a region -> addresses map")
    p.add_argument("--check", action="store_true")
    p.add_argument("--object", type=Path, help="also verify emitted ret N in a freshly compiled COFF object")
    args = p.parse_args()
    if args.report:
        targets = sorted(int(a, 16) for a in json.loads(args.report.read_text())["unresolved_functions"]["netz"])
    elif args.targets:
        targets = sorted(int(a, 16) for a in json.loads(args.targets.read_text())["netz"])
    else:
        targets = [int(a, 16) for a in json.loads((OUT / "targets.json").read_text())]
    source, manifest = generate(targets)
    files = {"stubs.cpp": source, "manifest.json": manifest, "targets.json": json.dumps([f"0x{a:08X}" for a in targets], indent=2) + "\n"}
    if args.check:
        bad = [name for name, content in files.items() if not (OUT / name).exists() or (OUT / name).read_text() != content]
        if bad:
            p.error("generated files differ: " + ", ".join(bad))
    else:
        OUT.mkdir(parents=True, exist_ok=True)
        for name, content in files.items():
            (OUT / name).write_text(content)
    entries = json.loads(manifest)["targets"]
    if args.object:
        dump = subprocess.run(["llvm-objdump-21", "--disassemble", "--no-show-raw-insn", str(args.object)],
                              capture_output=True, text=True, check=True).stdout
        funcs, instructions = {}, {}
        for block in re.split(r"(?=^[0-9a-f]+ <)", dump, flags=re.M):
            m = re.match(r"[0-9a-f]+ <\?([^@]+)@@[^>]+>:", block)
            if m:
                funcs[m.group(1)] = {int(n or "0", 0) for n in re.findall(r"\bretl(?:\s+\$(0x[0-9a-f]+))?", block)}
                instructions[m.group(1)] = [re.sub(r'\s+', ' ', ins.strip())
                    for ins in re.findall(r'^\s+[0-9a-f]+:\s+([^\n]+)', block, re.M)]
        support = {0x64A690: "string_default", 0x64A6B0: "string_construct", 0x64A6F0: "string_destruct", 0x64A710: "string_assign", CTOR: "gathering_construct"}
        support.update(SUPPORT_NAMES)
        checks = []
        for e in entries:
            if e["emitted"]:
                a = int(e["address"], 16)
                name = support[a] + "_regs" if a in support else f"offline_{a:08x}"
                checks.append((name, e["ret_bytes"]))
        checks += [(name, n) for name, n in zip(["gathering_delete", "gathering_factory", "gathering_class_name"], [4, 0, 4])]
        checks += [(f"offline_virtual_{i}", v["ret_bytes"]) for i, v in enumerate(json.loads(manifest)["virtuals"]) if i >= 3]
        checks += [('request_delete' if i == 0 else f'request_virtual_{i}', v['ret_bytes'])
                   for i, v in enumerate(json.loads(manifest)['request_virtuals'])]
        checks += [('request_delete' if i == 0 else f'request_virtual_{i}', v['ret_bytes'])
                   for i, v in enumerate(json.loads(manifest)['connection_virtuals'])]
        checks += [(name, v['ret_bytes']) for name, v in zip(
            ['endpoint_delete', 'endpoint_reset'], json.loads(manifest)['endpoint_virtuals'])]
        bad = [(name, n, sorted(funcs.get(name, set()))) for name, n in checks if funcs.get(name) != {n}]
        if bad:
            p.error("COFF cleanup mismatch: " + str(bad))
        # A ret N alone does not establish the ECX/argument-preserving entry ABI.
        # Check the actual HOOK_REGS frame/restore sequence, not just its source.
        regs_entry = ['pushal', 'leal 0x24(%esp), %eax', 'pushl %eax',
                      'leal 0x4(%esp), %eax', 'pushl %eax', 'calll *0x0',
                      'addl $0x8, %esp', 'movl %eax, 0x1c(%esp)', 'popal']
        bad_entry = []
        for e in entries:
            if not e['emitted']:
                continue
            a = int(e['address'], 16)
            name = support[a] + '_regs' if a in support else f'offline_{a:08x}'
            ins = instructions.get(name, [])
            if a in support:
                valid = ins[:9] == regs_entry and len(ins) >= 10 and ins[9].startswith('retl')
            else:
                # Result-only naked entries must not acquire a compiler prologue,
                # read untrusted parameters, or alter non-result registers.
                end = next((i for i, v in enumerate(ins) if v.startswith('retl')), -1)
                result = ins[:end] if end >= 0 else []
                valid = bool(result) and all(re.fullmatch(
                    r'movl \$0x[0-9a-f]+, %(eax|edx)(?: #.*)?', v) for v in result)
            if not valid:
                bad_entry.append(name)
        if bad_entry:
            p.error('COFF entry ABI mismatch: ' + ', '.join(bad_entry))
        print(f"COFF cleanup verified: {len(checks)} entry/virtual wrappers")
        print(f"COFF entry sequences verified: {sum(e['emitted'] for e in entries)} boundary entries")
    print(f"NetZ targets {len(entries)}: {sum(e['emitted'] for e in entries)} emitted, "
          f"{sum(e['family'] == 'allocator' for e in entries)} owned by alloc, "
          f"{sum(e['family'] == 'deferred' for e in entries)} deferred")


if __name__ == "__main__":
    main()
