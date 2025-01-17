#pragma once

#include "GameObject.h"

#include <memory>
#include <vector>
#include <unordered_map>
#include <cassert>

enum class eWorldType
{
	TEST,
	TEST2,

	END
};

class StaticMeshModel;
class SkeletalMeshModel;

/// <summary>
/// World의 원형 클래스
/// GameApp은 World 매니저를 통해 World들을 관리한다.
///	Init, Finalize와 OnEnter, OnExit이 구분되어 있다.
///	사용시 직접적으로 world->init, finalize는 하지 말 것
/// World는 GameObject 리스트를 들고 있다.
/// </summary>
class World
	: public std::enable_shared_from_this<World>
{
public:
	World();
	virtual ~World();

public:
	// GetGameObjects를 통해 얻은 벡터를 멤버변수 등으로 사용하여 객체 생명에 혼동을 주면 안된다.
	// 사용 용도 등을 보고 후에 구현
	// std::vector<std::weak_ptr<GameObject>>& GetGameObjects(eObjectType type) const;
	std::shared_ptr<GameObject> GetGameObject(std::string_view name);
	std::string GetName() const { return m_name; }
	
	void SetWorldType(eWorldType worldType) { m_worldType = worldType; }
	void SetName(std::string_view name) { m_name = name; }

public:
	virtual void Update(float deltaTime);

	/// Init, Finalize 이외에 월드 전환시 필요한 추가 행동
	virtual void OnEnter();
	virtual void OnExit();

protected:
	/// 오브젝트 생성, 파괴
	virtual void Initialize();
	virtual void Finalize();


protected:
	std::string m_name;
	eWorldType m_worldType = eWorldType::END;

	std::vector<std::shared_ptr<GameObject>> m_gameObjects[static_cast<int>(eObjectType::END)];
	std::unordered_map<std::string, std::pair<eObjectType, int>> m_objectMap;


public:
	template <typename T>
	std::weak_ptr<T> CreateGameObject(std::string_view objName, eObjectType objectType)
	{
		std::shared_ptr<T> obj = std::make_shared<T>();
		GameObject* inputObj = static_cast<GameObject*>(obj.get());
		assert(inputObj);

		obj->SetName(objName);
		obj->SetObjectType(objectType);
		obj->SetOwnerWorld(shared_from_this());

		int keyIdx = m_gameObjects[static_cast<int>(objectType)].size();
		m_objectMap.emplace(objName, std::make_pair(objectType, keyIdx));

		m_gameObjects[static_cast<int>(objectType)].emplace_back(obj);

		return obj;
	}
};

