//-----------------------------------------------------------------------------
// ScreenRecorder.java
// Authors: big
// CreateDate: 2022.2.22
//-----------------------------------------------------------------------------

package com.tatfook.paracraft.screenrecorder;

import android.annotation.TargetApi;
import android.app.AlertDialog;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.pm.PackageManager;
import android.graphics.Point;
import android.hardware.display.DisplayManager;
import android.hardware.display.VirtualDisplay;
import android.media.MediaCodecInfo;
import android.media.projection.MediaProjection;
import android.media.projection.MediaProjectionManager;
import android.net.Uri;
import android.os.Environment;
import android.os.Looper;
import android.util.Log;
import android.widget.Toast;

import com.tatfook.paracraft.BuildConfig;
import com.tatfook.paracraft.ParaEngineActivity;

import java.io.File;
import java.text.SimpleDateFormat;
import java.util.Arrays;
import java.util.Date;
import java.util.Locale;
import java.util.Stack;

import static android.Manifest.permission.RECORD_AUDIO;
import static android.Manifest.permission.WRITE_EXTERNAL_STORAGE;
import static android.content.Context.MEDIA_PROJECTION_SERVICE;
import static android.os.Build.VERSION_CODES.M;
import static com.tatfook.paracraft.screenrecorder.ScreenRecorderHelper.AUDIO_AAC;
import static com.tatfook.paracraft.screenrecorder.ScreenRecorderHelper.VIDEO_AVC;

public class ScreenRecorder {
    public static final int REQUEST_PERMISSIONS = 2;
    private static ScreenRecorder mInstance;
    private static final int REQUEST_MEDIA_PROJECTION = 1;
    private Notifications mNotifications;
    private MediaProjectionManager mMediaProjectionManager;
    private MediaProjection mMediaProjection;
    private ScreenRecorderHelper mRecorderHelper;
    private Intent mScreenRecorderServiceIntent;
    private String mVideoCodecName;
    private String mAudioCodecName;
    private VirtualDisplay mVirtualDisplay;
    private BroadcastReceiver mStopActionReceiver = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
            if (ACTION_STOP.equals(intent.getAction())) {
                Log.d("receive stop!!!", "33333333333");
    //                stopRecordingAndOpenFile(context);
            }
        }
    };

    static final String ACTION_STOP = BuildConfig.APPLICATION_ID + ".action.STOP";

    interface Callback {
        void onResult();
    }

    public static ScreenRecorder getInstance() {
        return mInstance;
    }

    public static void start() {
        if (!hasPermissions()) {
            requestPermissions();

            return;
        }

        if (getInstance() == null) {
            new ScreenRecorder(() -> {
                // request media projection
                Intent captureIntent = getInstance().getMediaProjectionManager().createScreenCaptureIntent();
                ParaEngineActivity.getContext().startActivityForResult(captureIntent, REQUEST_MEDIA_PROJECTION);
            });
        } else {
            if (getInstance().getMediaProjection() != null) {
                getInstance().startRecorder(getInstance().getMediaProjection());
            }
        }
    }

    public static void stop() {
        if (getInstance() == null) {
            return;
        }

        getInstance().stopRecorder();
    }

    public static void save() {

    }

    public static void play() {

    }

    public static void onActivityResult(int requestCode, int resultCode, Intent data) {
        if (requestCode == REQUEST_MEDIA_PROJECTION) {
            getInstance().mScreenRecorderServiceIntent =
                new Intent(ParaEngineActivity.getContext(), ScreenRecorderService.class);

            getInstance()
                .mScreenRecorderServiceIntent
                .putExtra("code", resultCode);

            getInstance()
                .mScreenRecorderServiceIntent
                .putExtra("data", data);

            if (data == null) {
                mInstance = null;
                return;
            }

            ParaEngineActivity
                .getContext()
                .startForegroundService(getInstance().mScreenRecorderServiceIntent);
        }
    }

    private static boolean hasPermissions() {
        PackageManager pm = ParaEngineActivity.getContext().getPackageManager();
        String packageName = ParaEngineActivity.getContext().getPackageName();

        int granted = pm.checkPermission(RECORD_AUDIO, packageName) | pm.checkPermission(WRITE_EXTERNAL_STORAGE, packageName);

        return granted == PackageManager.PERMISSION_GRANTED;
    }

//    @TargetApi(M)
    private static void requestPermissions() {
        ParaEngineActivity
            .getContext()
            .requestPermissions(
                    new String[]{WRITE_EXTERNAL_STORAGE, RECORD_AUDIO},
                    REQUEST_PERMISSIONS
            );
    }

    public ScreenRecorder(Callback callback) {
        mInstance = this;
        mNotifications = new Notifications(ParaEngineActivity.getContext().getApplicationContext());
        mMediaProjectionManager =
            (MediaProjectionManager) ParaEngineActivity.getContext()
            .getApplicationContext()
            .getSystemService(MEDIA_PROJECTION_SERVICE);

        // prepare video codec.
        Utils.findEncodersByTypeAsync(VIDEO_AVC, videoInfos -> {
            String[] videoNames = new String[videoInfos.length];

            for (int i = 0; i < videoInfos.length; i++) {
                videoNames[i] = videoInfos[i].getName();
            }

            if (videoNames[0] != null) {
                mVideoCodecName = videoNames[0];
            }

            Utils.findEncodersByTypeAsync(AUDIO_AAC, audioInfos -> {
                String[] audioNames = new String[videoInfos.length];

                for (int i = 0; i < audioInfos.length; i++) {
                    audioNames[i] = audioInfos[i].getName();
                }

                if (audioNames[0] != null) {
                    mAudioCodecName = audioNames[0];
                }
            });

            callback.onResult();
        });
    }

    public MediaProjectionManager getMediaProjectionManager() {
        return mMediaProjectionManager;
    }

    public Notifications getNotifications() {
        return mNotifications;
    }

    public void setMediaProjection(MediaProjection mediaProjection) {
        mMediaProjection = mediaProjection;
    }

    public MediaProjection getMediaProjection() {
        return mMediaProjection;
    }

    private ScreenRecorderHelper newRecorderHelper(MediaProjection mediaProjection,
                                                   VideoEncodeConfig video,
                                                   AudioEncodeConfig audio,
                                                   File output) {
        final VirtualDisplay display = getOrCreateVirtualDisplay(mediaProjection, video);

        ScreenRecorderHelper r = new ScreenRecorderHelper(video, audio, display, output.getAbsolutePath());

        r.setCallback(new ScreenRecorderHelper.Callback() {
            long startTime = 0;

            @Override
            public void onStop(Throwable error) {
                ParaEngineActivity.getContext().runOnUiThread(() -> stopRecorder());

                ParaEngineActivity
                    .getContext()
                    .stopService(mScreenRecorderServiceIntent);

                mInstance = null;

//                if (error != null) {
//                    toast("Recorder error ! See logcat for more details");
//                    error.printStackTrace();
//
//                    output.delete();
//                } else {
//                    Intent intent =
//                        new Intent(Intent.ACTION_MEDIA_SCANNER_SCAN_FILE)
//                        .addCategory(Intent.CATEGORY_DEFAULT)
//                        .setData(Uri.fromFile(output));
//
//                    ParaEngineActivity.getContext().sendBroadcast(intent);
//                }
            }

            @Override
            public void onStart() {
            }

            @Override
            public void onRecording(long presentationTimeUs) {
                if (startTime <= 0) {
                    startTime = presentationTimeUs;
                }

                long time = (presentationTimeUs - startTime) / 1000;
                mNotifications.recording(time);
            }
        });

        return r;
    }

    public void startRecorder(MediaProjection mediaProjection) {
        if (mRecorderHelper == null) {
            VideoEncodeConfig video = createVideoConfig();
            AudioEncodeConfig audio = createAudioConfig(); // audio can be null

            if (video == null) {
                return;
            }

            File dir =
                new File(
                    Environment
                        .getExternalStoragePublicDirectory(Environment.DIRECTORY_MOVIES),
                    "ParacraftScreenRecorder"
                );

            Stack<File> tmpFileStack = new Stack<File>();
            tmpFileStack.push(dir);

            try {
                while(false == tmpFileStack.isEmpty()) {
                    File curFile = tmpFileStack.pop();

                    if (null == curFile) {
                        continue;
                    }

                    if (curFile.isFile()) {
                        curFile.delete();
                    } else {
                        File[] tmpSubFileList = curFile.listFiles();

                        if (null == tmpSubFileList || 0 == tmpSubFileList.length) {	//空文件夹直接删
                            curFile.delete();
                        } else {
                            tmpFileStack.push(curFile);

                            for (File item : tmpSubFileList) {
                                tmpFileStack.push(item);
                            }
                        }
                    }
                }
            } catch (Exception e) {}

            if (!dir.exists() && !dir.mkdirs()) {
                cancelRecorder();
                return;
            }

            SimpleDateFormat format = new SimpleDateFormat("yyyyMMdd-HHmmss", Locale.SIMPLIFIED_CHINESE);
            final File file = new File(dir, "Paracraft-Screenshots-" + format.format(new Date())  + ".mp4");

            mRecorderHelper = newRecorderHelper(mediaProjection, video, audio, file);

            // permission
            if (!hasPermissions()) {
                cancelRecorder();
                return;
            }
        }

        if (mRecorderHelper == null) {
            return;
        }


        mRecorderHelper.start();
        ParaEngineActivity.getContext().registerReceiver(mStopActionReceiver, new IntentFilter(ACTION_STOP));
    }

    private void stopRecorder() {
//        mNotifications.clear();

        if (mRecorderHelper != null) {
            mRecorderHelper.quit();
        }

        mRecorderHelper = null;

        try {
            ParaEngineActivity.getContext().unregisterReceiver(mStopActionReceiver);
        } catch (Exception e) {
            //ignored
        }
    }

    private void cancelRecorder() {
        if (mRecorderHelper == null) return;

        stopRecorder();
    }

    private VirtualDisplay getOrCreateVirtualDisplay(MediaProjection mediaProjection, VideoEncodeConfig config) {
        if (mVirtualDisplay == null) {
            mVirtualDisplay =
                mediaProjection
                    .createVirtualDisplay(
                        "ScreenRecorder-display0",
                        config.width,
                        config.height,
                       1 /*dpi*/,
                        DisplayManager.VIRTUAL_DISPLAY_FLAG_PUBLIC,
                       null /*surface*/,
                       null,
                       null
                    );
        } else {
            // resize if size not matched
            Point size = new Point();
            mVirtualDisplay.getDisplay().getSize(size);

            if (size.x != config.width || size.y != config.height) {
                mVirtualDisplay.resize(config.width, config.height, 1);
            }
        }

        return mVirtualDisplay;
    }

    private VideoEncodeConfig createVideoConfig() {
        final String codec = mVideoCodecName;

        if (codec == null) {
            return null;
        }

        // video size
        int width = 1920;
        int height = 1080;
        int frameRate = 60;
        int iframe = 1;
        int bitrate = 16000 * 1000;
        MediaCodecInfo.CodecProfileLevel profileLevel = Utils.toProfileLevel("Default");

        return new VideoEncodeConfig(width, height, bitrate, frameRate, iframe, codec, VIDEO_AVC, profileLevel);
    }

    private AudioEncodeConfig createAudioConfig() {
        String codec = mAudioCodecName;

        if (codec == null) {
            return null;
        }

        int bitrate = 320 * 1000;
        int sampleRate = 44100;
        int channelCount = 2;
        int profile = MediaCodecInfo.CodecProfileLevel.AACObjectMain;

        return new AudioEncodeConfig(codec, AUDIO_AAC, bitrate, sampleRate, channelCount, profile);
    }

    private void toast(String message, Object... args) {
        Toast toast = Toast.makeText(ParaEngineActivity.getContext(),
                                     message,
                                     Toast.LENGTH_LONG);

        if (Looper.myLooper() != Looper.getMainLooper()) {
            ParaEngineActivity.getContext().runOnUiThread(toast::show);
        } else {
            toast.show();
        }
    }

    /**
     * Print information of all MediaCodec on this device.
     */
    private static void logCodecInfos(MediaCodecInfo[] codecInfos, String mimeType) {
        for (MediaCodecInfo info : codecInfos) {
            StringBuilder builder = new StringBuilder(512);
            MediaCodecInfo.CodecCapabilities caps = info.getCapabilitiesForType(mimeType);

            builder.append("Encoder '")
                   .append(info.getName()).append('\'')
                   .append("\n  supported : ")
                   .append(Arrays.toString(info.getSupportedTypes()));

            MediaCodecInfo.VideoCapabilities videoCaps = caps.getVideoCapabilities();

            if (videoCaps != null) {
                builder.append("\n  Video capabilities:")
                       .append("\n  Widths: ").append(videoCaps.getSupportedWidths())
                       .append("\n  Heights: ").append(videoCaps.getSupportedHeights())
                       .append("\n  Frame Rates: ").append(videoCaps.getSupportedFrameRates())
                       .append("\n  Bitrate: ").append(videoCaps.getBitrateRange());

                if (VIDEO_AVC.equals(mimeType)) {
                    MediaCodecInfo.CodecProfileLevel[] levels = caps.profileLevels;
                    builder.append("\n  Profile-levels: ");

                    for (MediaCodecInfo.CodecProfileLevel level : levels) {
                        builder.append("\n  ").append(Utils.avcProfileLevelToString(level));
                    }
                }

                builder.append("\n  Color-formats: ");

                for (int c : caps.colorFormats) {
                    builder.append("\n  ").append(Utils.toHumanReadable(c));
                }
            }

            MediaCodecInfo.AudioCapabilities audioCaps = caps.getAudioCapabilities();

            if (audioCaps != null) {
                builder.append("\n Audio capabilities:")
                       .append("\n Sample Rates: ").append(Arrays.toString(audioCaps.getSupportedSampleRates()))
                       .append("\n Bit Rates: ").append(audioCaps.getBitrateRange())
                       .append("\n Max channels: ").append(audioCaps.getMaxInputChannelCount());
            }

            Log.i("@@@", builder.toString());
        }
    }

//    private void stopRecordingAndOpenFile(Context context) {
//        File file = new File(mRecorder.getSavedPath());
//        stopRecorder();
//        Toast.makeText(context, getString(R.string.recorder_stopped_saved_file) + " " + file, Toast.LENGTH_LONG).show();
//        StrictMode.VmPolicy vmPolicy = StrictMode.getVmPolicy();
//
//        try {
//            // disable detecting FileUriExposure on public file
//            StrictMode.setVmPolicy(new StrictMode.VmPolicy.Builder().build());
//            viewResult(file);
//        } finally {
//            StrictMode.setVmPolicy(vmPolicy);
//        }
//    }
}