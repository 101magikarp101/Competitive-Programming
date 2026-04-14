#!/bin/sh
set -eu

echo Starting Codeforces Contest Wizard...

name="contest-wizard-starter-1.0-$(date +%s%N 2>/dev/null || date +%s).jar"
url="http://codeforces.com/codeforces.org/s/30723/wizard/contest-wizard-starter-1.0-SNAPSHOT.jar?session=fa89fe4434f25fd416ef15f973102d17"
tmp_dir="${TMPDIR:-${TEMP:-/tmp}}"

cd "$tmp_dir"

if command -v curl >/dev/null 2>&1; then
  curl -L --fail --output "$name" "$url"
elif command -v wget >/dev/null 2>&1; then
  wget --output-document="$name" "$url"
else
  echo "Missing downloader: install curl or wget." >&2
  exit 1
fi

exec java \
  --add-opens=java.base/java.lang=ALL-UNNAMED \
  --add-opens=java.base/java.lang.reflect=ALL-UNNAMED \
  --add-opens=java.desktop/javax.swing=ALL-UNNAMED \
  --add-opens=java.desktop/java.awt=ALL-UNNAMED \
  -cp "$name" \
  -Xss5m \
  -Xmx1024m \
  com.codeforces.contestwizard.Main \
  dGVzdGluZw \
  \# \
  MzAw \
  MTA2NDUwL3NhbmRib3gvY29udGVzdC56aXA \
  Km03YTFnNWk4azhhMXI0cDA \
  YnJfUjFjTU1jeV94V0RRSnA1WmhNMk53T3hJaDRxZzZBdUJkVnhoRVM4NGpQV3Z1dw \
  ZW4 \
  ZmFsc2U \
  \# \
  \# \
  \#
