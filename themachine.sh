startdir="$(realpath "$(dirname "$0")")"

./configure CC=clang --prefix="$startdir"/bin \
	--disable-debug \
	--enable-generic

make clean
make install
