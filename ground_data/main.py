import re
import pandas as pd
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import matplotlib.ticker as mticker


# Function to parse a line into a dictionary of key-value pairs
def parse_line(line):
    data = {}
    # Extract all key-value pairs in the format key:value
    matches = re.findall(r"(\w+):(-?[\d.]+)", line)
    for key, value in matches:
        # Convert numeric fields to float; otherwise, keep as string
        data[key] = float(value) if re.match(r"-?[\d.]+", value) else value
    return data

# Read and parse data from "data.txt"
data = []
with open("data.txt", "r") as file:
    for line in file:
        line = line.strip()
        parsed = parse_line(line)
        if parsed:  # Only append if line contains valid data
            data.append(parsed)

# Convert parsed data to a DataFrame
df = pd.DataFrame(data)

# Ensure lat, long, alt, and temp are present
required_columns = {"lat", "long", "alt", "temp"}
if not required_columns.issubset(df.columns):
    raise ValueError(f"Missing required columns for plotting: {required_columns - set(df.columns)}")

# Create a 3D scatter plot
fig = plt.figure(figsize=(10, 7))
ax = fig.add_subplot(111, projection='3d')

# Scatterplot with color representing temperature
sc = ax.scatter(
    df['long'],
    df['lat'],
    df['alt'],
    c=df['temp'],
    cmap='coolwarm',
    s=50,
    label='Data Points',
)

ax.xaxis.set_major_formatter(mticker.FormatStrFormatter("%.2f"))  # Adjust as needed for lat/long precision
ax.yaxis.set_major_formatter(mticker.FormatStrFormatter("%.2f"))
ax.zaxis.set_major_formatter(mticker.FormatStrFormatter("%.1f"))  # For altitude (m)

# Draw connecting line
ax.plot(
    df['long'],
    df['lat'],
    df['alt'],
    color='gray',
    linewidth=1,
    label='Path',
)

# Add color bar for temperature
cb = plt.colorbar(sc, ax=ax, pad=0.1)
cb.set_label("Temperature (°C)")

# Reduce margins
plt.tight_layout()

# Set axis labels
ax.set_xlabel("Longitude")
ax.set_ylabel("Latitude")
ax.set_zlabel("Altitude (m)")

# Add legend and title
ax.legend()
fig.suptitle('HIVE Outdoor Test', x=0.1, y=0.95, ha='left')
fig.savefig('image.png', dpi=300)
plt.show()
