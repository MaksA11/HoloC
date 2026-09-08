from PIL import Image

image = Image.open("image.png").convert("RGB")
pixels = image.load()
width, height = image.size

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