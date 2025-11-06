
import os

# Get the path to build_number.txt relative to this script
script_dir = os.path.dirname(os.path.abspath(__file__))
build_number_path = os.path.join(script_dir, '..', 'build_number.txt')

try:
    # Read current number
    with open(build_number_path, 'r') as f:
        count = int(f.read().strip())
except (FileNotFoundError, ValueError):
    # If file doesn't exist or is invalid, start at 0
    print("No build_number.txt was found, create one")
    count = 0

# Increment count
count += 1

# Write back
with open(build_number_path, 'w') as f:
    f.write(str(count))

print(f"Build number increased to: {count}")