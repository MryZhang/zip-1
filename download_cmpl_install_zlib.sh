#!/usr/bin/bash
`finclude $0 zsl.sh`
SetTEST $@
zlib=("zlib" "$HOME/git/zlib" "git://github.com/madler/zlib.git")
GitCheck ${zlib[@]}
if [ $? -ne 0 ];then exit 1; fi
exit 0

devlibs=/usr/devlibs
$TEST sudo mkdir -p $devlibs

$TEST cd ${zlib[1]}
$TEST ./configure --prefix="$devlibs"
$TEST make
$TEST sudo make install



