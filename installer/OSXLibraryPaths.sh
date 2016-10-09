#!/bin/bash
install_name_tool -change /usr/local/lib/libportaudio.2.dylib @executable_path/../Frameworks/libportaudio.2.dylib Sigmatizm.app/Contents/MacOS/Sigmatizm
otool -L Sigmatizm.app/Contents/MacOS/Sigmatizm
