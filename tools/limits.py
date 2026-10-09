"""Subscription / balance limits, read with the user's own tokens (read-only endpoints). Used by monitor.py and
by grunt.py lanes (Codex quota guard)."""
import json
import os
import urllib.request


CODEX_HOME_MAIN = os.path.expanduser("~/.codex")


def codex_homes():
    """Codex logins the grunts rotate over: the CLI's own ~/.codex, then every ~/.config/csf-re/codex-*/ holding an
    auth.json (log one in with `CODEX_HOME=~/.config/csf-re/codex-2 codex login`)."""
    import glob
    extra = sorted(os.path.dirname(p) for p in glob.glob(os.path.expanduser("~/.config/csf-re/codex-*/auth.json")))
    return [h for h in [CODEX_HOME_MAIN] + extra if os.path.exists(os.path.join(h, "auth.json"))]


def codex_label(home):
    return "codex" if home == CODEX_HOME_MAIN else os.path.basename(home)


def codex_usage(home=None):
    """5h / 7d windows of one ChatGPT subscription (default: ~/.codex), from the endpoint the Codex CLI's /status
    uses (own token). Returns {"5h": (used_percent, reset_after_seconds), "7d": (...)}."""
    a = json.load(open(os.path.join(home or CODEX_HOME_MAIN, "auth.json")))["tokens"]
    req = urllib.request.Request("https://chatgpt.com/backend-api/wham/usage", headers={
        "Authorization": f"Bearer {a['access_token']}", "ChatGPT-Account-Id": a.get("account_id") or "",
        "User-Agent": "codex_cli_rs"})
    d = json.load(urllib.request.urlopen(req, timeout=8))["rate_limit"]
    return {k: (d[w]["used_percent"], d[w]["reset_after_seconds"]) for k, w in
            (("5h", "primary_window"), ("7d", "secondary_window")) if d.get(w)}


def deepseek_balance():
    key = next(l.split("=", 1)[1].strip() for l in open(os.path.expanduser("~/.config/csf-re/env"))
               if l.startswith("DEEPSEEK_API_KEY="))
    req = urllib.request.Request("https://api.deepseek.com/user/balance",
                                 headers={"Authorization": f"Bearer {key}", "Accept": "application/json"})
    d = json.load(urllib.request.urlopen(req, timeout=8))
    return [(b["currency"], b["total_balance"]) for b in d.get("balance_infos", [])], d.get("is_available")
