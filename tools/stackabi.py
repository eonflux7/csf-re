"""Derive indirect-call stack pops from binary stack restoration, rather than IDA's shared-chunk deltas.

Only uniquely solved facts are returned. Back edges are skipped; unknown frame operations and excessive
path counts abort the proof. Global callback-table slots are grouped across dispatch registers; other sites
stay independent. Each equation comes from an actual save/restore or return on a simple binary path.
"""
from fractions import Fraction
import re


def indirect_facts(ea, db):
    from packet import disasm, ida_view
    from gate import effective_rets
    f = db.F.get(ea)
    if f is None:  # a test fixture or a chunk without a row: nothing to derive
        return {}
    ins = disasm(ea, f["size"])
    if len(ins) > 512 or not any(i.mnemonic == "call" and not i.op_str.startswith("0x") for i in ins):
        return {}
    at = {i.address: i for i in ins}
    # Reserved outgoing argument slots can be written directly instead of pushed. In that shape the
    # local-frame floor is not a bound on callee pops; rely on restore equations alone.
    bounds_safe = not any(
        "[esp" in i.op_str.split(", ")[0] and i.mnemonic not in ("cmp", "test", "push", "call") or
        i.mnemonic in ("mov", "lea") and i.op_str.endswith(", esp") and i.op_str != "ebp, esp" or
        i.mnemonic == "lea" and "[esp" in i.op_str and not i.op_str.startswith("esp,")
        for i in ins)
    chunks = {a for a, _ in (ida_view(ea) or {}).get("chunks", [])}
    equations, bounds, sites, groups = [], [], {}, {}
    paths = 0
    failed = False

    def add(expr, amount=0, key=None):
        c, vs = expr
        vs = dict(vs)
        if key is not None:
            vs[key] = vs.get(key, 0) + 1
        return c + amount, vs

    def equal(a, b):
        equations.append((a[0] - b[0], {k: a[1].get(k, 0) - b[1].get(k, 0)
                                      for k in a[1].keys() | b[1].keys()}))

    def walk(pc, esp, frame, floor, saves, written, origins, pushes, seen):
        nonlocal paths, failed
        paths += 1
        if pc in seen:
            return
        if paths > 12000 or pc not in at:
            failed = True
            return
        seen = seen | {pc}
        i = at[pc]
        m, ops = i.mnemonic, i.op_str
        parts = ops.split(", ")
        dst = parts[0]
        nxt = pc + i.size
        saves = {k: list(v) for k, v in saves.items()}
        written, origins = set(written), dict(origins)
        if pc in chunks:
            written = set()  # a shared entry may save the current register values in its own prologue
        if m == "push":
            esp = add(esp, -4)
            if dst in ("ebp", "ebx", "esi", "edi") and dst not in written:
                saves.setdefault(dst, []).append(esp)
                floor = esp
            else:
                pushes += 1
        elif m == "pop":
            if dst in saves and saves[dst]:
                equal(esp, saves[dst].pop())
            elif dst in ("ebp", "ebx", "esi", "edi", "esp"):
                failed = True
                return
            esp = add(esp, 4)
        elif m in ("add", "sub") and dst == "esp":
            try:
                esp = add(esp, (1 if m == "add" else -1) * int(parts[1], 0))
                if m == "sub":
                    floor = esp
            except ValueError:
                failed = True
                return
        elif m == "mov" and ops == "ebp, esp":
            frame = esp
        elif m == "mov" and ops == "esp, ebp":
            esp = frame
        elif m == "lea" and ops == "esp, [esp]":
            pass  # MSVC alignment padding
        elif m == "leave":
            if frame is None or not saves.get("ebp"):
                failed = True
                return
            equal(frame, saves["ebp"].pop())
            esp = add(frame, 4)
        elif m == "ret":
            equal(esp, (0, {}))
            if any(saves.values()):
                failed = True
            return
        elif m == "call":
            if re.fullmatch(r"0x[0-9a-f]+", ops):
                target = int(ops, 16)
                if target not in db.F:
                    failed = True
                    return
                pops = effective_rets(target, db)
                if len(pops) != 1:
                    # A forwarder ending in an indirect tail jump has no ret of its own. Let the caller's
                    # restores constrain its pops too, without trusting an invented prototype.
                    group = (target,)
                    groups.setdefault(group, [])
                    esp = add(esp, key=group)
                else:
                    esp = add(esp, pops[0])
            else:
                match = re.fullmatch(r"dword ptr \[(e\w+)(?: \+ (0x[0-9a-f]+|\d+))?\]", ops)
                origin = origins.get(match[1]) if match else None
                group = (origin, int(match[2] or "0", 0)) if origin is not None else (pc,)
                sites[pc] = group
                groups.setdefault(group, []).append((pc, pushes, match[1] if match else None))
                esp = add(esp, key=group)
                # A call cannot consume the caller's saved registers or reserved local frame. At the
                # frame floor, nonnegative pop sizes force every outstanding call pop to zero.
                bounds.append((esp[0] - floor[0], {k: esp[1].get(k, 0) - floor[1].get(k, 0)
                                                  for k in esp[1].keys() | floor[1].keys()}))
            pushes = 0
            for reg in ("eax", "ecx", "edx"):
                origins.pop(reg, None)
        elif m.startswith("j"):
            if not re.fullmatch(r"0x[0-9a-f]+", ops):
                failed = True
                return
            walk(int(ops, 16), esp, frame, floor, saves, written, origins, pushes, seen)
            if m != "jmp":
                walk(nxt, esp, frame, floor, saves, written, origins, pushes, seen)
            return
        elif dst == "esp" or m in ("enter", "int3"):
            failed = True
            return
        if m in ("mov", "lea", "xor", "and", "or", "movzx", "movsx") and dst in (
                "eax", "ecx", "edx", "ebx", "esi", "edi", "ebp"):
            written.add(dst)
            origin = re.fullmatch(r"dword ptr \[(0x[0-9a-f]+)\]", parts[1]) if m == "mov" and len(parts) == 2 else None
            if origin:
                origins[dst] = int(origin[1], 16)
            elif m == "mov" and len(parts) == 2 and parts[1] in origins:
                origins[dst] = origins[parts[1]]
            else:
                origins.pop(dst, None)
        walk(nxt, esp, frame, floor, saves, written, origins, pushes, seen)

    walk(ea, (0, {}), None, (0, {}), {}, set(), {}, 0, set())
    if failed or not equations:
        return {}
    keys = list(groups)
    for c, vs in bounds if bounds_safe else []:
        if c == 0 and all(v >= 0 for v in vs.values()):
            equations.extend((0, {k: 1}) for k, v in vs.items() if v)
    rows = [[Fraction(v.get(k, 0)) for k in keys] + [Fraction(-c)] for c, v in equations]
    pivots, r = {}, 0
    for col in range(len(keys)):
        pivot = next((i for i in range(r, len(rows)) if rows[i][col]), None)
        if pivot is None:
            continue
        rows[r], rows[pivot] = rows[pivot], rows[r]
        scale = rows[r][col]
        rows[r] = [x / scale for x in rows[r]]
        for i in range(len(rows)):
            if i != r and rows[i][col]:
                scale = rows[i][col]
                rows[i] = [x - scale * y for x, y in zip(rows[i], rows[r])]
        pivots[col] = r
        r += 1
    if any(not any(row[:-1]) and row[-1] for row in rows):
        return {}
    solved = {}
    for col, row in pivots.items():
        value = rows[row][-1]
        if sum(bool(x) for x in rows[row][:-1]) == 1 and value.denominator == 1 and 0 <= value <= 32 and value % 4 == 0:
            solved[keys[col]] = int(value)
    facts = {}
    for pc, group in sites.items():
        if group not in solved:
            continue
        pop = solved[group]
        fact = dict(pop=pop)
        uses = set(groups[group])
        # Alternating dispatch registers for one global callback slot prove ecx is incidental: at one
        # site it holds the table, at another it holds unrelated data. Uniform pushes give its arity.
        if len(group) == 2 and len({reg for _, _, reg in uses}) > 1 and \
                len({n for _, n, _ in uses}) == 1 and pop == 0:
            fact.update(nargs=next(iter(uses))[1], ecx=False)
        elif pop:
            fact["nargs"] = pop // 4
        else:
            after = at.get(pc + at[pc].size)
            counts = {n for site, n, _ in uses if site == pc}
            if len(counts) == 1 and after and after.mnemonic == "add" and after.op_str.startswith("esp, "):
                nargs = next(iter(counts))
                if nargs and int(after.op_str.split(", ")[1], 0) == nargs * 4:
                    fact["nargs"] = nargs  # this call's explicit pushes and immediate caller cleanup agree
        facts[pc] = fact
    return facts
