diff -ruN xserver/xggi/autogen.sh xserver/xggi/autogen.sh
--- xserver/xggi/autogen.sh	2005-12-20 22:29:17.000000000 +0000
+++ xserver/xggi/autogen.sh	2009-12-03 06:54:35.000000000 +0000
@@ -11,4 +11,4 @@
 autoreconf -v --install || exit 1
 cd $ORIGDIR || exit $?
 
-$srcdir/configure --enable-maintainer-mode "$@"
+$srcdir/configure "$@"
