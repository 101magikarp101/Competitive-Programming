from xml.etree.ElementTree import Element, SubElement, tostring
from xml.dom import minidom
import re

def dms_to_decimal_magnitude(s):
    """
    Parse a coordinate magnitude in any of these forms:
      - "32°45′06″"  (unicode prime symbols)
      - "32°45'06\"" (ascii)
      - "32 45 06"
      - "324506" (DDMMSS), "1695101" (DDDMMSS), "35056" (DMMSS)
      - "44.98194" (decimal)
    Returns: positive float magnitude in decimal degrees.
    """
    if isinstance(s, (int, float)):
        return abs(float(s))

    s = str(s).strip()
    if not s:
        raise ValueError("Empty coordinate token")

    # Decimal degrees
    if re.fullmatch(r"[+-]?\d+(\.\d+)?", s):
        return abs(float(s))

    # Extract signed/unsigned integers from DMS-ish formats
    nums = re.findall(r"-?\d+", s)
    if not nums:
        raise ValueError(f"Could not parse coordinate token: {s!r}")

    # Compact packed forms: DDMMSS / DDDMMSS / DMMSS
    if len(nums) == 1 and re.fullmatch(r"-?\d{5,7}", nums[0]):
        t = nums[0]
        sign = -1 if t.startswith("-") else 1
        t = t.lstrip("-")

        if len(t) == 5:  # DMMSS
            deg = int(t[0])
            minute = int(t[1:3])
            sec = int(t[3:5])
        elif len(t) == 6:  # DDMMSS
            deg = int(t[0:2])
            minute = int(t[2:4])
            sec = int(t[4:6])
        else:  # 7 => DDDMMSS
            deg = int(t[0:3])
            minute = int(t[3:5])
            sec = int(t[5:7])

        return abs(sign * (deg + minute / 60 + sec / 3600))

    # Standard D M S (first three numbers)
    if len(nums) >= 3:
        deg, minute, sec = map(int, nums[:3])
        return abs(deg + minute / 60 + sec / 3600)

    raise ValueError(f"Could not parse coordinate token: {s!r}")

def prettify_xml(elem):
    rough = tostring(elem, "utf-8")
    reparsed = minidom.parseString(rough)
    return reparsed.toprettyxml(indent="  ")

def parse_two_column_text(text):
    """
    Parses text where each non-empty line contains:
      <lat_token><whitespace><lon_token>
    Returns (lat_magnitudes, lon_magnitudes) as lists of strings.
    """
    lat_tokens = []
    lon_tokens = []
    for line in text.splitlines():
        line = line.strip()
        if not line:
            continue
        parts = re.split(r"\s+", line)
        if len(parts) < 2:
            raise ValueError(f"Expected two columns on line: {line!r}")
        lat_tokens.append(parts[0])
        lon_tokens.append(parts[1])
    return lat_tokens, lon_tokens

def make_kml(lat_tokens, lon_tokens, out_path="candidates.kml"):
    lats = [dms_to_decimal_magnitude(v) for v in lat_tokens]
    lons = [dms_to_decimal_magnitude(v) for v in lon_tokens]

    kml = Element("kml", xmlns="http://www.opengis.net/kml/2.2")
    doc = SubElement(kml, "Document")
    SubElement(doc, "name").text = "Coordinate Candidates (144×4)"

    quadrants = [
        ("NE (+lat,+lon)", +1, +1),
        ("NW (+lat,-lon)", +1, -1),
        ("SE (-lat,+lon)", -1, +1),
        ("SW (-lat,-lon)", -1, -1),
    ]

    for qname, s_lat, s_lon in quadrants:
        folder = SubElement(doc, "Folder")
        SubElement(folder, "name").text = qname

        for i, lat in enumerate(lats, start=1):
            for j, lon in enumerate(lons, start=1):
                plat = s_lat * lat
                plon = s_lon * lon

                pm = SubElement(folder, "Placemark")
                # Label includes indices so you can cross-reference later
                SubElement(pm, "name").text = f"{qname}  Lat#{i} Lon#{j}"
                SubElement(pm, "description").text = (
                    f"{qname}\n"
                    f"Lat token: {lat_tokens[i-1]}\nLon token: {lon_tokens[j-1]}\n"
                    f"lat={plat:.6f}, lon={plon:.6f}"
                )

                point = SubElement(pm, "Point")
                SubElement(point, "altitudeMode").text = "clampToGround"
                SubElement(point, "coordinates").text = f"{plon:.6f},{plat:.6f},0"

    with open(out_path, "w", encoding="utf-8") as f:
        f.write(prettify_xml(kml))

    print(f"Wrote {out_path} with {len(lats)*len(lons)*4} placemarks.")

# --- Your exact two-column block goes here ---
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
    lat_tokens, lon_tokens = parse_two_column_text(COORD_TEXT)
    make_kml(lat_tokens, lon_tokens, out_path="candidates.kml")
