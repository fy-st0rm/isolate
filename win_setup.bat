@echo "Installing pyinstaller..."
pip install pyinstaller

@echo "Building ipm..."
pyinstaller --onefile ipm/ipm.py

@echo "Installing ipm..."
@rmdir /s /q build
erase ipm.spec

@mkdir %AppData%\ipm

@move dist\ipm.exe %AppData%\ipm
@rmdir dist
@setx Path "%Path%;%AppData%\ipm"

@echo "Sucessfully installed ipm at %AppData%\ipm"