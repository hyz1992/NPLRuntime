//-----------------------------------------------------------------------------
// Class: ParaScriptingScreenRecorder.cpp
// Authors: big
// CreateDate: 2021.12.14
//-----------------------------------------------------------------------------

#import "ScreenRecorder/ScreenRecorder.h"

#include "ParaScriptingScreenRecorder.h"

namespace ParaScripting
{
	void ParaScriptingScreenRecorder::Start()
	{
        if (![ScreenRecorder getInstance]) {
            ScreenRecorder *instance = [[ScreenRecorder alloc] init];
            [ScreenRecorder setInstance:instance];
        }

        ScreenRecorder *instance = [ScreenRecorder getInstance];

        [instance startRecordingWithHandler:^(NSError *error) {}];
	}

    void ParaScriptingScreenRecorder::Stop()
    {
        ScreenRecorder *instance = [ScreenRecorder getInstance];

        [instance stopRecordingWithHandler];
    }

    void ParaScriptingScreenRecorder::Save()
    {
        ScreenRecorder *instance = [ScreenRecorder getInstance];

        [instance saveVideo];
    }

    void ParaScriptingScreenRecorder::Play()
    {
        ScreenRecorder *instance = [ScreenRecorder getInstance];
        [instance playPreview];
    }
}