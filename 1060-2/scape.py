#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
animelyricsaz_aozora_scraper_paragraph.py

Scrapes https://animelyricsaz.com/research?term=aozora&s=lyrics,
visits each result, extracts the lyrics block, and returns ONLY the songs where
**some paragraph** ends with one of:
  - "aozora" (romaji, case-insensitive)
  - "青空" (kanji)
  - "あおぞら" (hiragana)
  - "アオゾラ" (katakana)

A "paragraph" is defined as a block of lines separated by one or more completely
blank lines (i.e., >= 1 empty line after normalizing <br> tags into newlines).

Outputs a CSV (and optional JSON) listing all matched songs, including the last
line of the matched paragraph (and a short paragraph snippet).

Usage:
  pip install requests beautifulsoup4
  python animelyricsaz_aozora_scraper_paragraph.py --term aozora --out results.csv

Optional rough "modern covers" proxy (needs YouTube Data API key):
  python animelyricsaz_aozora_scraper_paragraph.py --term aozora --out results.csv \
      --check-youtube 5 --youtube-api-key YOUR_KEY
"""

import argparse
import csv
import html
import json
import os
import re
import sys
import time
from typing import List, Dict, Optional
from urllib.parse import urljoin, urlencode

import requests
from bs4 import BeautifulSoup

BASE = "https://animelyricsaz.com"

HEADERS = {
    "User-Agent": "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 "
                  "(KHTML, like Gecko) Chrome/126.0 Safari/537.36"
}

FINAL_TOKENS = {"aozora", "青空", "あおぞら", "アオゾラ"}


def get(url: str, *, timeout: float = 20.0, retries: int = 2) -> requests.Response:
    last_err = None
    for attempt in range(retries + 1):
        try:
            resp = requests.get(url, headers=HEADERS, timeout=timeout)
            resp.raise_for_status()
            return resp
        except Exception as e:
            last_err = e
            if attempt < retries:
                time.sleep(1.0 * (attempt + 1))
            else:
                raise
    raise last_err  # type: ignore


def parse_search(term: str = "aozora", scope: str = "lyrics") -> List[Dict]:
    """
    Returns list of search results with minimal metadata:
    [ {title, url, snippet, category_text, artist}, ... ]
    """
    params = {"term": term, "s": scope}
    url = f"{BASE}/research?{urlencode(params)}"
    r = get(url)
    soup = BeautifulSoup(r.text, "html.parser")

    results: List[Dict] = []
    for a in soup.select("div.list-group a.list-group-item"):
        href = a.get("href", "")
        if not href:
            continue
        full_url = urljoin(BASE, href)

        title_el = a.select_one("h5")
        title = title_el.get_text(strip=True) if title_el else ""

        snippet_el = a.select_one("p.mb-1")
        snippet = snippet_el.get_text("\n", strip=True) if snippet_el else ""

        small_el = a.select_one("small.text-muted")
        category_text = small_el.get_text(" ", strip=True) if small_el else ""

        artist_el = a.select_one("small span.nameartiste")
        artist = artist_el.get_text(strip=True).replace("by ", "") if artist_el else ""

        results.append({
            "title": title,
            "url": full_url,
            "snippet": snippet,
            "category_text": category_text,
            "artist": artist,
        })
    return results


def extract_lyrics_html_text(html_text: str) -> str:
    """
    Pull the raw lyrics text (romaji block). The prompt indicates it's in:
    <div class="tab-pane tab-pane-margin active" id="p1"> ... with <br> line breaks.
    """
    soup = BeautifulSoup(html_text, "html.parser")
    tab = soup.select_one("div.tab-pane.tab-pane-margin.active#p1")
    if tab is None:
        tab = soup.select_one("div.tab-pane.tab-pane-margin")
    if tab is None:
        tab = soup.select_one("#p1")

    if tab is None:
        # Last resort: pick the largest text-bearing node.
        candidates = sorted(
            (el for el in soup.find_all(True)),
            key=lambda el: len(el.get_text(" ", strip=True)),
            reverse=True
        )
        if candidates:
            tab = candidates[0]
        else:
            return ""

    # Replace <br> with newline to preserve line structure.
    for br in tab.find_all("br"):
        br.replace_with("\n")

    text = tab.get_text("\n")
    text = html.unescape(text)
    return text


def clean_line(s: str) -> str:
    """
    Strip bracketed/parenthetical notes and trailing punctuation from a single line.
    """
    s = re.sub(r"\[[^\]]*\]", " ", s)      # remove [notes]
    s = re.sub(r"\([^)]+\)", " ", s)       # remove (notes)
    s = re.sub(r"\s+", " ", s).strip()
    s = re.sub(r"[\"'“”’!！\?？、。…⋯〜~\-—\u3000\s]+$", "", s)  # trim trailing punctuation
    return s.strip()


def normalize_token(s: str) -> str:
    """
    Normalize a token for comparison: lowercase and keep only romaji letters or Japanese chars.
    """
    s = s.strip().lower()
    s = re.sub(r"[^a-z\u3040-\u30ff\u4e00-\u9faf]", "", s)
    return s


def last_word_token(text: str) -> str:
    """
    Return the normalized token for the *last word* in text.
    """
    parts = text.split()
    if not parts:
        return ""
    return normalize_token(parts[-1])


def ends_with_aozora_word(text: str) -> bool:
    """
    Decide if 'text' ends with "aozora"/"青空"/"あおぞら"/"アオゾラ" as the final *word*.
    This is robust against trailing punctuation.
    """
    token = last_word_token(clean_line(text))
    return token in FINAL_TOKENS


def split_paragraphs(lyrics_text: str) -> List[str]:
    """
    Convert the lyrics block into paragraphs by collapsing Windows/Mac newlines to \n
    and splitting on blank lines (>= 1 empty line). We then keep non-empty paragraphs.
    """
    if not lyrics_text:
        return []
    t = lyrics_text.replace("\r\n", "\n").replace("\r", "\n")
    # Normalize multiple blank lines to paragraph boundaries
    paras = re.split(r"\n\s*\n+", t.strip())
    # Keep non-empty paragraphs only
    paras = [p.strip() for p in paras if p and p.strip()]
    return paras


def fetch_song_and_check(url: str) -> Dict:
    """
    Fetch a song page, split into paragraphs, and record any paragraph(s)
    that end on 'aozora' (in any supported script).
    """
    r = get(url)
    lyrics_text = extract_lyrics_html_text(r.text)
    paragraphs = split_paragraphs(lyrics_text)

    matched = []
    for p in paragraphs:
        # Determine end-of-paragraph by *last word* in the paragraph.
        if ends_with_aozora_word(p):
            # capture also the very last line for clarity
            last_line = ""
            lines = [ln.strip() for ln in p.split("\n")]
            lines = [ln for ln in lines if ln]
            if lines:
                last_line = clean_line(lines[-1])
            snippet = clean_line(p)
            if len(snippet) > 140:
                snippet = snippet[:137] + "…"
            matched.append({
                "paragraph_snippet": snippet,
                "last_line": last_line
            })

    result = {
        "paragraphs_checked": len(paragraphs),
        "matches": matched,
        "ends_with_aozora_paragraph": bool(matched),
    }
    return result


def estimate_youtube_covers(title: str, artist: str, api_key: str, years: int = 5) -> Optional[int]:
    """
    OPTIONAL: Uses YouTube Data API v3 to estimate how many "cover" videos exist
    in the last `years` years. Provide API key via --youtube-api-key to enable.
    Returns an integer count or None if it fails.
    """
    try:
        import datetime
        import requests as rq

        q = f"{title} {artist} カバー OR cover"
        published_after = (datetime.datetime.utcnow() - datetime.timedelta(days=365 * years)).isoformat("T") + "Z"
        params = {
            "part": "id",
            "q": q,
            "type": "video",
            "maxResults": 50,
            "publishedAfter": published_after,
            "key": api_key,
        }
        url = "https://www.googleapis.com/youtube/v3/search"
        resp = rq.get(url, params=params, timeout=20)
        if resp.status_code != 200:
            return None
        data = resp.json()
        total = len(data.get("items", []))
        return total
    except Exception:
        return None


def main():
    ap = argparse.ArgumentParser(description="Find songs on animelyricsaz where a paragraph ends with 'aozora/青空'.")
    ap.add_argument("--term", default="aozora", help="Search term (default: aozora)")
    ap.add_argument("--scope", default="lyrics", help="Search scope value for 's' parameter (default: lyrics)")
    ap.add_argument("--out", default="aozora_paragraph_results.csv", help="Output CSV path")
    ap.add_argument("--json", default=None, help="Optional JSON output path")
    ap.add_argument("--check-youtube", type=int, default=0, metavar="YEARS",
                    help="If >0, estimate modern covers via YouTube Data API (uploads in last N years)")
    ap.add_argument("--youtube-api-key", default=os.getenv("YOUTUBE_API_KEY"),
                    help="YouTube Data API key (env YOUTUBE_API_KEY is used if not provided)")
    args = ap.parse_args()

    print(f"[info] Searching animelyricsaz for term='{args.term}', scope='{args.scope}' ...")
    results = parse_search(args.term, args.scope)
    if not results:
        print("[warn] No search results found.")
        return

    print(f"[info] Found {len(results)} search results. Fetching song pages...")

    output_rows: List[Dict] = []
    for i, item in enumerate(results, start=1):
        url = item["url"]
        try:
            check = fetch_song_and_check(url)
        except Exception as e:
            print(f"[error] ({i}/{len(results)}) Failed: {url} :: {e}")
            continue

        first_match_line = check["matches"][0]["last_line"] if check["matches"] else ""
        first_match_snippet = check["matches"][0]["paragraph_snippet"] if check["matches"] else ""

        row = {
            "title": item["title"],
            "artist": item["artist"],
            "category_text": item["category_text"],
            "url": url,
            "paragraphs_checked": check["paragraphs_checked"],
            "match_found": bool(check["ends_with_aozora_paragraph"]),
            "match_last_line": first_match_line,
            "match_paragraph_snippet": first_match_snippet,
            "youtube_covers_recent": None,
        }

        # Optional proxy for "modern covers"
        if row["match_found"] and args.check_youtube > 0 and args.youtube_api_key:
            cnt = estimate_youtube_covers(item["title"], item["artist"], args.youtube_api_key, args.check_youtube)
            row["youtube_covers_recent"] = cnt

        if row["match_found"]:
            output_rows.append(row)

        print(f"[{i}/{len(results)}] {item['title']} — "
              f"{'MATCH' if row['match_found'] else 'no'}; "
              f"paragraphs={row['paragraphs_checked']}"
              + (f"; last='{row['match_last_line']}'" if row['match_found'] else ""))

    # Sort: anime-tagged first, then title
    def is_anime(cat: str) -> int:
        return 0 if ("anime" in cat.lower()) else 1

    output_rows.sort(key=lambda r: (is_anime(r.get("category_text", "")), r.get("title", "").lower()))

    # Write CSV
    fieldnames = [
        "title", "artist", "category_text", "url",
        "paragraphs_checked",
        "match_found", "match_last_line", "match_paragraph_snippet",
        "youtube_covers_recent"
    ]
    with open(args.out, "w", newline="", encoding="utf-8") as f:
        writer = csv.DictWriter(f, fieldnames=fieldnames)
        writer.writeheader()
        for row in output_rows:
            writer.writerow(row)

    # Optional JSON
    if args.json:
        with open(args.json, "w", encoding="utf-8") as jf:
            json.dump(output_rows, jf, ensure_ascii=False, indent=2)

    print(f"[done] Wrote {len(output_rows)} matches to {args.out}")
    if args.json:
        print(f"[done] JSON saved to {args.json}")


if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        print("\n[warn] Interrupted by user.", file=sys.stderr)
        sys.exit(130)
