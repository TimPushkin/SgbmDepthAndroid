package me.timpushkin.sgbmandroid;

import org.bytedeco.javacpp.annotation.Platform;
import org.bytedeco.javacpp.annotation.Properties;
import org.bytedeco.javacpp.tools.InfoMap;
import org.bytedeco.javacpp.tools.InfoMapper;

@Properties(
        value = @Platform(include = "DepthEstimator.h"),
        target = "me.timpushkin.sgbmandroid.SgbmAndroid"
)
public class SgbmAndroidConfig implements InfoMapper {
    static {
        System.setProperty("org.bytedeco.javacpp.loadLibraries", "false");
    }

    public void map(InfoMap infoMap) {
    }
}
