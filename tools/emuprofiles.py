"""Concrete input graphs for instruction-audited, otherwise blocked game routines.

These are additional inputs, not repair-at-runtime or comparison exemptions.
Only original dispatch provenance and RET/stack constraints select tables. All
pages and pointer results are mirrored; the original must finish normally (or
at a proven nonreturning boundary) before an input counts as evidence.
"""
import collections
import json
import os
import struct


# Audited requests: keep existing controls' input coverage unchanged. The
# profiles below may be extended after reading a new routine's dereferences.
AUDITED = frozenset(int(x, 16) for x in '''
0041f6c0 00440f50 004d31e0 004de9b0 00533040 006aa880 005322a0
005348b0 00535860 0053cda0 00540030 006913d0 006b2e20 006b7670
006d6800 006e3a70 006e7000 006ef050 006f1170 007215f0 00721900
00729710 0073f3b0 0073f4f0 00738070 00738530 00782330
'''.split())


class Graph:
    def __init__(self):
        self.words, self.args, self.results = {}, {}, {}
        self.nodes = {}
        self.pages = set()
        self.site_tables = {}
        self.evidence = []

    def alloc(self, key):
        if key not in self.nodes:
            self.nodes[key] = 0x31000100 + len(self.nodes) * 0x10000
            self.pages.add(self.nodes[key] & ~0xFFF)
        return self.nodes[key]

    def put(self, address, value):
        self.words[address] = value & 0xFFFFFFFF
        self.pages.add(address & ~0xFFF)

    def page(self, base):
        data = bytearray(0x1000)
        for address, value in self.words.items():
            if address & ~0xFFF == base:
                struct.pack_into('<I', data, address - base, value)
        return bytes(data)


def build(e, ea, variant=0):
    """Build a small zero-initialized graph with actual image vtables.

    Choosing a concrete table here is input enumeration, NOT a claim that an
    unresolved dispatch always has that class or convention. Reject tables
    contradicting even one uniquely proved caller cleanup or the full CFG.
    """
    if ea not in AUDITED:
        return None
    graph = Graph()
    graph.pages.add(e.ECX0 & ~0xFFF)
    from packet import DB
    db = e.load('db', DB)
    root_table = db.class_of(ea)
    if root_table:
        graph.put(e.ECX0, int(root_table, 16))
    evidence = e.dispatch_evidence(ea)
    groups = collections.defaultdict(list)
    for pc, ev in evidence.items():
        if ev.get('virtual'):
            groups[ev['group'][0]].append((pc, ev['slot'], ev))
    tables = e.load('vtables', lambda: json.load(open(os.path.join(e.ROOT, 'db', 'vtables.json'))))
    counts = e.stack_call_pops(ea, constraints_only=True)

    def node(expr):
        if not expr:
            return None
        kind = expr[0]
        if kind == 'input':
            return {'ecx': e.ECX0, 'eax': e.EAX0,
                    'edx': e.ZONES + 0x0A100100}.get(expr[1])
        if kind == 'object':
            from packet import DB
            db = e.load('db', DB)
            return e.ECX0 if db.class_of(ea) == expr[1] else None
        if kind == 'frame' and expr[1] >= 8:
            k = (expr[1] - 8) // 4
            graph.args.setdefault(k, graph.alloc(expr))
            return graph.args[k]
        if kind == 'global':
            address = graph.alloc(expr)
            graph.put(expr[1], address)
            return address
        if kind == 'constant':
            return expr[1]
        if kind == 'result':
            address = graph.alloc(expr)
            graph.results[expr[1]] = address
            return address
        if kind == 'add':
            base = node(expr[1])
            return base + expr[2] if base is not None else None
        if kind == 'load':
            base = node(expr[1])
            if base is None:
                return None
            address = base + expr[2]
            graph.words.setdefault(address, graph.alloc(expr))
            graph.pages.add(address & ~0xFFF)
            return graph.words[address]
        return None

    # The group expression is the table pointer: LOAD(receiver,0). Build its
    # nested receiver pointers recursively, then install an actual image table.
    for expr, sites in groups.items():
        if not expr or expr[0] not in ('load', 'table') or expr[0] == 'load' and expr[2] != 0:
            continue
        receiver = node(expr[1]) if expr[0] == 'load' else None
        required = max(slot for _, slot, _ in sites)
        choices = []
        for table in tables:
            vt = int(table['addr'], 16)
            if len(table['slots']) * 4 <= required:
                continue
            if expr[0] == 'load' and expr[1][0] == 'object' and vt != int(expr[1][1], 16):
                continue
            if expr[0] == 'table' and len(expr) > 2 and expr[2] and vt != int(expr[1], 16):
                continue
            if any('tables' in ev and vt not in ev['tables'] for _, _, ev in sites):
                continue
            pops = {pc: e.binary_call_pop(int(table['slots'][slot // 4], 16)) for pc, slot, _ in sites}
            if any(n is None or pc in counts and counts[pc] != n for pc, n in pops.items()):
                continue
            if not e.profile_stack_consistent(ea, pops):
                continue
            choices.append(vt)
        if choices:
            choices.sort()
            selected = choices[0]
            if variant:
                inventory = {int(v['addr'], 16): v['slots'] for v in tables}
                def signature(vt):
                    return tuple(e.takes_ecx(int(inventory[vt][slot // 4], 16)) for _, slot, _ in sites)
                first = signature(selected)
                selected = next((vt for vt in choices[1:] if signature(vt) != first), choices[-1])
            graph.site_tables.update({pc: selected for pc, _, _ in sites})
            if receiver is not None:
                graph.put(receiver, selected)

    for pc, ev in evidence.items():
        if 'target' in ev and 'table' in ev:
            graph.site_tables.setdefault(pc, ev['table'])

    # Original container invariants. Unlike random pointers, these are finite
    # empty/nonempty ranges or sentinel lists the actual loops can terminate on.
    if ea in (0x4DE9B0, 0x6E7000):
        # [this+4] capacity, [this+8] used; records are 12/20 bytes.
        graph.put(e.ECX0, graph.alloc('records'))
        graph.put(e.ECX0 + 4, 2)
        graph.put(e.ECX0 + 8, variant)
        graph.args[0] = 1
    if ea in (0x6AA880, 0x782330):
        off = 0x44 if ea == 0x6AA880 else 8
        head = graph.alloc('head')
        graph.put(e.ECX0 + off, head)
        graph.put(head, head)
        graph.put(head + 4, head)
        if ea == 0x782330:
            graph.args[1] = 0  # lookup without insertion
    if ea == 0x6B7670:
        # Erasing the map's nil/end iterator is the explicit exception path:
        # 6B767D tests [arg1+15] before it touches the tree links.
        nil = graph.alloc('nil_iterator')
        graph.put(nil + 0x14, 0x100)
        graph.args.update({0: graph.alloc('iterator_output'), 1: nil})
    if ea == 0x6D6800:
        graph.put(e.ECX0 + 0x5C, 0)  # no children; update/optimise still run
    if ea in (0x7215F0, 0x721900):
        # VC7.1 wide-string SSO: empty length and capacity seven, cursor -1.
        for off in (0x150, 0x16C):
            graph.put(e.ECX0 + off + 0x14, 0)
            graph.put(e.ECX0 + off + 0x18, 7)
        graph.put(e.ECX0 + 0x1B0, 0xFFFFFFFF)
    if ea == 0x533040:
        source = graph.alloc('source_string')
        graph.args.update({0: 0, 1: variant, 2: source, 3: 0, 4: variant})
        for address in (e.ECX0, source):
            graph.put(address + 0x14, variant)
            graph.put(address + 0x18, 7)
    if ea == 0x729710:
        # The strip loop advances by [this+58]. A positive tile width is a
        # valid input; zero width explains the former non-progressing cap.
        graph.put(e.ECX0 + 0x58, 0x3F800000 if not variant else 0x40000000)
    if ea in (0x738070, 0x738530):
        graph.put(e.ECX0 + 8, 1)  # active render state, not only the disabled path
    if ea == 0x738530:
        graph.put(e.ECX0 + 0x1C, 1)  # at least one work item, entering the draw body
        graph.put(e.ECX0 + 0x34, variant)  # both draw modes
    if ea == 0x440F50:
        owner = graph.args.setdefault(0, graph.alloc('owner'))
        head = graph.alloc('head')
        graph.put(owner + 4, head)
        for off in (0, 4, 8):
            graph.put(head + off, head)
        archive = graph.args.get(1)
        if archive is not None:
            graph.put(archive + 0x14, 1)  # writing an empty tree
    if ea == 0x41F6C0:
        # Writing initializes the count from the input vector ([arg0+8]);
        # reading needs a real archive out-count, which no generic mock writes.
        archive = graph.args.get(1)
        if archive is not None:
            graph.put(archive + 0x14, 1)
    return graph


def discover(e, ea, prof, own, ret_site, memory_obj, graph, attempts=24):
    """Follow faulting pointer loads; restart with an immutable initial graph.

    No candidate is inspected, no branch/register/instruction is patched, and
    no field written by the function or by a mock is repaired. Each repair is
    justified by an actual dereference. Cycles/count loops and ambiguous ABI
    remain inconclusive; there is deliberately no blanket cap-escape rule.
    """
    import capstone
    from unicorn import UC_HOOK_CODE, UC_HOOK_MEM_UNMAPPED, UcError
    from unicorn import x86_const as X
    code = e.original_stack_code(ea)
    for _ in range(attempts):
        run = e.Run(ea, prof, own, memory_obj=memory_obj, graph=graph)
        run.ret_site = ret_site
        origins, repair, before = {}, [], {}

        def readable_input(address):
            # Only initial heap/data fields, not executable image, locals,
            # original stores or mock outputs. .data starts after .text/.rdata.
            return (e.ZONES <= address < e.OBJ_BASE or 0x890000 <= address < 0x900000) and not any(
                address + k in run.written for k in range(4))

        def memory_address(uc, mem):
            reg = lambda n: uc.reg_read(getattr(X, 'UC_X86_REG_' + code_name(n).upper())) if n else 0
            return (reg(mem.base) + reg(mem.index) * mem.scale + mem.disp) & 0xFFFFFFFF

        # Capstone's register IDs differ from Unicorn's.
        md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
        code_name = md.reg_name

        def request(uc, field, value, pc):
            if field is None or not readable_input(field) or graph.words.get(field) == value:
                return False
            repair.append((field, value, pc))
            run.status = 'profile discovery'
            uc.emu_stop()
            return True

        def trace(uc, pc, size, data):
            ins = code.get(pc)
            if ins is None:
                return
            before.clear()
            before.update(origins)
            ops = ins.operands
            if ins.mnemonic == 'call':
                if pc in graph.site_tables and ops[0].type == capstone.x86.X86_OP_MEM:
                    base = ins.reg_name(ops[0].mem.base)
                    if request(uc, origins.get(base), graph.site_tables[pc], pc):
                        return
                for reg in ('eax', 'ecx', 'edx'):
                    origins.pop(reg, None)
                return
            origin = None
            if ins.mnemonic == 'mov' and len(ops) == 2 and ops[0].type == capstone.x86.X86_OP_REG:
                if ops[1].type == capstone.x86.X86_OP_REG:
                    origin = origins.get(ins.reg_name(ops[1].reg))
                elif ops[1].type == capstone.x86.X86_OP_MEM and ops[1].size == 4:
                    address = memory_address(uc, ops[1].mem)
                    if readable_input(address):
                        origin = address
            for reg in ins.regs_access()[1]:
                origins.pop(e.register_parent(ins.reg_name(reg)), None)
            if origin is not None:
                origins[ins.reg_name(ops[0].reg)] = origin

        def fault(uc, access, address, size, value, data):
            if access == 21 or address >= 0x10000:
                return False
            pc = uc.reg_read(X.UC_X86_REG_EIP)
            ins = code.get(pc)
            if ins:
                for operand in ins.operands:
                    if operand.type == capstone.x86.X86_OP_MEM and not operand.mem.index:
                        field = before.get(ins.reg_name(operand.mem.base))
                        if field is not None:
                            request(uc, field, graph.alloc(('dereference', field)), pc)
            return False

        run.uc.hook_add(UC_HOOK_CODE, trace)
        run.uc.hook_add(UC_HOOK_MEM_UNMAPPED, fault)
        run.start(ea)
        if not repair:
            return run
        field, value, pc = repair[0]
        graph.put(field, value)
        graph.evidence.append(f'{pc:08x}: input [{field:08x}] = {value:08x}')
    return run  # discovery-limit status is never a completed original run
