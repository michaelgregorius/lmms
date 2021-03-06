INCLUDE(BuildPlugin)

BUILD_PLUGIN(WaveAnalyzer WaveAnalyzer.cpp WaveAnalyzerControls.cpp WaveAnalyzerControlDialog.cpp MOCFILES WaveAnalyzer.h WaveAnalyzerControls.h WaveAnalyzerControlDialog.h EMBEDDED_RESOURCES *.png)
