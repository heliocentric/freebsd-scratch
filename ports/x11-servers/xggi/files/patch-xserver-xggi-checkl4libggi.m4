diff -ruN xserver/xggi/check4libggi.m4 xserver/xggi/check4libggi.m4
--- xserver/xggi/check4libggi.m4	2005-12-20 22:29:17.000000000 +0000
+++ xserver/xggi/check4libggi.m4	2009-12-03 07:27:20.000000000 +0000
@@ -44,7 +44,6 @@
 dnl This isn't a problem during the build because libtool is always used.
 dnl Wish I could use $LIBTOOL, but it is written for Makefile use.
 save_CC="$CC"
-CC="$SHELL ./libtool --mode=link $CC"
 AC_CHECK_LIB(ggi,ggiInit,foo=bar,ggi_missing_part=yes)
 CC="$save_CC"
