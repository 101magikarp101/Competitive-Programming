
es = []

for _ in range(100):
    line = input().strip()
    toks = line.split(",")
    es.append("\"" + toks[1] + "\"")

print("{" + ",".join(es) + "}")
