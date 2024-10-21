#!/usr/bin/env bash

TMP_DIR="mnt/nonvol/"
if [[ -d mnt/ ]]; then
	rm -r mnt/
fi

# gather the files we need
mkdir -p $TMP_DIR
cp ../sample_defs/arm-rtems6-atsamv_cfe_es_startup.scr $TMP_DIR/cfe_es_startup.scr
cp ../build/tables/staging/arm-rtems6-atsamv/nonvol/*.tbl $TMP_DIR

# roll up an (uncompressed!) tarball
tar cf tarfile $TMP_DIR

# convert it to a binary object file for linking !
arm-rtems6-ld -r --noinhibit-exec -o tarfile.o -b binary tarfile
rm ../sample_defs/tarfile.o
mv tarfile.o ../sample_defs/

# clean up after yourself, you slob
rm tarfile
rm -r mnt/
