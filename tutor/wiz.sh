#!/bin/sh
set -eu

echo "Starting Codeforces Contest Wizard..."

tmp="${TMPDIR:-/tmp}"
cd "$tmp"

name="contest-wizard-starter-1.0-$(date +%s).jar"
url="https://codeforces.com/codeforces.org/s/18226/wizard/contest-wizard-starter-1.0-SNAPSHOT.jar?session=5d3409ffef3b3efc094cd7c52139fa4c"

wget -O "$name" "$url"

java \
  --add-opens=java.desktop/javax.swing=ALL-UNNAMED \
  --add-opens=java.desktop/javax.swing.plaf=ALL-UNNAMED \
  -Xss5m -Xmx1024m \
  -cp "$name" \
  com.codeforces.contestwizard.Main \
  VHV0b3JpbmcgRXhhbXBsZSBQcm9ibGVtcw \
  "#" \
  NjA \
  MTA2MjY0L3NhbmRib3gvY29udGVzdC56aXA \
  Km03YTFnNWk4azhhMXI0cDA \
  YnJfUjFjTU1jeV94V0RRSnA1WmhNMk53T3hJYktRNHh0V1ByV2RxSXREbVRlakFBUQ \
  ZW4 \
  dHJ1ZQ \
  Qg \
  "#" \
  "#"