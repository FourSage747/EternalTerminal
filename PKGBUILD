pkgname=eternalterminal
pkgver=0.1.0
pkgrel=1
pkgdesc="A custom Qt6 terminal emulator"
arch=('x86_64')
url="https://github.com/FourSage747/EternalTerminal"
license=('GPL')

depends=(
    'qt6-base'
    'qt6-declarative'
)

build() {
    cmake -B build \
        -S "$startdir" \
        -DCMAKE_BUILD_TYPE=Release \
        -DCMAKE_INSTALL_PREFIX=/usr

    cmake --build build
}

package() {
    cmake --install build --prefix "$pkgdir/usr"
}
