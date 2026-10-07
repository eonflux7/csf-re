"""Run the play copy under x32dbg (x64dbg-automate) and report the first crash. Windows-side script.

Run via tools/dbg.sh crash [seconds]. Writes a report to stdout:
  faulting eip + module, registers, disassembly around eip, stack words with module/symbol hints.
Symbolization of game addresses and of our dinput8.dll happens WSL-side (tools/dbg.sh pipes through
tools/dbg/symbolize.py).
"""
import sys, time
from x64dbg_automate import X64DbgClient
from x64dbg_automate.events import EventType

X64DBG = r"E:\dev\tools\x64dbg\release\x32\x32dbg.exe"
GAME = r"E:\dev\csf-play\CommXPC.exe"
secs = int(sys.argv[1]) if len(sys.argv) > 1 else 90

c = X64DbgClient(x64dbg_path=X64DBG)
c.start_session(GAME, current_dir=r"E:\dev\csf-play")
c.wait_until_debugging(30)
FATAL = (0xC0000005, 0xC000001D, 0xC00000FD, 0xC0000094, 0xC0000096)
c.clear_debug_events()
c.go(pass_exceptions=True)
t0 = time.time()
crashed = None
while time.time() - t0 < secs:
    if not c.is_debugging():
        print("process exited")
        break
    ev = c.wait_for_debug_event(EventType.EVENT_EXCEPTION, timeout=1)
    if ev is not None and ev.event_data.ExceptionCode in FATAL:
        crashed = ev.event_data
        break
    if not c.is_running():  # stopped for any other reason (system breakpoint, entry, handled exception): continue
        c.go(pass_exceptions=True)
if crashed is None:
    print(f"no crash in {secs}s")
else:
    c.wait_until_stopped(5)
    regs = c.get_regs()
    ctx = (regs[0] if isinstance(regs, list) else regs).context
    eip = ctx.eip
    base = 0
    try:  # the hook DLL logs its (relocated) base at startup
        import re
        m = re.search(r"dll base ([0-9A-Fa-f]{8})", open(r"E:\dev\csf-play\csf_hook.log").read())
        base = int(m.group(1), 16) if m else 0
    except OSError:
        pass
    print(f"DLLBASE {base:08X}")
    print(f"CRASH code={crashed.ExceptionCode:08X} eip={eip:08X} fault_addr="
          f"{(crashed.ExceptionInformation[1] if crashed.NumberParameters > 1 else 0):08X}")
    for n in ("eax", "ebx", "ecx", "edx", "esi", "edi", "ebp", "esp"):
        print(f"  {n}={getattr(ctx, n):08X}")
    a = eip
    for _ in range(8):
        ins = c.disassemble_at(a)
        if not ins:
            break
        print(f"  ASM {a:08X} {ins.instruction}")
        a += ins.instr_size
    esp = ctx.esp
    for i in range(32):
        try:
            v = c.read_dword(esp + 4 * i)
        except Exception:
            break
        print(f"  STK {esp + 4 * i:08X} {v:08X}")
c.terminate_session()
