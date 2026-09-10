#!/usr/bin/env bash

IMG="${1:-input_image.png}"

python3 image_converter.py "$IMG"
cp -f image.h ../firmware/image.h