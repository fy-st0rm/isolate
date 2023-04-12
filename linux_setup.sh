#!/bin/bash

echo "Installing pyinstaller..."
pip install pyinstaller

echo "Building ipm..."
pyinstaller --onefile ipm/ipm.py

echo "Installing ipm..."
rm -r build/
rm ipm.spec
sudo mv dist/ipm /usr/bin/
rmdir dist

echo "Sucessfully installed ipm at /usr/bin"
