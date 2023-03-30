#include "ParaEngine.h"

#include "util/StringHelper.h"
#include "AISimulator.h"
#include "ParaScriptingVideoPlayer.h"
#include "VideoPlayerPlatform.h"
#include "IVideoPlayers.h"

#include "2dengine/GUIRoot.h"

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

	void ParaVideoPlayer::PlayVideo(std::string uiname, std::string movie_path, int width, int height)
	{
		IVideoPlayers * pVideo = CVideoPlayerPlatform::GetSingleton().GetVideoPlayers(true);
		if (pVideo==nullptr) {
			return;
		}
		CGlobals::GetGUI()->GetUIObject(uiname);
		pVideo->PlayerCreate(0, width, height, 0, NULL);
		pVideo->setCallback_onPosition([](int32_t id, ParaEngine::PlayerState* state) {
			std::cout << "setCallback_onPosition: " << state->position() << std::endl;
		});
		pVideo->setCallback_onVideoFrame(0,[](uint8 *frame,int width,int height){
			static std::vector<char> buf;
			for (int i = 0; i < width; i++) {
				for (int j = 0; j < height; j++) {
					int index = j * width + i;
					//第j行第i列
					int r = frame[index + 0];
					int g = frame[index + 1];
					int b = frame[index + 2];
					int a = frame[index + 3];

					
				}
			}
			int i = 0;
		});
		
		pVideo->PlayerAdd(0, "MediaType.file", movie_path.c_str());
		pVideo->PlayerPlay(0);
	}
}