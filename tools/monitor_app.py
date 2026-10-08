#!/usr/bin/env python3
"""Textual dashboard (tools/monitor.py starts it; `monitor.py --once` prints one Rich frame instead).

Tabs: 1 Live (Claude | Costs, Progress, grunt with scrollable in-flight/results tables, captain, game/tools),
2 Modules (per-module prep/progress table, click a header to sort), 3 Plan (current focus + every phase as
collapsible sections), 4 Commits (this session), 5 Queue (escalations in queue/, the selected one previewed). All data comes from the helpers in monitor.py, read in a worker thread every
--interval seconds so slow sources (tasklist, usage endpoints) never block the UI.
"""
import glob, os, re, time

from rich.console import Group
from rich.text import Text
from textual import work
from textual.app import App, ComposeResult
from textual.binding import Binding
from textual.coordinate import Coordinate
from textual.containers import Horizontal, Vertical, VerticalScroll
from textual.widgets import Collapsible, DataTable, Footer, Header, Markdown, Static, TabbedContent, TabPane

import monitor as M

CSS = """
#top { height: auto; }
#claude { width: 1fr; height: auto; border: round green; padding: 0 1; }
#costs { width: 1fr; min-width: 44; height: auto; border: round cyan; padding: 0 1; }
#progress { height: auto; border: round yellow; padding: 0 1; }
#tools { height: 1; padding: 0 1; }
#modtable { height: 1fr; }
#grunt { height: 1fr; border: round magenta; padding: 0 1; }
#ghead { height: auto; }
#inflight { height: auto; max-height: 9; }
#results { height: 1fr; min-height: 3; border-top: solid $panel; }
#captain { height: auto; border: round $primary; padding: 0 1; }
DataTable { scrollbar-size-vertical: 1; }
#plan Collapsible { padding: 0; }
#queue { width: 2fr; }
#qview { width: 3fr; border-left: solid $panel; padding: 0 1; }
"""


MOD_COLS = [("id", "module"), ("name", "name"), ("fns", "fns"), ("kb", "KB"), ("classes", "classes"),
            ("cls_open", "unnamed"), ("cls_held", "held"), ("prepped", "prepped"), ("named", "fn named"),
            ("ph", "placeh."), ("done", "done"), ("pct", "done %B"), ("exe", "executed"), ("diff", "diffed"),
            ("esc", "esc")]


def mod_cells(r):
    """A Modules-tab row: prep = every class named (or held after a captain session); % = bytes done."""
    pct = r["pct"]
    return (r["id"], Text(r["name"] or "—", style="" if r["name"] else "dim"), str(r["fns"]), f"{r['kb']:.1f}",
            str(r["classes"]), Text(str(r["cls_open"]), style="yellow" if r["cls_open"] else "dim"),
            Text(str(r["cls_held"]), style="dim"), Text("✓" if r["prepped"] else "·",
                                                       style="green" if r["prepped"] else "dim"),
            str(r["named"]), Text(str(r["ph"]), style="yellow" if r["ph"] else "dim"), str(r["done"]),
            Text(f"{pct:5.1f}", style="bold green" if pct >= 90 else "green" if pct >= 25 else "" if pct else "dim"),
            Text(str(r["exe"]), style="cyan"), Text(str(r["diff"]), style="cyan"),
            Text(str(r["esc"]), style="red" if r["esc"] else "dim"))


class Monitor(App):
    TITLE = "csf-re monitor"
    CSS = CSS
    BINDINGS = [
        Binding("1", "tab('live')", "Live"),
        Binding("2", "tab('modules')", "Modules"),
        Binding("3", "tab('plan')", "Plan"),
        Binding("4", "tab('commits')", "Commits"),
        Binding("5", "tab('queue')", "Queue"),
        Binding("r", "refresh_now", "Refresh"),
        Binding("q", "quit", "Quit"),
    ]

    def __init__(self, interval=2.0):
        super().__init__()
        self.interval = interval
        self.plan_mtime = 0
        self.queue_files = []
        self.mod_sort = ("pct", True)

    def compose(self) -> ComposeResult:
        yield Header(show_clock=True)
        with TabbedContent(initial="live"):
            with TabPane("Live", id="live"):
                with Horizontal(id="top"):
                    yield Static(id="claude")
                    yield Static(id="costs")
                yield Static(id="progress")
                with Vertical(id="grunt"):
                    yield Static(id="ghead")
                    yield DataTable(id="inflight", cursor_type="none", zebra_stripes=False)
                    yield DataTable(id="results", cursor_type="row")
                yield Static(id="captain")
                yield Static(id="tools")
            with TabPane("Modules", id="modules"):
                yield DataTable(id="modtable", cursor_type="row", zebra_stripes=True)
            with TabPane("Plan", id="plan"):
                yield VerticalScroll(id="planbody")
            with TabPane("Commits", id="commits"):
                yield DataTable(id="commitlist", cursor_type="row")
            with TabPane("Queue", id="queue"):
                with Horizontal():
                    yield DataTable(id="queuelist", cursor_type="row")
                    with VerticalScroll(id="qview"):
                        yield Markdown(id="qmd")
        yield Footer()

    def on_mount(self):
        self.query_one("#inflight", DataTable).add_columns("ea", "model", "age", "kind", "function")
        self.query_one("#results", DataTable).add_columns("age", "", "by", "tags", "ea", "function / error")
        self.query_one("#commitlist", DataTable).add_columns("age", "commit", "subject")
        self.query_one("#queuelist", DataTable).add_columns("ea", "age", "reason")
        self.query_one("#grunt").border_title = "Grunt"
        self.query_one("#progress").border_title = "Progress"
        self.query_one("#costs").border_title = "Costs · usage · logins"
        mt = self.query_one("#modtable", DataTable)
        for key, label in MOD_COLS:
            mt.add_column(label, key=key)
        self.collect()
        self.set_interval(self.interval, self.collect)

    def action_tab(self, tab):
        self.query_one(TabbedContent).active = tab

    def action_refresh_now(self):
        M._cache.clear()
        self.collect()

    # ------------------------------------------------------------ data (worker thread)

    @work(thread=True, exclusive=True)
    def collect(self):
        g = M.grunt_snapshot()
        claude = M.panel_claude()
        snap = dict(
            claude=(claude.renderable, claude.title), progress=M.panel_progress().renderable,
            costs=M.panel_costs(g["st"], g["recent"]).renderable, modules=M.module_rows(),
            ghead=Group(*M.grunt_header(g)), calls=M.call_rows(g), results=M.result_rows(g, 300),
            captain=M.captain_lines(), tools=M.tool_line(),
            commits=self.commit_rows(), queue=self.queue_rows())
        p = os.path.join(M.ROOT, "docs", "PLAN.md")
        mt = os.path.getmtime(p)
        if mt != self.plan_mtime:
            snap["plan"] = (mt, M.focus(), M.open_items(), M.plan())
        self.call_from_thread(self.apply, snap)

    def commit_rows(self):
        jobs = M.cached("jobs", 1, M.claude_jobs)
        since = min(M.iso(j["createdAt"]) for j in jobs) if isinstance(jobs, list) and jobs else time.time() - 86400
        cm = M.cached("commits", 10, lambda: M.commits(since))
        return [(Text(M.ago(int(ct)), style="dim"), Text(h, style="yellow"), s) for ct, h, s in
                (cm if isinstance(cm, list) else [])]

    def queue_rows(self):
        rows = []
        for f in sorted(glob.glob(os.path.join(M.ROOT, "queue", "*.md")), key=os.path.getmtime, reverse=True):
            try:
                lines = open(f).read(4000).splitlines()
            except OSError:
                continue
            m = re.match(r"# Escalation 0x([0-9A-Fa-f]+):?\s*(.*)", lines[0] if lines else "")
            why = m.group(2) if m else ""
            if not why:  # gate escalations: the first line of the last gate error
                body = [l for l in lines[1:] if l.strip() and not l.startswith(("#", "```"))]
                why = "gate: " + (body[1] if len(body) > 1 and body[0].endswith(":") else body[0] if body else "?")
            rows.append((f, m.group(1).lower().zfill(8) if m else os.path.basename(f)[:8],
                         M.ago(os.path.getmtime(f)), why[:160]))
        return rows

    # ------------------------------------------------------------ apply (UI thread)

    def fill(self, table, rows):
        """Show rows without disturbing the reader: unchanged data = no-op; same row count = only the changed cells
        are updated in place (scroll and cursor untouched); otherwise rebuilt, then cursor and scroll restored
        after the layout pass (restoring before it clamps the scroll to 0)."""
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
        cur, sx, sy = table.cursor_row, table.scroll_x, table.scroll_y
        table.clear()
        for r in rows:
            table.add_row(*r)

        def restore():
            if rows and table.cursor_type != "none":
                table.move_cursor(row=min(cur, len(rows) - 1), animate=False, scroll=False)
            table.scroll_to(x=sx, y=sy, animate=False)
        self.call_after_refresh(restore)

    def apply(self, s):
        c = self.query_one("#claude", Static)
        c.update(s["claude"][0])
        c.border_title = s["claude"][1]
        self.query_one("#progress", Static).update(s["progress"])
        self.query_one("#ghead", Static).update(s["ghead"])
        inf = self.query_one("#inflight", DataTable)
        inf.display = bool(s["calls"])
        self.fill(inf, s["calls"])
        res = self.query_one("#results", DataTable)
        self.fill(res, s["results"])
        res.border_title = f"results ({len(s['results'])})"
        cap = self.query_one("#captain", Static)
        cap.display = bool(s["captain"])
        if s["captain"]:
            cap.update(Group(*s["captain"]))
            cap.border_title = "Captain"
        self.query_one("#tools", Static).update(s["tools"])
        self.query_one("#costs", Static).update(s["costs"])
        self.mod_data = s["modules"]
        self.fill_modules()
        self.fill(self.query_one("#commitlist", DataTable), s["commits"])
        q = self.query_one("#queuelist", DataTable)
        self.queue_files = [r[0] for r in s["queue"]]
        self.fill(q, [r[1:] for r in s["queue"]])
        self.query_one(TabbedContent).get_tab("queue").label = f"Queue ({len(s['queue'])})"
        if "plan" in s:
            self.build_plan(*s["plan"])

    def fill_modules(self):
        key, rev = self.mod_sort
        rows = sorted(getattr(self, "mod_data", []), key=lambda r: (r[key] if r[key] is not None else 0), reverse=rev)
        self.fill(self.query_one("#modtable", DataTable), [mod_cells(r) for r in rows])

    def on_data_table_header_selected(self, ev):
        if ev.data_table.id != "modtable":
            return
        k = ev.column_key.value
        self.mod_sort = (k, not self.mod_sort[1] if self.mod_sort[0] == k else k not in ("id", "name"))
        self.fill_modules()

    def build_plan(self, mtime, focus, phases, current):
        self.plan_mtime = mtime
        body = self.query_one("#planbody", VerticalScroll)
        body.remove_children()
        cur = current[0] if isinstance(current, tuple) else ""
        widgets = [Collapsible(*[Static(Text(f"{n}. {t}")) for n, t in focus], title="Current focus",
                               collapsed=False)]
        for name, items in phases:
            widgets.append(Collapsible(*[Static(Text("☐ " + it)) for it in items],
                                       title=f"{name}  ({len(items)} open)", collapsed=not cur.startswith(name)))
        body.mount(*widgets)

    def on_data_table_row_highlighted(self, ev):
        if ev.data_table.id != "queuelist" or ev.cursor_row is None or ev.cursor_row >= len(self.queue_files):
            return
        try:
            txt = open(self.queue_files[ev.cursor_row]).read()
        except OSError:
            txt = "(gone)"
        self.query_one("#qmd", Markdown).update(txt[:20000])


def run(interval=2.0):
    Monitor(interval).run()
