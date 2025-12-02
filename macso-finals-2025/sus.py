import requests
import csv
from datetime import datetime

# Replace with your PokéTCG API key (https://pokemontcg.io/)
API_KEY = "YOUR_API_KEY"

HEADERS = {"X-Api-Key": API_KEY}


def fetch_sets():
    """Fetch all sets from the PokéTCG API."""
    url = "https://api.pokemontcg.io/v2/sets"
    resp = requests.get(url, headers=HEADERS)
    resp.raise_for_status()
    return resp.json()["data"]


def fetch_cards_for_set(set_id):
    """Fetch all cards for a given set (handles pagination)."""
    url = "https://api.pokemontcg.io/v2/cards"
    page = 1
    page_size = 250
    cards = []
    while True:
        params = {"set.id": set_id, "page": page, "pageSize": page_size}
        resp = requests.get(url, headers=HEADERS, params=params)
        resp.raise_for_status()
        data = resp.json().get("data", [])
        if not data:
            break
        cards.extend(data)
        page += 1
    return cards


def main():
    # 1. Get all sets and parse their release dates
    sets = fetch_sets()
    for s in sets:
        try:
            s['releaseDate'] = datetime.strptime(s.get('releaseDate', ''), "%Y/%m/%d")
        except ValueError:
            s['releaseDate'] = None

    # 2. Identify the SHF (Shining Fates) and SVI (SV1) release dates
    start = next(s for s in sets if s['id'].lower() == 'shiningfates')
    end = next(s for s in sets if s['id'].lower() == 'sv1')
    start_date = start['releaseDate']
    end_date = end['releaseDate']

    # 3. Filter sets in the SHF→SVI range
    selected_sets = [s for s in sets if s['releaseDate'] and start_date <= s['releaseDate'] <= end_date]
    selected_sets.sort(key=lambda x: x['releaseDate'])

    # 4. Write out CSV
    with open('cards_shf_svi.csv', 'w', newline='', encoding='utf-8') as csvfile:
        writer = csv.writer(csvfile)
        writer.writerow(['set_code', 'set_name', 'card_number', 'card_name'])
        for s in selected_sets:
            print(f"Fetching cards for set: {s['id']} ({s['name']})...")
            cards = fetch_cards_for_set(s['id'])
            for c in cards:
                writer.writerow([s['id'], s['name'], c['number'], c['name']])

    print("Done! Saved to cards_shf_svi.csv")


if __name__ == '__main__':
    main()
