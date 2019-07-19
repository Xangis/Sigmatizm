#!/bin/bash
# This should only need to be run once to get the wxWidgets libraries into the app directory.
mkdir -p Sigmatizm.app/Contents/Frameworks
cp /usr/local/lib/libwx_osx_cocoau_adv-3.1.dylib Sigmatizm.app/Contents/Frameworks/
cp /usr/local/lib/libwx_osx_cocoau_core-3.1.dylib Sigmatizm.app/Contents/Frameworks/
cp /usr/local/lib/libwx_osx_cocoau_html-3.1.dylib Sigmatizm.app/Contents/Frameworks/
cp /usr/local/lib/libwx_baseu_net-3.1.dylib Sigmatizm.app/Contents/Frameworks/
cp /usr/local/lib/libwx_baseu_net-3.1.dylib Sigmatizm.app/Contents/Frameworks/
cp /usr/local/lib/libwx_osx_cocoau_qa-3.1.dylib Sigmatizm.app/Contents/Frameworks/
cp /usr/local/lib/libwx_baseu_xml-3.1.dylib Sigmatizm.app/Contents/Frameworks/
cp /usr/local/lib/libwx_osx_cocoau_xrc-3.1.dylib Sigmatizm.app/Contents/Frameworks/
cp /usr/local/lib/libwx_baseu-3.1.dylib Sigmatizm.app/Contents/Frameworks/
cp ../../../lib/SDL2_ttf-2.0.15/.libs/libSDL2_ttf-2.0.0.dylib Sigmatizm.app/Contents/Frameworks/
cp ../../../lib/rtmidi-4.0.0/.libs/librtmidi.5.dylib Sigmatizm.app/Contents/Frameworks/
cp /usr/local/lib/librtaudio.6.dylib Sigmatizm.app/Contents/Frameworks/
# These aren't libs, but should be copied now.
cp -R ../Release/Patches Sigmatizm.app/Contents/Resources/
