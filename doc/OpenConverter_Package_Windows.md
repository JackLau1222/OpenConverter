# Packaging the OC Project

1. **Run in Release Mode**
    Click the computer icon on the left side of Qt, select "Release," and then click "Run" to build the project.
2. **Locate the Release Folder**
    After the build completes, navigate to the "build" folder and find the "Release" folder containing the packaged files.
3. **Open Qt MinGW Terminal**
    Go to the Start menu, search for "min," and select "Qt MinGW" to open the terminal.
4. **Deploy Dependencies with windeployqt**
    Copy the path to the folder containing the newly generated .exe file. In the terminal, type windeployqt, paste the path (Ctrl+V), and append a backslash (\) followed by the .exe file name. Press Enter to execute.
5. **Download Enigma Protector**
    Use a new software tool for advanced packaging, available at: https://enigmaprotector.com/en/downloads.html.
6. **Package the .exe with Enigma Protector**
    Use the "Release" folder from the previous step as the base. Load the generated .exe file into the software, which will automatically create a new file named originalname_boxed.exe.
7. **Configure and Compress**
    In the bottom left corner, click "Add" and select "Select Folder (Recursive)." Then, under "File Options," check the "Compress Files" box and proceed with the packaging process.
8. **Create Final Archive**
    Combine the packaged .exe file with the DLL files from FFmpeg’s "bin" directory into a single compressed archive (e.g., ZIP file).