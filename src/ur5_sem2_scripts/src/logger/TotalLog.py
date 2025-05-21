import csv
import os
from glob import glob
from datetime import date

# Find nuværende mappe og gå 4 niveauer op
current_folder = os.getcwd()
output_folder = current_folder
for _ in range(4):
    output_folder = os.path.dirname(output_folder)

# Tilføj 'logLog' mappen
output_folder = os.path.join(output_folder, "logLog")
os.makedirs(output_folder, exist_ok=True)

# Lav filnavn med dags dato og tjek om den findes
base_name = date.today().isoformat()  # fx "2025-05-20"
output_file = os.path.join(output_folder, f"{base_name}.csv")
i = 1
while os.path.exists(output_file):
    output_file = os.path.join(output_folder, f"{base_name}_{i}.csv")
    i += 1

print("Output-fil gemmes her:", output_file)

# Find alle CSV-logfiler
log_folder = os.path.join(output_folder, "..", "Logs")
log_folder = os.path.abspath(log_folder)

log_files = glob(os.path.join(log_folder, "*.csv"))

all_rows = []
all_columns = []

# Få kolonner fra første fil
if log_files:
    with open(log_files[0], newline='') as f:
        reader = csv.reader(f)
        first_row = next(reader)
        seen = {}
        for col in first_row:
            if col in seen:
                seen[col] += 1
                col = f"{col}_{seen[col]}"
            else:
                seen[col] = 0
            all_columns.append(col)

# Saml data fra alle filer
for file in log_files:
    with open(file, newline='') as csvfile:
        reader = csv.DictReader(csvfile)
        for row in reader:
            clean_row = {col: row.get(col, "") for col in all_columns}
            all_rows.append(clean_row)

# Beregn summer for numeriske kolonner
sum_row = {}
first_col = all_columns[0]

for col in all_columns:
    if col == first_col:
        sum_row[col] = "SUM"
        continue

    total = 0.0
    for row in all_rows:
        val = row.get(col, "").strip()
        try:
            total += float(val)
        except ValueError:
            continue
    sum_row[col] = str(total) if total != 0 else ""


# Tilføj sum-række i bunden
all_rows.append(sum_row)


print("Søger i mappe:", log_folder)
print("Fandt log-filer:", log_files)


# Skriv samlet CSV-fil
with open(output_file, "w", newline='') as outfile:
    writer = csv.DictWriter(outfile, fieldnames=all_columns)
    writer.writeheader()
    writer.writerows(all_rows)

print(f"\n✅ Sammenfatning gemt som: {output_file}")
print(f"🔍 Antal logfiler: {len(log_files)}, antal rækker: {len(all_rows)}, antal kolonner: {len(all_columns)}")




