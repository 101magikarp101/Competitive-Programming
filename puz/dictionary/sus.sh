#!/usr/bin/env bash

BASE_URL="https://ln-hunt.com/static/puzzle_resources/shiny-red-button/imaginary"

while IFS= read -r word; do
    # Lowercase the word
    w=$(echo "$word" | tr '[:upper:]' '[:lower:]')

    # Skip empty lines
    [ -z "$w" ] && continue

    # Check length (4–9)
    len=${#w}
    if (( len < 4 || len > 9 )); then
        continue
    fi

    url="${BASE_URL}/${w}.htm"
    echo "Trying: $url"

    # Check if the page exists (HTTP 200) using HEAD request
    if curl -fs --head "$url" >/dev/null 2>&1; then
        echo "FOUND VALID PAGE: $url"
        break
    fi

    # Wait 2 seconds before next word
    sleep 2
done < input.txt
