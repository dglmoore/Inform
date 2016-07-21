$version = (cmake . -Bbuild/production -DCMAKE_BUILD_TYPE=Release -G "NMake Makefiles" | Select-String '-- inform version: ')
$version = $version.ToString().Split(' ')[3]

$prefix = "inform-$version"
$target = "dist/$prefix"
$archive = "$($target)_win-amd64.zip"

cmake . -Bbuild/production -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX="$target" | Out-Null
cd build/production
nmake
test/inform_unittest.exe
nmake install
cd ../..
Copy-Item LICENSE $target
Copy-Item README.md $target

Remove-Item -force $archive
Add-Type -Assembly System.IO.Compression.FileSystem
$compressionLevel = [System.IO.Compression.CompressionLevel]::Optimal
[System.IO.Compression.ZipFile]::CreateFromDirectory($target, $archive, $compressionLevel, $true)
