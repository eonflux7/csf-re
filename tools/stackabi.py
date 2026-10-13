"""Derive indirect-call stack pops from binary stack restoration, rather than IDA's shared-chunk deltas.

Only uniquely solved facts are returned. Back edges are skipped; unknown frame operations and excessive
path counts abort the proof. Global callback-table slots are grouped across dispatch registers; other sites
stay independent. Each equation comes from an actual save/restore or return on a simple binary path.
"""
from fractions import Fraction
import re
import functools
import json
import os


def call_evidence(ea, db, mode='calls', receiver=None):
    """Original-only reaching definitions of dispatch pointers.

    Known class/constant-table slots and image-initialized global callbacks yield
    exact targets. Unknown vtables yield identities, not guessed targets: those
    identities let the caller's stack equations relate repeated dispatches.
    No candidate code or IDA SP deltas enter this analysis.
    """
    from packet import disasm
    import this_class
    import api
    from semcheck import image_dword
    cache = getattr(db, '_dispatch_cache', None)
    if cache is None:
        cache = db._dispatch_cache = {}
    key = ea, mode, receiver
    if key in cache:
        return cache[key]
    cache[key] = None if mode == 'returns' else {}  # recursive types stay unknown
    f = db.F.get(ea)
    if not f:
        return {}
    ins = disasm(ea, f['size'])
    at = {i.address: i for i in ins}
    pred = {pc: [] for pc in at}
    for i in ins:
        if i.mnemonic.startswith('j'):
            if re.fullmatch(r'0x[0-9a-f]+', i.op_str):
                t = int(i.op_str, 16)
                if t in pred:
                    pred[t].append(i.address)
            if i.mnemonic == 'jmp':
                continue
        if i.mnemonic != 'ret' and i.address + i.size in pred:
            pred[i.address + i.size].append(i.address)
    if not hasattr(db, '_dispatch_metadata'):
        with open(os.path.join(api.ROOT, 'db', 'vtables.json')) as stream:
            tables = {int(v['addr'], 16): v['slots'] for v in json.load(stream)}
        db._dispatch_metadata = ({db.cls_name(v): v for v in db.classes},
                                 this_class.load_args(), tables)
    names, types, tables = db._dispatch_metadata
    cls = receiver or db.class_of(ea)
    abi = api.ida_abi().get(ea, {})

    def typed(t):
        name = re.sub(r'\b(const|volatile|struct|class)\b|[ *&]', '', t)
        return ('object', names[name]) if '*' in t and name in names else None

    def field(base, off, pc):
        if base is cycle:
            return None
        stored = memory_source(base, off, pc) if base else None
        if stored is not None and stored is not cycle:
            return stored
        if base and base[0] == 'object':
            vt = base[1]
            if off == 0:
                return ('table', vt, len(base) > 2 and base[2])
            # A concrete constructor assignment is stronger than a propagated
            # base-field type (base types can have spurious beyond-size members).
            values = []
            for ctor in db.classes.get(vt, {}).get('ctors', []):
                values += call_evidence(int(ctor, 16), db, f'fields:{off}').get(off, [])
            nonnull = [v for v in values if v != ('constant', 0)]
            if nonnull and all(isinstance(v, tuple) and v[0] == 'object' and v == nonnull[0] for v in nonnull):
                return nonnull[0]
            while vt:
                cn = db.cls_name(vt)
                for name, value in types.get('fields', {}).get(cn, {}).items():
                    layout = db.class_names.get(vt, {}).get('fields', {})
                    offsets = [k if isinstance(k, int) else int(k, 16)
                               for k, n in layout.items() if n.split()[-1] == name]
                    match = re.fullmatch(r'(?:field_|pad_)([0-9a-fA-F]+)', name)
                    if off in offsets or match and int(match[1], 16) == off:
                        if value in db.classes:
                            return ('object', value)
                vt = db.classes.get(vt, {}).get('base')
            # Constructor stores are binary evidence where typed exports leave
            # an integer field. Inspect only this class and its bases; never infer
            # a pointer type from scalar stores or from a majority of guesses.
            vt = base[1]
            while vt:
                values = []
                for ctor in db.classes.get(vt, {}).get('ctors', []):
                    values += call_evidence(int(ctor, 16), db, f'fields:{off}').get(off, [])
                nonnull = [v for v in values if v != ('constant', 0)]
                if nonnull and all(isinstance(v, tuple) and v[0] == 'object' and v == nonnull[0] for v in nonnull):
                    return nonnull[0]
                vt = db.classes.get(vt, {}).get('base')
        return ('load', base, off) if base else None

    active = set()
    budget = [25000]
    cycle = object()

    def aliases(reg):
        return {reg, reg[1:], reg[1] + 'l', reg[1] + 'h'} if reg in ('eax', 'ebx', 'ecx', 'edx') else {reg, reg[1:]}

    def implicit_write(m, reg):
        return m.startswith('rep') and reg in ('ecx', 'esi', 'edi') or m.startswith('loop') and reg == 'ecx'

    def meet(values):
        # A null allocation path cannot reach a virtual dispatch. It adds no
        # competing receiver class/provenance to a non-null reaching definition.
        nonnull = [v for v in values if v != ('constant', 0)]
        if nonnull:
            values = nonnull
        return values[0] if values and all(v == values[0] for v in values) else None if values else cycle

    @functools.lru_cache(None)
    def memory_source(base, off, pc):
        key = ('memory', base, off, pc)
        budget[0] -= 1
        if budget[0] < 0:
            return None
        if key in active:
            return cycle
        trail = set()
        while pc != ea and len(pred.get(pc, [])) == 1:
            p = pred[pc][0]
            if p in trail:
                return cycle
            trail.add(p)
            j = at[p]
            dst = j.op_str.split(', ')[0]
            match = re.fullmatch(r'dword ptr \[(e[a-z]{2})(?: ([+-]) (0x[0-9a-f]+|\d+))?\]', dst)
            offset = int(match[3] or '0', 0) * (-1 if match[2] == '-' else 1) if match else None
            if j.mnemonic == 'mov' and match and offset == off:
                break
            pc = p
        if pc == ea:
            return None
        active.add(key)
        values = []
        for p in pred.get(pc, []):
            j = at[p]
            parts = j.op_str.split(', ')
            match = re.fullmatch(r'dword ptr \[(e[a-z]{2})(?: ([+-]) (0x[0-9a-f]+|\d+))?\]', parts[0])
            offset = int(match[3] or '0', 0) * (-1 if match[2] == '-' else 1) if match else None
            if j.mnemonic == 'mov' and len(parts) == 2 and match and offset == off and source(match[1], p) == base:
                value = operand(parts[1], p)
            else:
                value = memory_source(base, off, p)
            if value is not cycle:
                values.append(value)
        active.remove(key)
        return meet(values)

    @functools.lru_cache(None)
    def source(reg, pc):
        key = reg, pc
        budget[0] -= 1
        if budget[0] < 0:
            return None
        if key in active:
            return cycle
        trail = set()
        while pc != ea and len(pred.get(pc, [])) == 1:
            p = pred[pc][0]
            if p in trail:
                return cycle
            trail.add(p)
            i = at[p]
            dst = i.op_str.split(', ')[0]
            if i.mnemonic == 'call' and reg in ('eax', 'ecx', 'edx') or implicit_write(i.mnemonic, reg) or \
                    dst in aliases(reg) and i.mnemonic not in ('cmp', 'test', 'push'):
                break
            pc = p
        if pc == ea:
            if reg == 'ecx' and cls:
                return ('object', cls)
            arg = next((typed(t) for t, _, loc in abi.get('args', []) if loc == reg), None)
            return arg or ('input', reg)
        if not pred.get(pc):
            return None  # an unreachable exported head is not a reaching definition
        active.add(key)
        values = [v for p in pred.get(pc, []) if (v := after(reg, p)) is not cycle]
        active.remove(key)
        return meet(values)

    def operand(op, pc):
        if re.fullmatch(r'e[a-z]{2}', op):
            return source(op, pc)
        if re.fullmatch(r'0x[0-9a-f]+|\d+', op):
            value = int(op, 0)
            g = types.get('globals', {}).get(f'{value:08x}', {})
            return ('object', g['vt']) if isinstance(g, dict) and g.get('kind') == 'inst' else ('constant', value)
        m = re.fullmatch(r'dword ptr \[(e[a-z]{2})(?: ([+-]) (0x[0-9a-f]+|\d+))?\]', op)
        if m:
            off = int(m[3] or '0', 0) * (-1 if m[2] == '-' else 1)
            if m[1] == 'ebp':
                # Frame-based spills have stable addresses even when call pops are unknown.
                value = memory_source(source('ebp', pc), off, pc) if off < 0 else None
                return value if value is not None and value is not cycle else ('frame', off)
            return field(source(m[1], pc), off, pc)
        m = re.fullmatch(r'dword ptr \[(0x[0-9a-f]+)\]', op)
        if m:
            addr = int(m[1], 16)
            val = image_dword(addr)
            if val in db.F or val in tables:
                return ('constant', val)
            g = types.get('globals', {}).get(f'{addr:08x}', {})
            vt = g.get('vt') if isinstance(g, dict) else g
            return ('object', vt) if isinstance(g, dict) and g.get('kind') == 'ptr' and vt in db.classes else ('global', addr)
        return None

    def after(reg, pc):
        i = at[pc]
        m, ops = i.mnemonic, i.op_str.split(', ')
        if m == 'call' and reg in ('eax', 'ecx', 'edx'):
            if reg != 'eax':
                return None
            if re.fullmatch(r'0x[0-9a-f]+', i.op_str):
                t = int(i.op_str, 16)
                vt = types.get('returns', {}).get(f'{t:08x}')
                tp = typed(api.ida_abi().get(t, {}).get('ret', ''))
                # Original trivial pointer getters carry provenance across repeated
                # calls, even when the exported return type is just an integer.
                tf = db.F.get(t)
                code = disasm(t, tf['size']) if tf and tf['size'] <= 16 else []
                if len(code) == 2 and code[0].mnemonic == 'mov' and code[1].mnemonic == 'ret' and \
                        code[0].op_str.startswith('eax, '):
                    op = code[0].op_str[5:]
                    if op.startswith('dword ptr [0x'):
                        return operand(op, pc)
                    match = re.fullmatch(r'dword ptr \[ecx(?: \+ (0x[0-9a-f]+|\d+))?\]', op)
                    if match:
                        off = int(match[1] or '0', 0)
                        value = field(source('ecx', pc), off, pc)
                        if value and value[0] in ('object', 'table', 'constant'):
                            return value
                        return ('getter', t, off)
                if db.class_of(t):
                    value = call_evidence(t, db, 'returns')
                    if value:
                        return value
                return ('object', vt) if vt in db.classes else tp or ('result', pc)
            match = re.fullmatch(r'dword ptr \[(e[a-z]{2})(?: \+ (0x[0-9a-f]+|\d+))?\]', i.op_str)
            table = source(match[1], pc) if match else None
            if table is cycle:
                return ('result', pc)
            vt = int(table[1], 16) if table and table[0] == 'table' else table[1] if table and table[0] == 'constant' else None
            slot = int(match[2] or '0', 0) // 4 if match else -1
            if vt in tables and 0 <= slot < len(tables[vt]):
                value = call_evidence(int(tables[vt][slot], 16), db, 'returns')
                if value:
                    return value
            return ('result', pc)
        if implicit_write(m, reg):
            return None
        if ops[0] in aliases(reg) and m not in ('cmp', 'test', 'push'):
            if ops[0] != reg:
                return None  # a partial pointer definition is not a class-preserving copy
            if m in ('xor', 'sub') and len(ops) == 2 and ops[0] == ops[1]:
                return ('constant', 0)
            if m == 'mov' and len(ops) == 2:
                return operand(ops[1], pc)
            if m == 'lea' and len(ops) == 2 and ops[1] == f'[{reg}]':
                return source(reg, pc)
            return None
        return source(reg, pc)

    if mode == 'returns':
        values = [source('eax', i.address) for i in ins if i.mnemonic == 'ret']
        value = meet(values)
        result = value if value is not cycle and value and value[0] == 'object' else None
        if result and result[1] == cls:
            # Returning `this` from a constructor that installs its concrete
            # primary table is stronger than a merely static pointer type.
            installs = set()
            for i in ins:
                match = re.fullmatch(r'dword ptr \[(e[a-z]{2})\], 0x([0-9a-f]+)', i.op_str)
                if i.mnemonic != 'mov' or not match or int(match[2], 16) != int(cls, 16):
                    continue
                base = source(match[1], i.address)
                if isinstance(base, tuple) and base[0] == 'object' and base[1] == cls:
                    installs.add(i.address)
            # Require the concrete table installation on every normal return path.
            successors = {pc: [] for pc in at}
            for pc, previous in pred.items():
                for p in previous:
                    successors[p].append(pc)
            todo, seen = [ea], set()
            concrete = bool(installs)
            while todo and concrete:
                pc = todo.pop()
                if pc in seen or pc in installs:
                    continue
                seen.add(pc)
                if at[pc].mnemonic == 'ret':
                    concrete = False
                todo += successors[pc]
            if concrete:
                result = ('object', cls, True)
        cache[key] = result
        return result
    if mode.startswith('fields:'):
        wanted = int(mode.split(':')[1])
        out = {}
        for i in ins:
            parts = i.op_str.split(', ')
            match = re.fullmatch(r'dword ptr \[(e[a-z]{2})(?: \+ (0x[0-9a-f]+|\d+))?\]', parts[0])
            if i.mnemonic != 'mov' or len(parts) != 2 or not match:
                continue
            if int(match[2] or '0', 0) != wanted:
                continue
            base = source(match[1], i.address)
            if isinstance(base, tuple) and base[0] == 'object' and base[1] == cls:
                out.setdefault(int(match[2] or '0', 0), []).append(operand(parts[1], i.address))
        cache[key] = out
        return out
    out = {}
    for i in ins:
        if i.mnemonic not in ('call', 'jmp'):
            continue
        if re.fullmatch(r'0x[0-9a-f]+', i.op_str):
            if i.mnemonic == 'call':
                target = int(i.op_str, 16)
                out[i.address] = dict(target=target)
                from gate import effective_rets
                ret_cache = getattr(db, '_dispatch_rets', None)
                if ret_cache is None:
                    ret_cache = db._dispatch_rets = {}
                if target not in ret_cache:
                    ret_cache[target] = effective_rets(target, db)
                if ret_cache[target]:
                    continue  # receiver type cannot improve the callee's own RET
                obj = source('ecx', i.address)
                if obj and obj is not cycle and obj[0] == 'object':
                    out[i.address]['receiver'] = obj[1]
            continue
        value = operand(i.op_str, i.address)
        if value is not cycle and value and value[0] == 'constant' and value[1] in db.F:
            out[i.address] = dict(target=value[1])
            continue
        match = re.fullmatch(r'dword ptr \[(e[a-z]{2})(?: \+ (0x[0-9a-f]+|\d+))?\]', i.op_str)
        if not match:
            continue
        table = source(match[1], i.address)
        if table is cycle:
            continue
        slot = int(match[2] or '0', 0)
        if slot % 4:
            continue
        vt = int(table[1], 16) if table and table[0] == 'table' else table[1] if table else None
        if table and table[0] in ('table', 'constant') and vt in tables:
            slots = tables[vt]
            if slot // 4 < len(slots):
                from gate import effective_rets
                compatible = [vt]
                if table[0] == 'table' and not (len(table) > 2 and table[2]):
                    for child in db.classes:
                        cur, seen = child, set()
                        while cur and cur not in seen:
                            if cur == table[1]:
                                compatible.append(int(child, 16))
                                break
                            seen.add(cur)
                            cur = db.classes.get(cur, {}).get('base')
                targets = {int(tables[t][slot // 4], 16) for t in compatible if t in tables and slot // 4 < len(tables[t])}
                counts = {tuple(effective_rets(t, db)) for t in targets}
                if len(counts) == 1 and len(next(iter(counts))) == 1:
                    out[i.address] = dict(target=int(slots[slot // 4], 16), table=vt, slot=slot)
                else:
                    out[i.address] = dict(group=(table, slot), virtual=True, slot=slot,
                                          tables=tuple(sorted(set(compatible))))
        elif table and table[0] == 'load' and table[2] == 0 and table[1] is not None:
            # A virtual dispatch is recognized only when ECX holds the same object
            # whose first word was loaded as the table. Register callback tables
            # are not interchangeable with vtables.
            obj = source('ecx', i.address)
            if obj == table[1]:
                out[i.address] = dict(group=(table, slot), virtual=True, slot=slot)
    cache[key] = out
    return out


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
