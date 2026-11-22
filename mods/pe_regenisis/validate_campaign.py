import json
from pathlib import Path

path = Path(__file__).with_name("campaign.json")
campaign = json.loads(path.read_text(encoding="utf-8"))
assert campaign["version"] == 1
assert campaign["entry"] == "m0182i"
rooms = campaign["rooms"]
assert len(rooms) == len(set(rooms))
assert len(rooms) == 9
events = campaign["events"]
ids = {e["id"] for e in events}
assert len(ids) == len(events)
for event in events:
    assert event["when"] in {"enter", "explore", "victory"}
    assert event["pages"] and all(isinstance(p, str) and p for p in event["pages"])
    if "room" in event:
        assert event["room"] in rooms
    if "transfer" in event:
        assert event["transfer"] in rooms
    if "requires" in event:
        assert event["requires"] in ids and event["requires"] < event["id"]
assert any(e.get("finish") for e in events)
print("REGENISIS campaign manifest: validated", len(rooms), "rooms,", len(events), "events")
