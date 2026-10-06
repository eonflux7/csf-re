#!/usr/bin/env python3
"""Cluster functions by normalized instruction shape (template instantiations, copy-paste code).

Normalization keeps mnemonics + operand kinds (reg/mem/imm), drops immediates, displacements and call
targets. Writes export/clones.json: list of clusters (>=2 members) sorted by total bytes.
"""
import collections, hashlib, json, os
import pefile
from capstone import Cs, CS_ARCH_X86, CS_MODE_32
from capstone.x86 import X86_OP_REG, X86_OP_MEM, X86_OP_IMM

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
pe = pefile.PE(os.path.join(ROOT, "work", "pc", "CommXPC.exe"), fast_load=True)
base = pe.OPTIONAL_HEADER.ImageBase
img = pe.get_memory_mapped_image()
md = Cs(CS_ARCH_X86, CS_MODE_32)
md.detail = True


def shape(ea, size):
    toks = []
    for ins in md.disasm(img[ea - base: ea - base + size], ea):
        ops = []
        for op in ins.operands:
            if op.type == X86_OP_REG:
                ops.append(ins.reg_name(op.reg))
            elif op.type == X86_OP_MEM:
                ops.append("m" + (ins.reg_name(op.mem.base) if op.mem.base else "") + str(op.size))
            elif op.type == X86_OP_IMM:
                ops.append("i")
        toks.append(ins.mnemonic + " " + ",".join(ops))
    return toks


def main():
    rows = [json.loads(l) for l in open(os.path.join(ROOT, "export", "functions.jsonl"))]
    groups = collections.defaultdict(list)
    for f in rows:
        if f["size"] < 16:  # too small to be meaningful; handled as "tiny" batches instead
            continue
        h = hashlib.sha1("\n".join(shape(f["ea"], f["size"])).encode()).hexdigest()[:16]
        groups[h].append(f)
    clusters = []
    for h, fs in groups.items():
        if len(fs) < 2:
            continue
        clusters.append(dict(hash=h, n=len(fs), size=fs[0]["size"], total=sum(f["size"] for f in fs),
                             members=[f"{f['ea']:08x}" for f in fs]))
    clusters.sort(key=lambda c: -c["total"])
    json.dump(clusters, open(os.path.join(ROOT, "export", "clones.json"), "w"), indent=0)
    n_in = sum(c["n"] for c in clusters)
    saved = sum(c["total"] - c["size"] for c in clusters)
    print(f"clusters {len(clusters)}  functions in clusters {n_in}  bytes saved if one impl per cluster {saved}")
    for c in clusters[:15]:
        print(f"  n={c['n']:4d} size={c['size']:5d} first={c['members'][0]}")


if __name__ == "__main__":
    main()
