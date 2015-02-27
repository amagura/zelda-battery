#if defined(__linux__) || defined(__gnu_linux__)
# define MAKE make
#elif defined(__FreeBSD__) || defined(__bsdi__)
# define MAKE gmake
#endif

./configure --disable-native --disable-generic

cd src
MAKE clean


for obj in power.o main-text.o main-color.o; do
    MAKE "$obj" CFLAGS=--static
done

for bin in zbatt zbatc; do
    MAKE "$bin" CFLAGS=--static CFLAGS+='obj/*.o'
done
