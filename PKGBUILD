# Maintainer: cobra-r9 <cobra.rev.9@gmail.com>
pkgname=pomoc
pkgver=1.0.1
pkgrel=1
pkgdesc="A Unix domain socket based pomodoro timer daemon and client"
arch=('x86_64')
license=('MIT')
makedepends=('gcc' 'make')
source=()
sha256sums=()

build() {
    cd "$startdir"
    make
}

package() {
    cd "$startdir"
    install -Dm755 "build/bin/pomod" "$pkgdir/usr/bin/pomod"
    install -Dm755 "build/bin/pomoc" "$pkgdir/usr/bin/pomoc"
    install -Dm644 LICENSE "$pkgdir/usr/share/licenses/$pkgname/LICENSE"
    install -Dm644 readme.md "$pkgdir/usr/share/docs/$pkgname/README.md"
    cp -r docs "$pkgdir/usr/share/docs/$pkgname/docs"
    echo "Run 'make purge' to clean makepkg residues..."
}
