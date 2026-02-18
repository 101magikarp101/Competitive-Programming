from xml.etree.ElementTree import Element, SubElement, tostring
from xml.dom import minidom
import re

def dms_to_decimal_magnitude(s):
    """
    Parse "32°45′06″" / "32°45'06\"" / "32 45 06" / decimal / packed DDMMSS.
    Returns positive float magnitude in decimal degrees.
    """
    s = str(s).strip()
    if re.fullmatch(r"[+-]?\d+(\.\d+)?", s):
        return abs(float(s))

    nums = re.findall(r"-?\d+", s)
    if not nums:
        raise ValueError(f"Could not parse token: {s!r}")

    # packed forms: 5-7 digits -> DMMSS / DDMMSS / DDDMMSS
    if len(nums) == 1 and re.fullmatch(r"-?\d{5,7}", nums[0]):
        t = nums[0]
        sign = -1 if t.startswith("-") else 1
        t = t.lstrip("-")
        if len(t) == 5:
            deg, minute, sec = int(t[0]), int(t[1:3]), int(t[3:5])
        elif len(t) == 6:
            deg, minute, sec = int(t[0:2]), int(t[2:4]), int(t[4:6])
        else:
            deg, minute, sec = int(t[0:3]), int(t[3:5]), int(t[5:7])
        return abs(sign * (deg + minute/60 + sec/3600))

    # standard D M S
    if len(nums) >= 3:
        deg, minute, sec = map(int, nums[:3])
        return abs(deg + minute/60 + sec/3600)

    raise ValueError(f"Could not parse token: {s!r}")

def prettify_xml(elem):
    rough = tostring(elem, "utf-8")
    return minidom.parseString(rough).toprettyxml(indent="  ")

def parse_two_column_text(text):
    lat_tokens = []
    for line in text.splitlines():
        line = line.strip()
        if not line:
            continue
        parts = re.split(r"\s+", line)
        if len(parts) < 2:
            raise ValueError(f"Expected two columns: {line!r}")
        lat_tokens.append(parts[0])
    return lat_tokens

def make_latitude_lines_kml(
    lat_tokens,
    out_path="latitude_lines.kml",
    include_southern=True,
    step_deg=1.0
):
    # unique magnitudes (stable dedupe by rounding)
    mags = sorted({round(dms_to_decimal_magnitude(t), 10) for t in lat_tokens})

    kml = Element("kml", xmlns="http://www.opengis.net/kml/2.2")
    doc = SubElement(kml, "Document")
    SubElement(doc, "name").text = "Latitude lines (parallels)"

    def add_folder(name):
        folder = SubElement(doc, "Folder")
        SubElement(folder, "name").text = name
        return folder

    folder_n = add_folder("North (+lat)")
    folder_s = add_folder("South (-lat)") if include_southern else None

    def add_parallel(folder, lat, label):
        pm = SubElement(folder, "Placemark")
        SubElement(pm, "name").text = label
        ls = SubElement(pm, "LineString")
        SubElement(ls, "tessellate").text = "1"

        coords = []
        lon = -180.0
        # include endpoint +180
        while lon <= 180.0 + 1e-9:
            coords.append(f"{lon:.6f},{lat:.6f},0")
            lon += step_deg

        SubElement(ls, "coordinates").text = " ".join(coords)

    for m in mags:
        add_parallel(folder_n, +m, f"lat = +{m:.6f}")
        if include_southern:
            add_parallel(folder_s, -m, f"lat = -{m:.6f}")

    with open(out_path, "w", encoding="utf-8") as f:
        f.write(prettify_xml(kml))

    total = len(mags) * (2 if include_southern else 1)
    print(f"Wrote {out_path} with {total} latitude lines.")

# --- Your exact two-column block ---
COORD_TEXT = """
32°45′06″    3°03′19″
30°02′40″    36°49′27″
40°27′00″    60°35′28″
42°52′29″    100°29′16″
31°14′09″    140°42′54″
27°26′54″    155°23′32″
36°52′01″    169°51′01″
21°14′08″    134°59′04″
32°30′47″    113°29′38″
0°42′29″     79°53′01″
14°39′57″    56°10′07″
3°50′56″     25°37′56″
""".strip()

if __name__ == "__main__":
    lat_tokens = parse_two_column_text(COORD_TEXT)
    make_latitude_lines_kml(
        lat_tokens,
        out_path="latitude_lines.kml",
        include_southern=True,  # set False if you only want +lat
        step_deg=1.0            # smaller = smoother, bigger = lighter
    )
