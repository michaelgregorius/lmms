INCLUDE(BuildPlugin)

BUILD_PLUGIN(WaveAnalyzer
	WaveAnalyzer.cpp
	WaveAnalyzerControls.cpp
	WaveAnalyzerControlDialog.cpp
	WaveAnalyzerLevelIndicator.cpp
	WaveAnalyzerClipIndicator.cpp
	MOCFILES
	WaveAnalyzer.h
	WaveAnalyzerControls.h
	WaveAnalyzerControlDialog.h
	WaveAnalyzerLevelIndicator.h
	WaveAnalyzerClipIndicator.h
	EMBEDDED_RESOURCES
	*.png
)
