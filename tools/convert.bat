@echo off
set "IMG=%~1"
if "%IMG%"=="" set "IMG=input_image.png"

python image_converter.py "%IMG%"
copy /y "image.h" "..\firmware\image.h"