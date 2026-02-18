#!/usr/bin/env python3
"""
Build a Caldecott (Medal + Honors) PDF with actual book jacket images.

Sources:
- ALSC official PDF list (1938–2023): https://www.ala.org/sites/default/files/alsc/content/awardsgrants/bookmedia/caldecottmedal/caldecott-medal-honors-to-present.pdf
- ALA news release 2024: https://www.ala.org/news/2024/01/dave-eggers-vashti-harrison-win-newbery-caldecott-medals
- ALA news release 2025: https://www.ala.org/news/2025/01/american-library-association-announces-2025-youth-media-award-winners

Covers:
- Open Library Search API: https://openlibrary.org/dev/docs/api/search
- Open Library Covers API: https://openlibrary.org/dev/docs/api/covers
"""

from __future__ import annotations

import argparse
import csv
import hashlib
import io
import os
import re
import sys
import time
from dataclasses import dataclass
from typing import Dict, List, Optional, Tuple
from concurrent.futures import ThreadPoolExecutor, as_completed

import requests
from bs4 import BeautifulSoup
from pypdf import PdfReader
from PIL import Image as PILImage

from reportlab.lib.pagesizes import letter
from reportlab.lib.units import inch
from reportlab.lib import utils
from reportlab.platypus import (
    SimpleDocTemplate, Paragraph, Spacer, Image, Table, TableStyle, PageBreak
)
from reportlab.lib.styles import getSampleStyleSheet
from reportlab.lib.enums import TA_CENTER
from reportlab.lib.colors import black, lightgrey


ALSC_PDF_URL = (
    "https://www.ala.org/sites/default/files/alsc/content/awardsgrants/bookmedia/"
    "caldecottmedal/caldecott-medal-honors-to-present.pdf"
)

ALA_2024_URL = "https://www.ala.org/news/2024/01/dave-eggers-vashti-harrison-win-newbery-caldecott-medals"
ALA_2025_URL = "https://www.ala.org/news/2025/01/american-library-association-announces-2025-youth-media-award-winners"

# ---------- Models ----------

@dataclass
class BookEntry:
    title: str
    raw: str = ""
    illustrator: str = ""
    author: str = ""
    award: str = ""  # "Medal" or "Honor"

@dataclass
class YearRecord:
    year: int
    medal: Optional[BookEntry]
    honors: List[BookEntry]


# ---------- Utilities ----------

def ensure_dir(p: str) -> None:
    os.makedirs(p, exist_ok=True)

def slug_key(s: str) -> str:
    s2 = re.sub(r"\s+", " ", s.strip().lower())
    return hashlib.sha256(s2.encode("utf-8")).hexdigest()[:24]

def normalize_title(s: str) -> str:
    s = s.lower().strip()
    s = re.sub(r"[\u2018\u2019]", "'", s)  # curly apostrophes
    s = re.sub(r"[\u201c\u201d]", '"', s)  # curly quotes
    s = re.sub(r"[^a-z0-9]+", " ", s)
    s = re.sub(r"\s+", " ", s).strip()
    return s

def parse_creators_from_raw(raw: str) -> Tuple[str, str]:
    """
    Heuristic parser for 'illustrated by ...' and 'written by ...' patterns.
    Returns (illustrator, author).
    """
    illustrator = ""
    author = ""

    m = re.search(r"illustrated by\s+([^,;()]+)", raw, flags=re.IGNORECASE)
    if m:
        illustrator = m.group(1).strip()

    m = re.search(r"written by\s+([^,;()]+)", raw, flags=re.IGNORECASE)
    if m:
        author = m.group(1).strip()

    # Handle "text:" patterns
    m = re.search(r"text:\s*([^,;()]+)", raw, flags=re.IGNORECASE)
    if m and not author:
        author = m.group(1).strip()

    # Fallback: first " by " after title (often author/illustrator)
    if not illustrator and not author:
        parts = raw.split(" (", 1)[0]  # strip publisher
        # Try: "<Title> by <Name...>"
        if " by " in parts:
            try:
                after = parts.split(" by ", 1)[1].strip()
                # Might include "Illustrated by ..." etc; keep only first chunk
                after = re.split(r"[;,]", after)[0].strip()
                author = after
            except Exception:
                pass

    return illustrator, author

def parse_title_from_raw(raw: str) -> str:
    """
    Heuristic: title is before ' by ' or before ' illustrated' or before '('.
    """
    s = raw.strip()
    s = re.sub(r"\s+", " ", s)
    # Remove publisher tail
    s_no_pub = s.split(" (", 1)[0].strip()

    # Prefer split by " by "
    if " by " in s_no_pub:
        t = s_no_pub.split(" by ", 1)[0].strip()
        return t.rstrip(" ,;:")

    # Or split by " illustrated"
    m = re.search(r"\sillustrated\b", s_no_pub, flags=re.IGNORECASE)
    if m:
        t = s_no_pub[: m.start()].strip()
        return t.rstrip(" ,;:")

    return s_no_pub.rstrip(" ,;:")

def http_get(url: str, timeout: int = 30, headers: Optional[dict] = None) -> requests.Response:
    hdrs = {
        "User-Agent": "caldecott-pdf-builder/1.0 (personal-use; contact: none)",
        "Accept": "*/*",
    }
    if headers:
        hdrs.update(headers)
    r = requests.get(url, timeout=timeout, headers=hdrs)
    r.raise_for_status()
    return r


# ---------- Parsing ALSC official PDF (1938–2023) ----------

def download_cached(url: str, cache_path: str) -> str:
    ensure_dir(os.path.dirname(cache_path))
    if os.path.exists(cache_path) and os.path.getsize(cache_path) > 0:
        return cache_path
    r = http_get(url)
    with open(cache_path, "wb") as f:
        f.write(r.content)
    return cache_path

def extract_pdf_text_lines(pdf_path: str) -> List[str]:
    reader = PdfReader(pdf_path)
    lines: List[str] = []
    for page in reader.pages:
        txt = page.extract_text() or ""
        # Keep page-level line breaks, but normalize weird whitespace
        for ln in txt.splitlines():
            ln = ln.replace("\xa0", " ").strip()
            if ln:
                lines.append(ln)
    return lines

def preprocess_lines_for_headings(text: str) -> str:
    """
    Insert newlines before headings that sometimes appear mid-line.
    """
    # Before "YYYY Medal Winner"
    text = re.sub(r"(?<!\n)(\b(19|20)\d{2}\s+Medal Winner\b)", r"\n\1", text)
    # Before "YYYY Honor Book(s)"
    text = re.sub(r"(?<!\n)(\b(19|20)\d{2}\s+Honor Books?\b)", r"\n\1", text)
    # Before "Honor Book(s):" if stuck to previous text
    text = re.sub(r"(?<!\n)(\bHonor Books?\b)", r"\n\1", text)
    return text

def parse_alsc_pdf_records(lines: List[str]) -> Dict[int, YearRecord]:
    """
    Parse ALSC PDF text into structured YearRecord objects.
    """
    blob = "\n".join(lines)
    blob = preprocess_lines_for_headings(blob)
    work_lines = [ln.strip() for ln in blob.splitlines() if ln.strip()]

    year_re_medal = re.compile(r"^((19|20)\d{2})\s+Medal Winner\b:?$", re.IGNORECASE)
    year_re_honors = re.compile(r"^((19|20)\d{2})\s+Honor Books?\b:?$", re.IGNORECASE)
    honors_heading = re.compile(r"^Honor Books?\b:?$", re.IGNORECASE)

    records: Dict[int, YearRecord] = {}

    current_year: Optional[int] = None
    expecting: Optional[str] = None  # "medal" or "honor"
    buf: List[str] = []

    def flush_buf() -> None:
        nonlocal buf, expecting, current_year
        if not buf or current_year is None or expecting is None:
            buf = []
            return
        entry_raw = " ".join(buf).strip()
        buf = []

        title = parse_title_from_raw(entry_raw)
        illustrator, author = parse_creators_from_raw(entry_raw)
        be = BookEntry(
            title=title,
            raw=entry_raw,
            illustrator=illustrator,
            author=author,
            award=("Medal" if expecting == "medal" else "Honor"),
        )
        yr = records.get(current_year)
        if not yr:
            yr = YearRecord(year=current_year, medal=None, honors=[])
            records[current_year] = yr

        if expecting == "medal":
            yr.medal = be
        else:
            yr.honors.append(be)

    for ln in work_lines:
        m = year_re_medal.match(ln)
        if m:
            flush_buf()
            current_year = int(m.group(1))
            if current_year not in records:
                records[current_year] = YearRecord(year=current_year, medal=None, honors=[])
            expecting = "medal"
            continue

        m = year_re_honors.match(ln)
        if m:
            flush_buf()
            current_year = int(m.group(1))
            if current_year not in records:
                records[current_year] = YearRecord(year=current_year, medal=None, honors=[])
            expecting = "honor"
            continue

        if honors_heading.match(ln):
            flush_buf()
            if current_year is not None:
                expecting = "honor"
            continue

        # Accumulate a book line (wrapping can occur)
        if expecting in ("medal", "honor") and current_year is not None:
            buf.append(ln)
            # Most entries end with a publisher parenthesis; flush on that.
            if re.search(r"\)\.?\s*$", ln):
                flush_buf()

    flush_buf()
    return records


# ---------- Parsing ALA news pages for 2024/2025 (supplements beyond ALSC PDF) ----------

def parse_ala_2024() -> YearRecord:
    html = http_get(ALA_2024_URL).text
    # Extract Caldecott medal title in curly quotes
    # Example: ... awarded ... for “Big,” ...
    medal_title = ""
    honors: List[str] = []

    # Medal
    m = re.search(r"2024\s+Caldecott\s+Medal.*?for\s+“([^”]+)”", html, flags=re.IGNORECASE | re.DOTALL)
    if m:
        medal_title = m.group(1).strip()

    # Honors: quoted titles following "Four Caldecott Honor Books were named:"
    m = re.search(r"Four\s+Caldecott\s+Honor\s+Books\s+were\s+named:(.*?)(Members\s+of\s+the\s+2024|$)",
                  html, flags=re.IGNORECASE | re.DOTALL)
    if m:
        chunk = m.group(1)
        honors = [t.strip() for t in re.findall(r"“([^”]+)”", chunk)]

    yr = YearRecord(
        year=2024,
        medal=BookEntry(title=medal_title, raw="(ALA news release)", award="Medal") if medal_title else None,
        honors=[BookEntry(title=t, raw="(ALA news release)", award="Honor") for t in honors],
    )
    return yr

def parse_ala_2025() -> YearRecord:
    html = http_get(ALA_2025_URL).text

    medal_title = ""
    honor_titles: List[str] = []

    # Medal line: “Chooch Helped,” ...
    m = re.search(r"Randolph\s+Caldecott\s+Medal.*?:\s*“([^”]+)”", html,
                  flags=re.IGNORECASE | re.DOTALL)
    if m:
        medal_title = m.group(1).strip()

    # Honor line: Caldecott Honor Books ... named: "A"; "B"; "C"; and "D".
    m = re.search(r"Caldecott\s+Honor\s+Books.*?named:(.*?)(Coretta\s+Scott\s+King|$)",
                  html, flags=re.IGNORECASE | re.DOTALL)
    if m:
        chunk = m.group(1)
        honor_titles = [t.strip() for t in re.findall(r"\"([^\"]+)\"", chunk)]
        # The ALA page uses straight quotes for the honor list; keep as-is.

    yr = YearRecord(
        year=2025,
        medal=BookEntry(title=medal_title, raw="(ALA news release)", award="Medal") if medal_title else None,
        honors=[BookEntry(title=t, raw="(ALA news release)", award="Honor") for t in honor_titles],
    )
    return yr


# ---------- Covers (Open Library first, then optional Wikipedia fallback) ----------

def openlibrary_find_cover_url(title: str, author_hint: str = "", timeout: int = 20) -> Optional[str]:
    params = {"title": title, "limit": 10}
    if author_hint.strip():
        params["author"] = author_hint.strip()

    r = requests.get(
        "https://openlibrary.org/search.json",
        params=params,
        timeout=timeout,
        headers={"User-Agent": "caldecott-pdf-builder/1.0"}
    )
    if r.status_code != 200:
        return None
    data = r.json()
    docs = data.get("docs", [])
    best = None

    nt = normalize_title(title)

    def score(doc: dict) -> float:
        dt = normalize_title(doc.get("title", "") or "")
        s = 0.0
        if dt:
            # rough similarity
            s += 2.5 * (1.0 if dt == nt else _ratio(dt, nt))
        if author_hint:
            a = (doc.get("author_name") or [""])[0]
            s += 0.6 * _ratio(a.lower(), author_hint.lower())
        if doc.get("cover_i") is not None:
            s += 1.0
        return s

    for doc in docs:
        if best is None or score(doc) > score(best):
            best = doc

    if not best:
        return None

    cover_i = best.get("cover_i")
    if cover_i:
        return f"https://covers.openlibrary.org/b/id/{cover_i}-L.jpg"

    # Try ISBN fallback
    isbns = best.get("isbn") or []
    if isbns:
        return f"https://covers.openlibrary.org/b/isbn/{isbns[0]}-L.jpg"

    return None

def _ratio(a: str, b: str) -> float:
    # Lightweight similarity (no external deps)
    if not a or not b:
        return 0.0
    # token overlap proxy
    sa, sb = set(a.split()), set(b.split())
    if not sa or not sb:
        return 0.0
    return (2.0 * len(sa & sb)) / (len(sa) + len(sb))


def download_and_normalize_image(url: str, out_path: str, max_px: int = 900) -> bool:
    try:
        r = http_get(url, timeout=30, headers={"Accept": "image/*"})
        img = PILImage.open(io.BytesIO(r.content)).convert("RGB")
        # Resize to max dimension to control PDF size
        w, h = img.size
        scale = min(1.0, float(max_px) / float(max(w, h)))
        if scale < 1.0:
            img = img.resize((int(w * scale), int(h * scale)))
        ensure_dir(os.path.dirname(out_path))
        img.save(out_path, format="JPEG", quality=85, optimize=True)
        return True
    except Exception:
        return False


# ---------- PDF rendering ----------

def fit_image(path: str, max_w: float, max_h: float) -> Image:
    img = utils.ImageReader(path)
    iw, ih = img.getSize()
    scale = min(max_w / iw, max_h / ih)
    return Image(path, width=iw * scale, height=ih * scale)

def build_pdf(
    records: List[YearRecord],
    cover_paths: Dict[Tuple[int, str, str], Optional[str]],
    out_pdf: str,
) -> None:
    styles = getSampleStyleSheet()
    styles["Title"].alignment = TA_CENTER

    doc = SimpleDocTemplate(
        out_pdf,
        pagesize=letter,
        leftMargin=0.6 * inch,
        rightMargin=0.6 * inch,
        topMargin=0.6 * inch,
        bottomMargin=0.6 * inch,
        title="Caldecott Medal & Honor Books (with jackets)",
    )

    story = []
    story.append(Paragraph("Caldecott Medal & Honor Books (with book jackets)", styles["Title"]))
    story.append(Spacer(1, 0.15 * inch))
    story.append(Paragraph(
        "Compiled from ALSC/ALA lists; covers sourced primarily via Open Library. "
        "For personal/reference use.",
        styles["BodyText"]
    ))
    story.append(PageBreak())

    for yr in records:
        story.append(Paragraph(f"{yr.year}", styles["Heading1"]))
        story.append(Spacer(1, 0.12 * inch))

        # Medal section
        if yr.medal:
            medal_key = (yr.year, "Medal", yr.medal.title)
            cover = cover_paths.get(medal_key)
            left = None
            if cover and os.path.exists(cover):
                left = fit_image(cover, max_w=2.1 * inch, max_h=2.6 * inch)
            else:
                left = Paragraph("(cover not found)", styles["BodyText"])

            meta = yr.medal.raw.strip()
            if meta and meta != "(ALA news release)":
                meta_text = f"<br/><font size=8 color='#444444'>{_escape(meta)}</font>"
            else:
                meta_text = ""

            right = Paragraph(
                f"<b>Medal</b><br/>{_escape(yr.medal.title)}{meta_text}",
                styles["BodyText"]
            )

            t = Table([[left, right]], colWidths=[2.2 * inch, 4.8 * inch])
            t.setStyle(TableStyle([
                ("VALIGN", (0, 0), (-1, -1), "TOP"),
                ("BOX", (0, 0), (-1, -1), 0.6, black),
                ("INNERPADDING", (0, 0), (-1, -1), 6),
            ]))
            story.append(t)
            story.append(Spacer(1, 0.18 * inch))

        # Honors section (grid)
        if yr.honors:
            story.append(Paragraph("<b>Honor Books</b>", styles["Heading3"]))
            story.append(Spacer(1, 0.08 * inch))

            row = []
            rows = []
            for h in yr.honors:
                honor_key = (yr.year, "Honor", h.title)
                cover = cover_paths.get(honor_key)
                if cover and os.path.exists(cover):
                    img = fit_image(cover, max_w=1.45 * inch, max_h=1.85 * inch)
                else:
                    img = Paragraph("(cover not found)", styles["BodyText"])

                cell = Table(
                    [[img],
                     [Paragraph(f"{_escape(h.title)}", styles["BodyText"])]],
                    colWidths=[1.6 * inch],
                )
                cell.setStyle(TableStyle([
                    ("VALIGN", (0, 0), (-1, -1), "TOP"),
                    ("BOX", (0, 0), (-1, -1), 0.25, lightgrey),
                    ("INNERPADDING", (0, 0), (-1, -1), 4),
                ]))

                row.append(cell)
                if len(row) == 4:
                    rows.append(row)
                    row = []
            if row:
                # pad last row
                while len(row) < 4:
                    row.append(Paragraph("", styles["BodyText"]))
                rows.append(row)

            grid = Table(rows, colWidths=[1.7 * inch] * 4)
            grid.setStyle(TableStyle([("VALIGN", (0, 0), (-1, -1), "TOP")]))
            story.append(grid)

        story.append(PageBreak())

    doc.build(story)

def _escape(s: str) -> str:
    # Minimal HTML escaping for reportlab Paragraph
    return (s.replace("&", "&amp;")
             .replace("<", "&lt;")
             .replace(">", "&gt;"))


# ---------- Main ----------

def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("--out", default="caldecott_with_jackets.pdf", help="Output PDF path")
    ap.add_argument("--start-year", type=int, default=1938)
    ap.add_argument("--end-year", type=int, default=2025)
    ap.add_argument("--max-workers", type=int, default=12)
    ap.add_argument("--cache-dir", default=".cache/caldecott")
    ap.add_argument("--max-image-px", type=int, default=900, help="Max image dimension to keep PDF size reasonable")
    ap.add_argument("--sleep", type=float, default=0.0, help="Optional delay per cover request (seconds)")
    args = ap.parse_args()

    ensure_dir(args.cache_dir)
    pdf_cache = os.path.join(args.cache_dir, "alsc_caldecott_list.pdf")
    pdf_path = download_cached(ALSC_PDF_URL, pdf_cache)

    lines = extract_pdf_text_lines(pdf_path)
    base_records = parse_alsc_pdf_records(lines)

    # Supplement with 2024/2025 from ALA news releases (if within range)
    if args.end_year >= 2024:
        try:
            y2024 = parse_ala_2024()
            if y2024.medal and y2024.year not in base_records:
                base_records[y2024.year] = y2024
        except Exception as e:
            print(f"[warn] failed to parse ALA 2024: {e}", file=sys.stderr)

    if args.end_year >= 2025:
        try:
            y2025 = parse_ala_2025()
            if y2025.medal and y2025.year not in base_records:
                base_records[y2025.year] = y2025
        except Exception as e:
            print(f"[warn] failed to parse ALA 2025: {e}", file=sys.stderr)

    # Filter + sort
    years = sorted(y for y in base_records.keys() if args.start_year <= y <= args.end_year)
    records = [base_records[y] for y in years]

    # Collect all book entries for cover fetching
    fetch_jobs: List[Tuple[int, str, BookEntry]] = []
    for yr in records:
        if yr.medal:
            fetch_jobs.append((yr.year, "Medal", yr.medal))
        for h in yr.honors:
            fetch_jobs.append((yr.year, "Honor", h))

    cover_dir = os.path.join(args.cache_dir, "covers")
    ensure_dir(cover_dir)

    cover_paths: Dict[Tuple[int, str, str], Optional[str]] = {}
    missing_rows: List[Tuple[int, str, str]] = []

    def fetch_one(job: Tuple[int, str, BookEntry]) -> Tuple[Tuple[int, str, str], Optional[str]]:
        year, award, entry = job
        key = (year, award, entry.title)

        # cache key
        fname = f"{year}_{award.lower()}_{slug_key(entry.title)}.jpg"
        out_path = os.path.join(cover_dir, fname)
        if os.path.exists(out_path) and os.path.getsize(out_path) > 0:
            return key, out_path

        # best-effort author hint
        ah = entry.author or entry.illustrator
        url = openlibrary_find_cover_url(entry.title, author_hint=ah)
        if url and download_and_normalize_image(url, out_path, max_px=args.max_image_px):
            if args.sleep > 0:
                time.sleep(args.sleep)
            return key, out_path

        # Fail
        return key, None

    print(f"[info] years: {years[0]}–{years[-1]} ({len(years)} years)")
    print(f"[info] fetching covers for {len(fetch_jobs)} entries... (cached in {cover_dir})")

    with ThreadPoolExecutor(max_workers=args.max_workers) as ex:
        futures = [ex.submit(fetch_one, job) for job in fetch_jobs]
        for fut in as_completed(futures):
            key, path = fut.result()
            cover_paths[key] = path
            if path is None:
                missing_rows.append((key[0], key[1], key[2]))

    # Write missing report
    missing_csv = os.path.join(args.cache_dir, "missing_covers.csv")
    with open(missing_csv, "w", newline="", encoding="utf-8") as f:
        w = csv.writer(f)
        w.writerow(["year", "award", "title"])
        for row in sorted(missing_rows):
            w.writerow(list(row))

    print(f"[info] missing covers: {len(missing_rows)} (see {missing_csv})")

    print(f"[info] building PDF: {args.out}")
    build_pdf(records, cover_paths, args.out)
    print("[done]")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
