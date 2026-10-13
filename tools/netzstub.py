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
static uint32_t __cdecl station_name(hook::Regs*, const uint32_t*) {
    return reinterpret_cast<uintptr_t>("");
}
} // namespace
'''


def generate(targets):
    contracts, abis = api.load(), api.ida_abi()
    callers = {a: [] for a in targets}
    for p in sorted((ROOT / "src/auto").glob("*.cpp")):
        text = p.read_text()
        for m in api.DECL.finditer(api.strip_comments(text)):
            a = int(m.group(4), 16)
            if a in callers:
                callers[a].append({"file": str(p.relative_to(ROOT)), "name": m.group(3),
                                   "ret": api.ws(m.group(1)), "conv": m.group(2), "params": api.ws(m.group(5))})
    manifest, lines = [], ["// Generated by tools/netzstub.py. Do not edit.", "#ifdef CSF_STANDALONE", '#include "globals.h"', '#include "hook.h"', SUPPORT]
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
    for a in targets:
        c, abi = contracts.get(a, {}), abis.get(a, {})
        fam, value, why = family(a, c, abi)
        entry = {"address": f"0x{a:08X}", "contract": c, "export_abi": {k: abi.get(k) for k in ("cc", "ret", "args", "stk")},
                 "family": fam, "reason": why, "callers": callers[a], "emitted": False}
        entry["result_dereference_review"] = bool("*" in c.get("ret", "") or "*" in abi.get("ret", "") or any("*" in caller["ret"] for caller in callers[a]))
        if fam not in ("allocator", "deferred"):
            try:
                pop = cleanup(a)
                expected = api.stack_bytes(c.get("conv", "__cdecl"), c.get("params", ""))
                expected = 0 if c.get("conv") == "__cdecl" else expected
                if pop != expected:
                    raise ValueError(f"contract pops {expected}, binary pops {pop}")
                entry["ret_bytes"] = pop
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
    return "\n".join(lines), json.dumps({"virtuals": [{"target": f"0x{a:08X}", "ret_bytes": n} for a, n in vt], "targets": manifest}, indent=2) + "\n"


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
        funcs = {}
        for block in re.split(r"(?=^[0-9a-f]+ <)", dump, flags=re.M):
            m = re.match(r"[0-9a-f]+ <\?([^@]+)@@[^>]+>:", block)
            if m:
                funcs[m.group(1)] = {int(n or "0", 0) for n in re.findall(r"\bretl(?:\s+\$(0x[0-9a-f]+))?", block)}
        support = {0x64A690: "string_default", 0x64A6B0: "string_construct", 0x64A6F0: "string_destruct", 0x64A710: "string_assign", CTOR: "gathering_construct"}
        checks = []
        for e in entries:
            if e["emitted"]:
                a = int(e["address"], 16)
                name = support[a] + "_regs" if a in support else f"offline_{a:08x}"
                checks.append((name, e["ret_bytes"]))
        checks += [(name, n) for name, n in zip(["gathering_delete", "gathering_factory", "gathering_class_name"], [4, 0, 4])]
        checks += [(f"offline_virtual_{i}", v["ret_bytes"]) for i, v in enumerate(json.loads(manifest)["virtuals"]) if i >= 3]
        bad = [(name, n, sorted(funcs.get(name, set()))) for name, n in checks if funcs.get(name) != {n}]
        if bad:
            p.error("COFF cleanup mismatch: " + str(bad))
        print(f"COFF cleanup verified: {len(checks)} entry/virtual wrappers")
    print(f"NetZ targets {len(entries)}: {sum(e['emitted'] for e in entries)} emitted, "
          f"{sum(e['family'] == 'allocator' for e in entries)} owned by alloc, "
          f"{sum(e['family'] == 'deferred' for e in entries)} deferred")


if __name__ == "__main__":
    main()
