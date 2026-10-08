#!/usr/bin/env python3
"""Temporary dashboard for `grunt.py ab` runs (work/ab/): per track a summary per arm and a function x arm grid.

Cells: ✓N = passed on attempt N, ✗ = failed, … = queued/in flight (an `ab` process for that arm is running),
blank = not part of a run. Track b also shows the code similarity to the committed Codex file (~0.82; = for
identical code) and Codex's logged attempt as the baseline column.

Usage: ab_monitor.py [--interval 3]      keys: a / b switch track, r refresh, q quit
"""
import argparse, json, os, re, statistics, subprocess

from rich.text import Text
from textual.app import App, ComposeResult
from textual.binding import Binding
from textual.coordinate import Coordinate
from textual.widgets import DataTable, Footer, Header, Static, TabbedContent, TabPane

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
AB = os.path.join(ROOT, "work", "ab")

CSS = """
.summary { height: auto; max-height: 12; border: round cyan; }
.grid { height: 1fr; border: round magenta; }
.log { height: 8; border: round $panel; padding: 0 1; }
"""


def running():
    """{track: [arms]} of live `grunt.py ab` processes."""
    out = subprocess.run(["pgrep", "-af", "[g]runt.py ab "], capture_output=True, text=True).stdout
    live = {}
    for line in out.splitlines():
        t, a = re.search(r"--track (\w)", line), re.search(r"--arms (\S+)", line)
        if t and a and "python" in line.split()[1]:
            live.setdefault(t.group(1), []).extend(a.group(1).split(","))
    return live


def load(track):
    p = os.path.join(AB, track)
    try:
        fns = json.load(open(os.path.join(p, "set.json")))["fns"]
    except OSError:
        return [], []
    rs = []
    if os.path.exists(os.path.join(p, "results.jsonl")):
        for line in open(os.path.join(p, "results.jsonl")):
            try:
                rs.append(json.loads(line))
            except json.JSONDecodeError:
                pass  # a line being appended right now
    return fns, rs


def summary_rows(track, fns, rs, arms, live):
    rows = []
    if track == "b" and fns:
        n = len(fns)
        rows.append((Text("codex gpt-6.1-sol (log)", style="dim"), f"{n}/{n}",
                     f"{sum(f['codex_attempt'] == 1 for f in fns) / n:.0%}", "quota", "quota",
                     str(sum(f["codex_out"] for f in fns) // n), "", "1.00", ""))
    for arm in arms:
        a = [r for r in rs if r["arm"] == arm]
        ok = [r for r in a if r["ok"]]
        cost = sum(r.get("cost") or 0 for r in a)
        outt = sum(u.get("completion_tokens", 0) or 0 for r in a for u in r["usage"])
        sims = [r["sim"] for r in ok if r.get("sim") is not None]
        state = Text("running", style="bold green") if arm in live else Text("done" if len(a) >= len(fns) else
                                                                              "stopped", style="dim")
        rows.append((Text(arm, style="bold"), f"{len(ok)}/{len(a)}" + (f" of {len(fns)}" if len(a) < len(fns) else ""),
                     f"{sum(r['attempts'] == 1 for r in ok) / max(1, len(a)):.0%}", f"{cost:.3f}",
                     f"{cost / max(1, len(ok)):.4f}", str(outt // max(1, len(a))),
                     f"{statistics.median(r['secs'] for r in a):.0f}" if a else "",
                     f"{statistics.mean(sims):.2f} ({sum(s == 1.0 for s in sims)}=)" if sims else "", state))
    return rows


def grid_rows(track, fns, rs, arms, live):
    by = {(r["arm"], r["ea"]): r for r in rs}
    size = {r["ea"]: r["size"] for r in rs}
    rows = []
    for f in sorted(fns, key=lambda f: size.get(f["ea"], 1 << 30)):
        ea = f["ea"]
        base = [f"{ea:08x}", str(size.get(ea, ""))]
        if track == "b":
            base += [f.get("kind", ""), f"✓{f['codex_attempt']}"]
        cells = []
        for arm in arms:
            r = by.get((arm, ea))
            if r is None:
                cells.append(Text("…", style="yellow") if arm in live else Text(""))
            elif r["ok"]:
                sim = r.get("sim")
                t = Text(f"✓{r['attempts']}", style="green" if r["attempts"] == 1 else "yellow")
                t.append(f" ${r.get('cost') or 0:.4f} {r['secs']:.0f}s", style="dim")
                if sim is not None:
                    t.append(" =" if sim == 1.0 else f" ~{sim:.2f}", style="cyan" if sim >= 0.8 else "magenta")
                cells.append(t)
            else:
                cells.append(Text(f"✗ {(r.get('err') or '').splitlines()[0][:40] if r.get('err') else ''}",
                                  style="red"))
        rows.append(tuple(base + cells))
    return rows


def log_tail(track, n=6):
    lines = []
    for f in sorted(os.listdir(AB)):
        if f.startswith(f"track_{track}") and f.endswith(".log"):
            lines += [l for l in open(os.path.join(AB, f)).read().splitlines() if l.startswith("  ")]
    return "\n".join(lines[-n:])


class ABMonitor(App):
    TITLE = "grunt A/B"
    CSS = CSS
    BINDINGS = [Binding("a", "tab('a')", "Track a"), Binding("b", "tab('b')", "Track b"),
                Binding("r", "refresh_now", "Refresh"), Binding("q", "quit", "Quit")]

    def __init__(self, interval):
        super().__init__()
        self.interval = interval
        self.cols = {}

    def compose(self) -> ComposeResult:
        yield Header(show_clock=True)
        with TabbedContent(initial="b"):
            for t, title in (("a", "a · DeepSeek lane"), ("b", "b · Codex lane")):
                with TabPane(title, id=t):
                    yield DataTable(id=f"sum{t}", classes="summary", cursor_type="none")
                    yield DataTable(id=f"grid{t}", classes="grid", cursor_type="row", zebra_stripes=True)
                    yield Static(id=f"log{t}", classes="log")
        yield Footer()

    def on_mount(self):
        for t in "ab":
            self.query_one(f"#sum{t}", DataTable).add_columns(
                "arm", "ok", "1st try", "$ total", "$ / ok", "out tok/fn", "median s", "sim", "state")
            self.query_one(f"#sum{t}").border_title = "Summary"
            self.query_one(f"#grid{t}").border_title = "Functions × arms"
            self.query_one(f"#log{t}").border_title = "Latest"
        self.tick()
        self.set_interval(self.interval, self.tick)

    def action_tab(self, t):
        self.query_one(TabbedContent).active = t

    def action_refresh_now(self):
        self.tick()

    def fill(self, table, rows):
        """Same row count: update changed cells in place (scroll/cursor kept); otherwise rebuild."""
        plain = [tuple(str(c) for c in r) for r in rows]
        old = getattr(table, "_plain", None)
        if plain == old:
            return
        table._plain = plain
        if old is not None and len(old) == len(plain) and table.row_count == len(plain):
            for y, (a, b) in enumerate(zip(old, plain)):
                for x, (ca, cb) in enumerate(zip(a, b)):
                    if ca != cb:
                        table.update_cell_at(Coordinate(y, x), rows[y][x])
            return
        table.clear()
        for r in rows:
            table.add_row(*r)

    def tick(self):
        live = running()
        for t in "ab":
            fns, rs = load(t)
            arms = list(dict.fromkeys([r["arm"] for r in rs] + live.get(t, [])))
            grid = self.query_one(f"#grid{t}", DataTable)
            cols = ["function", "size"] + (["kind", "codex"] if t == "b" else []) + arms
            if self.cols.get(t) != cols:  # a new arm: rebuild the columns
                grid.clear(columns=True)
                grid.add_columns(*cols)
                grid._plain = None
                self.cols[t] = cols
            self.fill(self.query_one(f"#sum{t}", DataTable), summary_rows(t, fns, rs, arms, live.get(t, [])))
            self.fill(grid, grid_rows(t, fns, rs, arms, live.get(t, [])))
            done = sum(1 for r in rs if r["arm"] in arms)
            self.query_one(f"#grid{t}").border_title = f"Functions × arms  ({done}/{len(fns) * len(arms)} runs)"
            self.query_one(f"#log{t}", Static).update(log_tail(t))


if __name__ == "__main__":
    ap = argparse.ArgumentParser()
    ap.add_argument("--interval", type=float, default=3)
    ABMonitor(ap.parse_args().interval).run()
