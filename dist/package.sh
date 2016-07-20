#!/usr/bin/env bash

version=$(cmake -H. -Bbuild/production -DCMAKE_BUILD_TYPE=Release | grep "\-\- inform version:")
version=${version#"-- inform version: "}

target=inform-$version
prefix=dist/$target
tarball=$target"_linux-x86_64.tar.gz"

cmake -H. -Bbuild/production -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=$prefix > /dev/null
make -C build/production all test
make -C build/production install > /dev/null
cp dist/install.sh $prefix > /dev/null
cp LICENSE $prefix > /dev/null
cp README.md $prefix > /dev/null

cd dist
echo "Creating tarball: dist/$tarball"
tar czf $tarball $target
