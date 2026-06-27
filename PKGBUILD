# Maintainer: cobra-r9 <cobra.rev.9@gmail.com>
pkgname=my_program
pkgver=1.0.0
pkgrel=1
pkgdesc="A C template project structure defined specifically for arch linux."
arch=('x86_64')
license=('MIT')
makedepends=('gcc' 'make')
source=()
sha256sums=()

build() {
    cd "$startdir"
    make NAME="$pkgname"
}

package() {
    cd "$startdir"
    install -Dm755 "build/bin/$pkgname" "$pkgdir/usr/bin/$pkgname"
    install -Dm644 LICENSE "$pkgdir/usr/share/licenses/$pkgname/LICENSE"
    install -Dm644 readme.md "$pkgdir/usr/share/docs/$pkgname/README.md"
    cp -r writeups "$pkgdir/usr/share/docs/$pkgname/writeups"
    echo "Run 'make purge' to clean makepkg residues..."
}
