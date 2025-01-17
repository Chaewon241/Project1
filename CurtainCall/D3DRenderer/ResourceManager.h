#pragma once
#include <map>
#include <memory>
#include <string>
#include "ModelResource.h"

class MaterialTexture;
class Animation;

class ResourceManager
{
public:
	~ResourceManager();

	
private:
	ResourceManager() = default;
	static ResourceManager* Instance;

public:
	static ResourceManager* GetInstance()
	{
		if (!Instance)
			Instance = new ResourceManager;

		return Instance;
	}

	std::map<std::wstring, std::weak_ptr<MaterialTexture>> m_MaterialTextureMap;
	std::map<std::string, std::weak_ptr<ModelResource>> m_StaticModelMap;
	std::map<std::string, std::weak_ptr<ModelResource>> m_SkeletalModelMap;
	std::map<std::string, std::weak_ptr<Animation>> m_AnimationMap;	// Animation


	std::shared_ptr<MaterialTexture> CreateMaterialTexture(std::wstring filePath);
	std::shared_ptr<ModelResource> CreateModelResource(std::string filePath, ModelType modelType);
	std::shared_ptr<Animation> CreateAnimationResource(std::string filePath);

	// 같은 머터리얼 내에서도 같은 텍스처를 사용할수 있으므로 공유 목록에서 파일경로 확인해서 얻어오기
	std::shared_ptr<MaterialTexture> CreateTextureResource(std::wstring filePath);
};
