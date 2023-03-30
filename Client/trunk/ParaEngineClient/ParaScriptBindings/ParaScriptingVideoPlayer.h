
#pragma once
#include "ParaScriptingCommon.h"

#include <string>
namespace ParaScripting
{
	using namespace std;
	/**
	* @ingroup ParaMovie
	movie making and screen capture functions  */
	class PE_CORE_DECL ParaVideoPlayer
	{
	public:
		/** get the attribute object associated with an object. */
		static ParaAttributeObject GetAttributeObject();

		static void PlayVideo(std::string uiname,std::string movie_path,int width,int height);

	};
}
