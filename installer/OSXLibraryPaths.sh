#!/bin/bash
install_name_tool -change /usr/local/lib/libportaudio.2.dylib @executable_path/../Frameworks/libportaudio.2.dylib Sigmatizm.app/Contents/MacOS/Sigmatizm
install_name_tool -change /usr/local/lib/libwx_osx_cocoau_xrc-3.1.dylib @executable_path/../Frameworks/libwx_osx_cocoau_xrc-3.1.dylib Sigmatizm.app/Contents/MacOS/Sigmatizm
install_name_tool -change /usr/local/lib/libwx_osx_cocoau_html-3.1.dylib @executable_path/../Frameworks/libwx_osx_cocoau_html-3.1.dylib Sigmatizm.app/Contents/MacOS/Sigmatizm
install_name_tool -change /usr/local/lib/libwx_osx_cocoau_qa-3.1.dylib @executable_path/../Frameworks/libwx_osx_cocoau_qa-3.1.dylib Sigmatizm.app/Contents/MacOS/Sigmatizm
install_name_tool -change /usr/local/lib/libwx_osx_cocoau_adv-3.1.dylib @executable_path/../Frameworks/libwx_osx_cocoau_adv-3.1.dylib Sigmatizm.app/Contents/MacOS/Sigmatizm
install_name_tool -change /usr/local/lib/libwx_osx_cocoau_core-3.1.dylib @executable_path/../Frameworks/libwx_osx_cocoau_core-3.1.dylib Sigmatizm.app/Contents/MacOS/Sigmatizm
install_name_tool -change /usr/local/lib/libwx_baseu_xml-3.1.dylib @executable_path/../Frameworks/libwx_baseu_xml-3.1.dylib Sigmatizm.app/Contents/MacOS/Sigmatizm
install_name_tool -change /usr/local/lib/libwx_baseu_net-3.1.dylib @executable_path/../Frameworks/libwx_baseu_net-3.1.dylib Sigmatizm.app/Contents/MacOS/Sigmatizm
install_name_tool -change /usr/local/lib/libwx_baseu-3.1.dylib @executable_path/../Frameworks/libwx_baseu-3.1.dylib Sigmatizm.app/Contents/MacOS/Sigmatizm
otool -L Sigmatizm.app/Contents/MacOS/Sigmatizm
