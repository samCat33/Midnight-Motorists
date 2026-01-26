#ifndef ECS_H
#define ECS_H

#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <bitset>
#include <array>
using namespace std;

class Component;
class Entity;
class Manager;

using ComponentID = size_t;
using Group = size_t;

inline ComponentID getNewComponentTypeID() {
	static ComponentID lastID = 0u;
	return lastID++;
}

template <typename T> inline 
ComponentID getComponentTypeID() noexcept {
	static ComponentID typeID = getNewComponentTypeID();
	return typeID;
}

constexpr size_t maxComponents = 32;
constexpr size_t maxGroups = 32;

using ComponentBitSet = bitset<maxComponents>;
using GroupBitSet= bitset<maxComponents>;

//array<dataType, size> is better than a traditional array
// in that it allows iterators and it knows its own size
using ComponentArray = array<Component*, maxComponents>;


class Component {
	public:
		Entity* entity;

		virtual void init(){}
		virtual void update(){}
		virtual void draw(){}
		virtual ~Component(){}
};
/// <summary>
/// Component class
/// </summary>

class Entity {
private:
	Manager& manager;
	bool active = true;

	vector<unique_ptr<Component>> components;

	ComponentArray componentArray = {};
	ComponentBitSet componentBitSet = {};
	GroupBitSet groupBitSet = {};

public:
	//Sets reference member in constructor
	Entity(Manager& myManager) : manager(myManager){};

	void update() {
		for (auto& c : components) c->update();
	}
	void draw() {
		for (auto& c : components) c->draw();
	}

	bool isActive() const{
		return active;
	}

	void destroy(){
		active = false;
	}

	bool hasGroup(Group mGroup) {
		return groupBitSet[mGroup];
	}

	//This function is defined in ECS.h and
	//expanded upon in ECS.cpp
	void addGroup(Group mGroup);

	void delGroup(Group mGroup) {
		groupBitSet[mGroup] = false;
	}



	template <typename T> bool hasComponent() const {
		
		return componentBitSet[getComponentTypeID<T>()];
	}

	template<typename T, typename... TArgs> T& addComponent(TArgs&&... mArgs) {
		T* c(new T(forward<TArgs>(mArgs)...));
		c->entity = this;
		unique_ptr<Component> uPtr{c};
		components.emplace_back(move(uPtr));
		
		componentArray[getComponentTypeID<T>()] = c;
		componentBitSet[getComponentTypeID<T>()] = true;

		//INITIALIZE THE COMPONENT
		c->init();

		return *c;
	}

	template <typename T> T& getComponent() const {
		auto ptr(componentArray[getComponentTypeID<T>()]);

		return *static_cast<T*>(ptr);
	}
};
/// <summary>
/// Entity class
/// </summary>

class Manager {
	private:
		//Unique_ptrs provide exclusive ownership of dynamically allocated
		//objects, auutomatically deleting them when they go out of scope.
		//This helps to prevent memory leaks.
		vector<unique_ptr<Entity>> entities;

		array<vector<Entity*>, maxGroups> groupedEntities;

	public:
		void update() {
			for (auto& e : entities) e->update();
		}

		void draw() {
			for (auto& e : entities) e->draw();
		}

		void refresh() {

			//Erase all entities in each group
			for (unsigned int i = 0; i < maxGroups; ++i) {

				//This returns the vector of entities at a given group
				auto& v (groupedEntities[i]);

				//Move all the inactive or groupless entities to the back
				//of the vector of entities before deleting them

				//[i](Entity* entity) accesses each entity at the ith group
				v.erase(remove_if(begin(v), end(v), [i](Entity* entity) {
					return !entity->isActive() || !entity->hasGroup(i);
					}
				), end(v));
			}

			//entities is a vector of pointers to entities
			//Moves all inactive entities that have not yet been removed
			//to the back of the entities vector before erasing them.
			entities.erase(remove_if(begin(entities), end(entities), 
				[](const unique_ptr<Entity>& mEntity) {
					return !mEntity->isActive();
				}), end(entities));
		}
		

		void AddToGroup(Entity* mEntity, Group mGroup){
			groupedEntities[mGroup].emplace_back(mEntity);
		}

		vector<Entity*>& GetEntitiesInGroup(Group mGroup) {
			return groupedEntities[mGroup];
		}

		Entity& addEntity() {
			Entity* e = new Entity(*this);
			unique_ptr<Entity> uPtr {e};

			//Management of uPtr is transferred to the entities vector
			entities.emplace_back(move(uPtr));

			return *e;
		}
};
/// <summary>
/// Manager class
/// </summary>


#endif