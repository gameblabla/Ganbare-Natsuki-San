cp COPYRIGHT opk/COPYRIGHT
cp gnp opk/gnp
mksquashfs ./opk gnp.opk -all-root -noappend -no-exports -no-xattrs
rm opk/COPYRIGHT
rm opk/gnp
