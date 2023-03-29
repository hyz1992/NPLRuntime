#pragma once
#include "IAttributeFields.h"
#include "IVideoPlayers.h"
//#include "AttributeClass.h"
namespace ParaEngine
{
	class CVideoPlayerPlatform : public IAttributeFields
	{
	public:
		CVideoPlayerPlatform(void);
		virtual ~CVideoPlayerPlatform(void);
		static CVideoPlayerPlatform& GetSingleton();

		void Cleanup();

		/** try get the movie codec. this function may return NULL if dll does not exist. */
		IVideoPlayers* GetVideoPlayers(bool bCreateIfNotExist = true);

	public:
		//////////////////////////////////////////////////////////////////////////
		// implementation of IAttributeFields

		/** attribute class ID should be identical, unless one knows how overriding rules work.*/
		virtual int GetAttributeClassID() { return ATTRIBUTE_CLASSID_VideoPlayer; }
		/** a static string, describing the attribute class object's name */
		virtual const char* GetAttributeClassName() { static const char name[] = "VideoPlayerPlatform"; return name; }
		/** a static string, describing the attribute class object */
		virtual const char* GetAttributeClassDescription() { static const char desc[] = ""; return desc; }
		/** this class should be implemented if one wants to add new attribute. This function is always called internally.*/
		virtual int InstallFields(CAttributeClass* pClass, bool bOverride);

	private:
		IVideoPlayers* m_pVideoPlayer;
		int m_nVideoPlayerVersion;
	};
}