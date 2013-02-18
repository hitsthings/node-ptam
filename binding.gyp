{
    "targets": [{
        "target_name": "ptam",
        "dependencies": [
            "ptam-core"
        ],
        "sources": [
            "src/ptam.cc",
            "src/PTAMWrap.cpp"
        ],
        "include_dirs": [
            '../libcvd_installed/include',
            '../TooN-2.0',
            "../glew-1.9.0/include",
            "../gvars-3.0",
            "../pthreads-w32-2-9-1-release/Pre-built.2/include",
            "PTAM"
        ],
        "libraries": [
            "../../Libs/liblapack.lib",
            "../../Libs/libblas.lib",
            "../../libcvd_installed/lib/libcvd.lib",
            "../../glew-1.9.0/lib/glew32s.lib",
            "../../gvars3_installed/lib/gvars3-headless.lib",
            "../../pthreads-w32-2-9-1-release/Pre-built.2/lib/x86/pthreadVSE2.lib"
        ]
    }, {
        "target_name": "ptam-core",
        "type": "static_library",
        "defines": [
            "GLEW_STATIC"
        ],
        "sources": [
            #"PTAM/ARDriver.cc",
            "PTAM/ATANCamera.cc",
            "PTAM/Bundle.cc",
            "PTAM/CalibCornerPatch.cc",
            "PTAM/CalibImage.cc",
            #"PTAM/EyeGame.cc",
            "PTAM/HomographyInit.cc",
            "PTAM/KeyFrame.cc",
            "PTAM/Map.cc",
            "PTAM/MapMaker.cc",
            "PTAM/MapPoint.cc",
            #"PTAM/MapViewer.cc",
            "PTAM/MiniPatch.cc",
            "PTAM/PatchFinder.cc",
            "PTAM/Relocaliser.cc",
            "PTAM/ShiTomasi.cc",
            "PTAM/SmallBlurryImage.cc",
            "PTAM/System.cc",
            "PTAM/Tracker.cc",
            #"PTAM/VideoSource_Linux_DV.cc",
            #"PTAM/VideoSource_Linux_V4L.cc",
            #"PTAM/VideoSource_Win32_CMU1394.cc"
        ],
        "include_dirs": [
            "../libcvd_installed/include",
            '../TooN-2.0',
            "../glew-1.9.0/include",
            "../gvars-3.0",
            "../pthreads-w32-2-9-1-release/Pre-built.2/include",
            "PTAM"
        ],
        "libraries": [
            "../../Libs/liblapack.lib",
            "../../Libs/libblas.lib",
            "../../libcvd_installed/lib/libcvd.lib",
            "../../glew-1.9.0/lib/glew32s.lib",
            "../../gvars3_installed/lib/gvars3-headless.lib",
            "../../pthreads-w32-2-9-1-release/Pre-built.2/lib/x86/pthreadVSE2.lib"
        ]
    }]
}
