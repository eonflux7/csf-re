#!/usr/bin/env bash
# Launch the play copy, take screenshots of the game window only at the given seconds ($PLAY/shots/tN.png, scaled
# to 960 wide), then kill it.
# Usage: shots.sh 10 20 30      (uses the deployed dinput8.dll and csf_hook.ini, incl. autopilot steps)
set -uo pipefail
PLAY=${CSF_PLAY:-${CSF_DATA_ROOT:-/mnt/e/dev}/csf-play}
WINPLAY=$(wslpath -w "$PLAY")
mkdir -p "$PLAY/shots"; rm -f "$PLAY/shots"/*.png
TIMES=$(IFS=,; echo "$*")
powershell.exe -NoProfile -Command "
Add-Type -AssemblyName System.Windows.Forms,System.Drawing
Add-Type -TypeDefinition 'using System; using System.Runtime.InteropServices; public struct R { public int L, T, Rt, B; } public static class W { [DllImport(\"user32.dll\")] public static extern bool GetWindowRect(IntPtr h, out R r); }'

\$p = Start-Process -FilePath '$WINPLAY\\CommXPC.exe' -WorkingDirectory '$WINPLAY' -PassThru $(if [ -n "${CSF_ARGS:-}" ]; then echo "-ArgumentList '$CSF_ARGS'"; fi)
\$t0 = Get-Date
foreach (\$t in @($TIMES)) {
  while (((Get-Date) - \$t0).TotalSeconds -lt \$t) { Start-Sleep -Milliseconds 200 }
  if (\$p.HasExited) { Write-Output \"exited at \$t s code \$(\$p.ExitCode)\"; break }
  \$p.Refresh(); \$r = New-Object R; [W]::GetWindowRect(\$p.MainWindowHandle, [ref]\$r) | Out-Null
  \$w = \$r.Rt - \$r.L; \$h = \$r.B - \$r.T; if (\$w -le 0 -or \$h -le 0) { continue }
  \$bmp = New-Object System.Drawing.Bitmap \$w, \$h
  \$g = [System.Drawing.Graphics]::FromImage(\$bmp); \$g.CopyFromScreen(\$r.L, \$r.T, 0, 0, \$bmp.Size)
  (New-Object System.Drawing.Bitmap \$bmp, 960, [int](960 * \$h / \$w)).Save('$WINPLAY\\shots\\t' + \$t + '.png'); \$g.Dispose(); \$bmp.Dispose()
}
if (-not \$p.HasExited) { Stop-Process -Id \$p.Id -Force }
" | tr -d '\r'
ls "$PLAY/shots"
