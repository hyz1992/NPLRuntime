#include "ParaEngine.h"
#include "VideoPlayerPlatform.h"
#include "IVideoPlayers.h"
#include "PluginManager.h"
#include "PluginAPI.h"
#include "IParaEngineApp.h"

using namespace ParaEngine;

#define VideoPlayer_CLASS_ID Class_ID(0x20230326, 0x03262023)

/** the movie codec plugin dll file name. We will try these location in order */
#ifdef _DEBUG
const char* VIDEO_PLAYER_DLL_FILE_PATHS[] = { "Mod/VideoPlayerPlugin/VideoPlayerPlugin_d.dll",  "Mod/VideoPlayerPlugin/VideoPlayerPlugin.dll", "VideoPlayerPlugin_d.dll" };
#else
const char* VIDEO_PLAYER_DLL_FILE_PATHS[] = { "Mod/VideoPlayerPlugin/VideoPlayerPlugin.dll", "VideoPlayerPlugin.dll" };
#endif

CVideoPlayerPlatform::CVideoPlayerPlatform(void)
{

}

CVideoPlayerPlatform::~CVideoPlayerPlatform(void)
{
	Cleanup();
}

void CVideoPlayerPlatform::Cleanup()
{
	if (m_pVideoPlayer != 0)
	{
		//m_pVideoPlayer->PlayerStop(0);
		m_pVideoPlayer = NULL;
	}
}

CVideoPlayerPlatform& CVideoPlayerPlatform::GetSingleton()
{
	static CVideoPlayerPlatform g_singleton;
	return g_singleton;
}

IVideoPlayers* CVideoPlayerPlatform::GetVideoPlayers(bool bCreateIfNotExist)
{
	if (m_pVideoPlayer)
	{
		return m_pVideoPlayer;
	}
	else
	{
		static bool s_bIsLoaded = false;
		if (s_bIsLoaded || !bCreateIfNotExist)
		{
			return m_pVideoPlayer;
		}
		else
		{
			s_bIsLoaded = true;

			ParaEngine::DLLPlugInEntity* pPluginEntity = NULL;
			for (int i = 0; m_pVideoPlayer == 0 && i < sizeof(VIDEO_PLAYER_DLL_FILE_PATHS) / sizeof(const char*); ++i)
			{
				const char* sFilename = VIDEO_PLAYER_DLL_FILE_PATHS[i];

				pPluginEntity = CGlobals::GetPluginManager()->GetPluginEntity(sFilename);

				if (pPluginEntity == 0)
				{
					// load the plug-in if it has never been loaded before. 
					pPluginEntity = ParaEngine::CGlobals::GetPluginManager()->LoadDLL("", sFilename);
				}

				if (pPluginEntity != 0 && pPluginEntity->IsValid())
				{
					m_nVideoPlayerVersion = pPluginEntity->GetLibVersion();
					if (m_nVideoPlayerVersion >= 1)
					{
						for (int i = 0; i < pPluginEntity->GetNumberOfClasses(); ++i)
						{
							ClassDescriptor* pClassDesc = pPluginEntity->GetClassDescriptor(i);

							if (pClassDesc && pClassDesc->ClassID() == VideoPlayer_CLASS_ID)
							{
								m_pVideoPlayer = (IVideoPlayers*)pClassDesc->Create();
							}
						}
					}
					else
					{
						OUTPUT_LOG("movie codec require at least version 9 but you only have version %d\n", m_nVideoPlayerVersion);
						CGlobals::GetApp()->SystemMessageBox("MovieCodec plugin needs at least version 9. Please update from official website!");
					}
				}
			}
		}
		return m_pVideoPlayer;
	}
}

int CVideoPlayerPlatform::InstallFields(CAttributeClass* pClass, bool bOverride)
{
	IAttributeFields::InstallFields(pClass, bOverride);

	IVideoPlayers* pVideoplayer = GetVideoPlayers();
	if (pVideoplayer)
	{

	}
	//pClass->AddField("codec", FieldType_Int, (void*)SetCodec_s, (void*)GetCodec_s, NULL, NULL, bOverride);
	//pClass->AddField("video_encode_example", FieldType_String, (void*)video_encode_example_s, NULL, NULL, NULL, bOverride);
	
	return S_OK;
}