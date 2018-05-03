#!/usr/bin/env bash
cd ../

SCRIPTPATH="$( cd "$(dirname "$0")" ; pwd -P )"
APP=TestDeferred
BINPATH=${SCRIPTPATH}/Build/Debug/bin
BUNDLE=${BINPATH}/Microverse.app

rm -rf ${BUNDLE}
mkdir ${BUNDLE}
mkdir ${BUNDLE}/Contents
mkdir ${BUNDLE}/Contents/MacOS
mkdir ${BUNDLE}/Contents/Resources
mkdir ${BUNDLE}/Contents/Frameworks

cp -r ${SCRIPTPATH}/Resources ${BUNDLE}/Contents/Resources
cp ${BINPATH}/Microverse ${BUNDLE}/Contents/MacOS/Microverse
cp ${SCRIPTPATH}/Scripts/Info.plist ${BUNDLE}/Contents/Info.plist