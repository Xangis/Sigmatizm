#!/bin/bash
cp -R ../Release/Patches Sigmatizm.app/Contents/Resources/
codesign --deep -f -s "3rd Party Mac Developer Application: Jason Champion (G6PFF85X2R)" Sigmatizm.app --entitlements Sigmatizm.app/Contents/Resources/Entitlements.plist
productbuild --sign "3rd Party Mac Developer Installer: Jason Champion (G6PFF85X2R)" --component Sigmatizm.app /Applications Sigmatizm.pkg
