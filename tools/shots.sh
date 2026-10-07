#!/usr/bin/env bash
# Launch the play copy, take screenshots at the given seconds (960x540 PNGs in $PLAY/shots/tN.png), then kill it.
# Usage: shots.sh 10 20 30      (uses the deployed dinput8.dll and csf_hook.ini, incl. autopilot steps)
set -uo pipefail
PLAY=${CSF_PLAY:-/mnt/e/dev/csf-play}
WINPLAY=$(wslpath -w "$PLAY")
mkdir -p "$PLAY/shots"; rm -f "$PLAY/shots"/*.png
TIMES=$(IFS=,; echo "$*")
powershell.exe -NoProfile -Command "
Add-Type -AssemblyName System.Windows.Forms,System.Drawing
\$p = Start-Process -FilePath '$WINPLAY\\CommXPC.exe' -WorkingDirectory '$WINPLAY' -PassThru $(if [ -n "${CSF_ARGS:-}" ]; then echo "-ArgumentList '$CSF_ARGS'"; fi)
\$t0 = Get-Date
foreach (\$t in @($TIMES)) {
  while (((Get-Date) - \$t0).TotalSeconds -lt \$t) { Start-Sleep -Milliseconds 200 }
  if (\$p.HasExited) { Write-Output \"exited at \$t s code \$(\$p.ExitCode)\"; break }
  \$b = [System.Windows.Forms.Screen]::PrimaryScreen.Bounds
  \$bmp = New-Object System.Drawing.Bitmap \$b.Width, \$b.Height
  \$g = [System.Drawing.Graphics]::FromImage(\$bmp); \$g.CopyFromScreen(\$b.Location, [System.Drawing.Point]::Empty, \$b.Size)
  (New-Object System.Drawing.Bitmap \$bmp, 960, 540).Save('$WINPLAY\\shots\\t' + \$t + '.png'); \$g.Dispose(); \$bmp.Dispose()
}
if (-not \$p.HasExited) { Stop-Process -Id \$p.Id -Force }
" | tr -d '\r'
ls "$PLAY/shots"
