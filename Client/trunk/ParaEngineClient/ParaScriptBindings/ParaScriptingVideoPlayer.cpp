#include "ParaEngine.h"

#include "util/StringHelper.h"
#include "AISimulator.h"
#include "ParaScriptingVideoPlayer.h"
#include "VideoPlayerPlatform.h"

using namespace ParaEngine;

namespace ParaScripting
{
	//////////////////////////////////////////////////////////////////////////
	//
	// screen capture and movie setting
	//
	//////////////////////////////////////////////////////////////////////////

	ParaScripting::ParaAttributeObject ParaVideoPlayer::GetAttributeObject()
	{
		return ParaAttributeObject(&CVideoPlayerPlatform::GetSingleton());
	}
}