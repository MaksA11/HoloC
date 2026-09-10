import sys
import os
from PIL import Image

if len(sys.argv) < 2:
    print("Missing image path")
    input("Press Enter to exit")
    sys.exit(1)

image_path = sys.argv[1]

if not os.path.isfile(image_path):
    print("File does not exist")
    input("Press Enter to exit")
    sys.exit(1)

image = Image.open(image_path).convert("RGB")
pixels = image.load()
width, height = image.size

if height != 16:
    print("Image height must be 16 pixels")
    input("Press Enter to exit")
    sys.exit(1)
elif width > 64:
    print("Image width must not exceed 64 pixels")
    input("Press Enter to exit")
    sys.exit(1)

pixelArray = [[pixels[x, y] for x in range(width)] for y in range(height)]

arrayString = f"#define COLUMN_COUNT {width}\n\n"
arrayString += f"Color pixels[{height}][{width}] = {{\n"

for i in range(height):
    arrayString += "\t{"

    for j in range(width):
        r, g, b = pixelArray[i][j]
        arrayString += f"{{{r}, {g}, {b}}}"
        if j < width - 1:
            arrayString += ", "

    arrayString += "}"
    if i < height - 1:
        arrayString += ","
    arrayString += "\n"

arrayString += "};"

with open("image.h", "w") as file:
    file.write(arrayString)