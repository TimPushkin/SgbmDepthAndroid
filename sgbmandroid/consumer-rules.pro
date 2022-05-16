# Uncomment this to preserve the line number information for
# debugging stack traces.
-keepattributes SourceFile,LineNumberTable

# If you keep the line number information, uncomment this to
# hide the original source file name.
-renamesourcefileattribute SourceFile

# Remove logging (https://www.guardsquare.com/manual/configuration/examples#logging)
-assumenosideeffects class android.util.Log {
    public static int v(...);
    public static int d(...);
    public static int i(...);
    public static int w(...);
    public static int e(...);
}

# ProGuard might remove DepthEstimator otherwise
-keep class me.timpushkin.sgbmandroid.DepthEstimator { *; }