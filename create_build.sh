#!/usr/bin/env bash
BUILDDIR="coinage_linux64"
mkdir $BUILDDIR
echo "Compiling ..."
make -B
echo "Copying docs into" $BUILDDIR
cp LICENSE $BUILDDIR
cp README.md $BUILDDIR
echo "Copying data assets" $BUILDDIR
cp -r data $BUILDDIR
echo "Copying executables into" $BUILDDIR
cp coinage $BUILDDIR
echo "Compressiong" $BUILDDIR
tar -cvf coinage-linux64.tar $BUILDDIR
gzip coinage-linux64.tar
echo "Cleaning up"
rm -Rf $BUILDDIR
