#ifndef SECS_SECS_HPP
#define SECS_SECS_HPP

#include <vector>
#include <bitset>
#include <map>
#include <unordered_map>
#include <memory>

namespace secs
{

typedef std::size_t Entity;
typedef std::size_t ComponentId;

class Archetype;
class EntityManager;
class BaseComponentList;
template<typename T> class ComponentList;
class ComponentManager;
class System;
class SystemManager;
class SECS;

class Archetype
{
public:

    void setComponent(secs::ComponentId id, bool value);
    bool isComponentSet(secs::ComponentId id);

private:

    std::vector<bool> mComponents {};
    friend bool checkIfSubarchetype(const Archetype& superArchetype, const Archetype& subArchetype);
    friend bool operator==(const Archetype& a1, const Archetype& a2);
    friend bool operator!=(const Archetype& a1, const Archetype& a2);
};

bool checkIfSubarchetype(const Archetype& superArchetype, const Archetype& subArchetype);
bool operator==(const Archetype& a1, const Archetype& a2);
bool operator!=(const Archetype& a1, const Archetype& a2);

class EntityDoesntExist : public std::exception
{
public:

    const char * what() const noexcept override;
};

class EntityManager
{
public:

    Entity createEntity();
    bool deleteEntity(Entity entity);

    Archetype getEntityArchetype(Entity entity) const;
    void setEntityArchetype(Entity entity, Archetype archetype);

    const std::unordered_map<Entity, Archetype> & getEntityArchetypes() const noexcept;

private:

    std::unordered_map<Entity, Archetype> mEntityArchetypes {};
    std::vector<Entity> deletedEntities {};

    Entity mCreatedEntityCount = 0;

    Entity generateNewId();
};

class BaseComponentList
{
public:

    virtual ~BaseComponentList() = default;
    virtual bool deleteComponent(Entity entity) = 0;
    virtual void entityDeleted(Entity entity) = 0;
};


class ComponentDoesntExist : public std::exception
{
public:

    const char * what() const noexcept override;
};

class DuplicateComponent : public std::exception
{
public:

    const char * what() const noexcept override;
};

template<typename T>
class ComponentList : public BaseComponentList
{
public:

    void addComponent(Entity entity) {
        auto pIter = mComponents.find(entity);
        if (pIter != mComponents.end())
            throw DuplicateComponent();
        mComponents.insert({entity, T()});
    }

    void addComponent(Entity entity, T component) {
        auto pIter = mComponents.find(entity);
        if (pIter != mComponents.end())
            throw DuplicateComponent();
        mComponents.insert({entity, component});
    }


    bool deleteComponent(Entity entity) override {
        auto pIter = mComponents.find(entity);
        if (pIter == mComponents.end())
            return false;
        mComponents.erase(pIter);
        return true;
    }

    void entityDeleted(Entity entity) override {
        mComponents.erase(entity);
    }

    T * getComponent(Entity entity) {
        auto pIter = mComponents.find(entity);
        if (pIter == mComponents.end())
            throw ComponentDoesntExist();
        return &(pIter->second);
    }

private:

    std::unordered_map<Entity, T> mComponents {};
};


class ComponentNotRecognised : public std::exception
{
    const char * what() const noexcept override;
};

class ComponentManager
{
public:

    template<typename T>
    void registerComponent() {
        const char* typeName = typeid(T).name();
        if (mComponentLists.find(typeName) != mComponentLists.end())
            throw DuplicateComponent();
        std::shared_ptr<ComponentList<T>> pComponentList = std::make_shared<ComponentList<T>>();
        mComponentLists.insert({typeName, pComponentList});
    }

    template<typename T>
    ComponentId getComponentId() {
        static ComponentId id = mUniqueComponentCount++;
        return id;
    }

    template<typename T>
    void addComponent(Entity entity) {
        const char* typeName = typeid(T).name();
        auto pIter = mComponentLists.find(typeName);
        if (pIter == mComponentLists.end())
            throw ComponentNotRecognised();
        std::shared_ptr<ComponentList<T>> pComponentList = std::static_pointer_cast<ComponentList<T>>(pIter->second);
        pComponentList->addComponent(entity);
    }

    template<typename T>
    void addComponent(Entity entity, T component) {
        const char* typeName = typeid(T).name();
        auto pIter = mComponentLists.find(typeName);
        if (pIter == mComponentLists.end())
            throw ComponentNotRecognised();
        std::shared_ptr<ComponentList<T>> pComponentList = std::static_pointer_cast<ComponentList<T>>(pIter->second);
        pComponentList->addComponent(entity, component);
    }

    template<typename T>
    bool deleteComponent(Entity entity) {
        const char* typeName = typeid(T).name();
        auto pIter = mComponentLists.find(typeName);
        if (pIter == mComponentLists.end())
            return false;
        return pIter->second->deleteComponent(entity);
    }

    template<typename T>
    T * getComponent(Entity entity) {
        const char* typeName = typeid(T).name();
        auto pIter = mComponentLists.find(typeName);
        if (pIter == mComponentLists.end())
            throw ComponentDoesntExist();
        std::shared_ptr<ComponentList<T>> pComponentList = std::static_pointer_cast<ComponentList<T>>(mComponentLists[typeName]);
        return pComponentList->getComponent(entity);
    }

    void entityDeleted(Entity entity);

private:

    std::unordered_map<const char*, std::shared_ptr<BaseComponentList>> mComponentLists {};

    std::size_t mUniqueComponentCount = 0;
};

class ECSPointerIsNULL : public std::exception
{
    const char * what() const noexcept override;
};

class System
{
protected:

    std::vector<Entity> mEntities {};

    SECS * getECS() {
        if (!mpECS)
            throw ECSPointerIsNULL();
        return mpECS;
    }

private:

    std::unordered_map<Entity, std::size_t> mEntityToIndex {};

    SECS *mpECS = nullptr;

    friend class SystemManager;
    friend class SECS;
};

class SystemNotActivated : public std::exception
{
public:

    const char* what() const noexcept override;
};

class SystemAlreadyActivated : public std::exception
{
public:

    const char* what() const noexcept override;
};

class SystemManager
{
public:

    template<typename T>
    void activateSystem(System *system) {
        const char *typeName = typeid(T).name();
        auto pIter = mNameToSystem.find(typeName);
        if (pIter != mNameToSystem.end())
            throw SystemAlreadyActivated();
        mNameToSystem.insert({typeName, system});
        mSystemDependencies.insert({typeName, Archetype()});
    }

    template<typename T>
    bool deactivateSystem() {
        const char *typeName = typeid(T).name();
        auto pIter = mNameToSystem.find(typeName);
        if (pIter == mNameToSystem.end())
            return false;
        pIter->second->mEntities = {};
        pIter->second->mpECS = nullptr;
        mNameToSystem.erase(pIter);
        mSystemDependencies.erase(typeName);
        return true;
    }

    template<typename T>
    Archetype getSystemDependencies() const {
        const char *typeName = typeid(T).name();
        auto pIter = mSystemDependencies.find(typeName);
        if (pIter == mSystemDependencies.end())
            throw SystemNotActivated();
        return pIter->second;
    }

    Archetype getSystemDependencies(const char *systemName) const;

    template<typename T>
    void setSystemDependencies(const Archetype &dependencies) {
        const char *typeName = typeid(T).name();
        auto pIter = mSystemDependencies.find(typeName);
        if (pIter == mSystemDependencies.end())
            throw SystemNotActivated();
        pIter->second = dependencies;
    }

    void updateSystemsFor(Entity entity, Archetype entityArchetype);

    template<typename T>
    void addEntityToSystem(Entity entity) {
        const char *typeName = typeid(T).name();
        auto pIter = mNameToSystem.find(typeName);
        if (pIter == mNameToSystem.end())
            throw SystemNotActivated();
        System* pSystem = pIter->second;
        pSystem->mEntityToIndex.insert({entity, pSystem->mEntities.size()});
        pSystem->mEntities.push_back(entity);
    }

    template<typename T>
    bool removeEntityFromSystem(Entity entity) {
        const char *typeName = typeid(T).name();
        auto pIter = mNameToSystem.find(typeName);
        if (pIter == mNameToSystem.end())
            return false;
        System* pSystem = pIter->second;
        std::size_t index = pSystem->mEntityToIndex.find(entity)->second;
        pSystem->mEntities.erase(pSystem->mEntities.begin() + index);
        pSystem->mEntityToIndex.erase(entity);
        return true;
    }

    void entityDeleted(Entity entity);
    void sECSDeleted();

private:

    std::unordered_map<const char *, System *> mNameToSystem {};
    std::map<const char *, Archetype> mSystemDependencies {};
};

class SECS
{
public:

    ~SECS();

    Entity createEntity();
    bool deleteEntity(Entity entity);

    template<typename T>
    ComponentId getComponentId() {
        return mComponentManager.getComponentId<T>();
    }

    template<typename T>
    void registerComponent() {
        mComponentManager.registerComponent<T>();
    }

    template<typename T>
    T * getComponent(Entity entity) {
        return mComponentManager.getComponent<T>(entity) ;
    }

    template<typename T>
    void addComponent(Entity entity) {
        mComponentManager.addComponent<T>(entity);
        Archetype archetype = mEntityManager.getEntityArchetype(entity);
        archetype.setComponent(this->getComponentId<T>(), true);
        mEntityManager.setEntityArchetype(entity, archetype);
        mSystemManager.updateSystemsFor(entity, archetype);
    }

    template<typename T>
    void addComponent(Entity entity, T component) {
        mComponentManager.addComponent<T>(entity, component);
        Archetype archetype = mEntityManager.getEntityArchetype(entity);
        archetype.setComponent(this->getComponentId<T>(), true);
        mEntityManager.setEntityArchetype(entity, archetype);
        mSystemManager.updateSystemsFor(entity, archetype);
    }

    template<typename T>
    bool deleteComponent(Entity entity) {
        Archetype archetype = mEntityManager.getEntityArchetype(entity);
        archetype.setComponent(this->getComponentId<T>(), false);
        mEntityManager.setEntityArchetype(entity, archetype);
        mSystemManager.updateSystemsFor(entity, archetype);
        return mComponentManager.deleteComponent<T>(entity);
    }

    template<typename T>
    bool checkIfHasComponent(Entity entity) {
        return mEntityManager.getEntityArchetype(entity).isComponentSet(getComponentId<T>());
    }

    template<typename T>
    void setArchetypeComponent(secs::Archetype &archetype, bool value = true) {
        archetype.setComponent(mComponentManager.getComponentId<T>(), value);
    }

    template<typename T>
    bool isArchetypeComponentSet(secs::Archetype &archetype) {
        return archetype.isComponentSet(mComponentManager.getComponentId<T>());
    }

    template<typename T>
    void activateSystem(System *system) {
        system->mpECS = this;
        mSystemManager.activateSystem<T>(system);
        const std::unordered_map<Entity, Archetype> entityArchetypes = mEntityManager.getEntityArchetypes();
        for (auto cpIter = entityArchetypes.begin(); cpIter != entityArchetypes.end(); cpIter++) {
            mSystemManager.addEntityToSystem<T>(cpIter->first);
        }
    }

    template<typename T>
    bool deactivateSystem() {
        return mSystemManager.deactivateSystem<T>();
    }

    template<typename T>
    Archetype getSystemDependencies() const {
        return mSystemManager.getSystemDependencies<T>();
    }

    template<typename T>
    void setSystemDependencies(Archetype dependencies) {
        mSystemManager.setSystemDependencies<T>(dependencies);
        const std::unordered_map<Entity, Archetype> entityArchetypes = mEntityManager.getEntityArchetypes();
        for (auto cpIter = entityArchetypes.begin(); cpIter != entityArchetypes.end(); cpIter++) {
            if (!checkIfSubarchetype(dependencies, cpIter->second)) {
                mSystemManager.removeEntityFromSystem<T>(cpIter->first);
                continue;
            }
            mSystemManager.addEntityToSystem<T>(cpIter->first);
        }
    }

    template<typename TSystem, typename TComponent>
    void setSystemDependency(bool value = true) {
        Archetype archetype = getSystemDependencies<TSystem>();
        archetype.setComponent(getComponentId<TComponent>(), value);
        setSystemDependencies<TSystem>(archetype);
    }

private:

    EntityManager mEntityManager {};
    ComponentManager mComponentManager {};
    SystemManager mSystemManager {};
};
}

#endif //SECS_SECS_HPP