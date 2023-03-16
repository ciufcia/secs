#pragma once

#include <vector>
#include <unordered_map>
#include <memory>
#include <algorithm>

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
        [[nodiscard]] bool isComponentSet(secs::ComponentId id);

    private:

        std::vector<bool> mComponents {};
        friend bool checkIfSubArchetype(const Archetype& superArchetype, const Archetype& subArchetype);
        friend bool operator==(const Archetype& a1, const Archetype& a2);
        friend bool operator!=(const Archetype& a1, const Archetype& a2);
    };

    [[nodiscard]] bool checkIfSubArchetype(const Archetype& superArchetype, const Archetype& subArchetype);
    [[nodiscard]] bool operator==(const Archetype& a1, const Archetype& a2);
    [[nodiscard]] bool operator!=(const Archetype& a1, const Archetype& a2);

    class EntityDoesntExist : public std::exception
    {
    public:

        [[nodiscard]] const char * what() const noexcept override;
    };

    class EntityManager
    {
    public:

        Entity createEntity();
        void deleteEntity(Entity entity);

        [[nodiscard]] Archetype getEntityArchetype(Entity entity) const;
        void setEntityArchetype(Entity entity, const Archetype &archetype);

        [[nodiscard]] const std::unordered_map<Entity, Archetype> & getEntityArchetypes() const noexcept;

        [[nodiscard]] const std::vector<Entity> getEntities(const Archetype &filter = Archetype()) const;

    private:

        std::unordered_map<Entity, Archetype> mEntityArchetypes {};
        std::vector<Entity> deletedEntities {};

        Entity mCreatedEntityCount = 0;

        [[nodiscard]] Entity generateNewId();
    };

    class BaseComponentList
    {
    public:

        virtual ~BaseComponentList() = default;
        virtual void deleteComponent(Entity entity) = 0;
        virtual void entityDeleted(Entity entity) = 0;
    };

    class ComponentDoesntExist : public std::exception
    {
    public:

        [[nodiscard]] const char * what() const noexcept override;
    };

    class DuplicateComponent : public std::exception
    {
    public:

        [[nodiscard]] const char * what() const noexcept override;
    };

    template<typename T>
    class ComponentList : public BaseComponentList
    {
    public:

        void addComponent(Entity entity) {
            if (checkIfComponentExists(entity))
                throw DuplicateComponent();

            mComponents.insert({entity, T()});
        }

        void addComponent(Entity entity, T component) {
            if (checkIfComponentExists(entity))
                throw DuplicateComponent();

            mComponents.insert({entity, component});
        }

        void deleteComponent(Entity entity) override {
            mComponents.erase(getComponentIterator(entity));
        }

        void entityDeleted(Entity entity) override {
            auto pIter = mComponents.find(entity);

            if (pIter == mComponents.end())
                return;

            mComponents.erase(pIter);
        }

        [[nodiscard]] T * getComponent(Entity entity) {
            return &(getComponentIterator(entity)->second);
        }

    private:

        [[nodiscard]] bool checkIfComponentExists(Entity entity) {
            return mComponents.find(entity) != mComponents.end();
        }

        template<typename TError = ComponentDoesntExist>
        [[nodiscard]] typename std::unordered_map<Entity, T>::iterator getComponentIterator(Entity entity) {
            auto pIter = mComponents.find(entity);

            if (pIter == mComponents.end())
                throw TError();

            return pIter;
        }

        std::unordered_map<Entity, T> mComponents {};
    };


    class ComponentNotRecognised : public std::exception
    {
        [[nodiscard]] const char * what() const noexcept override;
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
        [[nodiscard]] ComponentId getComponentId() {
            static ComponentId id = mUniqueComponentCount++;
            return id;
        }

        template<typename T>
        void addComponent(Entity entity) {
            getComponentList<T>()->addComponent(entity);
        }

        template<typename T>
        void addComponent(Entity entity, T component) {
            getComponentList<T>()->addComponent(entity, component);
        }

        template<typename T>
        void removeComponent(Entity entity) {
            getComponentList<T>()->deleteComponent(entity);
        }

        template<typename T>
        [[nodiscard]] T * getComponent(Entity entity) {
            return getComponentList<T>()->getComponent(entity);
        }

        void entityDeleted(Entity entity);

    private:

        template<typename T, typename TError = ComponentNotRecognised>
        [[nodiscard]] std::shared_ptr<ComponentList<T>> getComponentList() {
            const char *typeName = typeid(T).name();

            auto pIter = mComponentLists.find(typeName);

            if (pIter == mComponentLists.end())
                throw TError();

            return std::static_pointer_cast<ComponentList<T>>(mComponentLists[typeName]);
        }

        std::unordered_map<const char*, std::shared_ptr<BaseComponentList>> mComponentLists {};

        std::size_t mUniqueComponentCount = 0;
    };

    class ECSPointerIsNULL : public std::exception
    {
        [[nodiscard]] const char * what() const noexcept override;
    };

    class System
    {
    protected:

        [[nodiscard]] const std::vector<Entity> &getEntities() const;

        [[nodiscard]] SECS * getECS() {
            if (!mpECS)
                throw ECSPointerIsNULL();
            return mpECS;
        }

        virtual void onEntityAdded(secs::Entity entity) {};
        virtual void onEntityRemoved(secs::Entity entity) {};

    private:

        std::vector<Entity> mEntities {};

        std::unordered_map<Entity, std::size_t> mEntityToIndex {};

        SECS *mpECS = nullptr;

        friend class SystemManager;
        friend class SECS;
    };

    class SystemNotActivated : public std::exception
    {
    public:

        [[nodiscard]] const char* what() const noexcept override;
    };

    class SystemAlreadyActivated : public std::exception
    {
    public:

        [[nodiscard]] const char* what() const noexcept override;
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
        void deactivateSystem() {
            auto pIter = getSystemPair<T>();

            pIter->second->mEntities = {};
            pIter->second->mpECS = nullptr;

            mNameToSystem.erase(pIter);
            mSystemDependencies.erase(typeid(T).name());
        }

        template<typename T>
        [[nodiscard]] Archetype getSystemDependencies() {
            return getSystemDependenciesPair<T>()->second;
        }

        [[nodiscard]] Archetype getSystemDependencies(const char *systemName);

        template<typename T>
        void setSystemDependencies(const Archetype &dependencies) {
            getSystemDependenciesPair<T>()->second = dependencies;
        }

        void updateSystemsFor(Entity entity, const Archetype &entityArchetype);

        template<typename T>
        void addEntityToSystem(Entity entity) {
            System *pSystem = getSystemPair<T>()->second;

            if (pSystem->mEntityToIndex.find(entity) != pSystem->mEntityToIndex.end())
                return;

            pSystem->mEntityToIndex.insert({entity, pSystem->mEntities.size()});
            pSystem->mEntities.push_back(entity);
            pSystem->onEntityAdded(entity);
        }

        void addEntityToSystem(Entity entity, const char *systemName) {
            System *pSystem = getSystemPair(systemName)->second;

            if (pSystem->mEntityToIndex.find(entity) != pSystem->mEntityToIndex.end())
                return;

            pSystem->mEntityToIndex.insert({entity, pSystem->mEntities.size()});
            pSystem->mEntities.push_back(entity);
            pSystem->onEntityAdded(entity);
        }

        template<typename T>
        void removeEntityFromSystem(Entity entity) {
            System* pSystem = getSystemPair<T>()->second;

            auto pETIIter = pSystem->mEntityToIndex.find(entity);

            if (pETIIter == pSystem->mEntityToIndex.end())
                return;

            std::size_t index = pETIIter->second;

            pSystem->mEntities.erase(pSystem->mEntities.begin() + index);
            pSystem->mEntityToIndex.erase(entity);

            updateSystemEntitiesMapping(*pSystem, index);

            pSystem->onEntityRemoved(entity);
        }

        void removeEntityFromSystem(Entity entity, const char *systemName) {
            System* pSystem = getSystemPair(systemName)->second;

            auto pETIIter = pSystem->mEntityToIndex.find(entity);

            if (pETIIter == pSystem->mEntityToIndex.end())
                return;

            std::size_t index = pETIIter->second;

            pSystem->mEntities.erase(pSystem->mEntities.begin() + index);
            pSystem->mEntityToIndex.erase(entity);

            updateSystemEntitiesMapping(*pSystem, index);

            pSystem->onEntityRemoved(entity);
        }

        void entityDeleted(Entity entity);
        void sECSDeleted();

    private:

        template<typename T>
        [[nodiscard]] std::unordered_map<const char *, System *>::iterator getSystemPair() {
            const char *typeName = typeid(T).name();

            auto pIter = mNameToSystem.find(typeName);

            if (pIter == mNameToSystem.end())
                throw SystemNotActivated();

            return pIter;
        }

        [[nodiscard]] std::unordered_map<const char *, System *>::iterator getSystemPair(const char *systemName) {

            auto pIter = mNameToSystem.find(systemName);

            if (pIter == mNameToSystem.end())
                throw SystemNotActivated();

            return pIter;
        }

        template<typename T>
        [[nodiscard]] std::unordered_map<const char *, Archetype>::iterator getSystemDependenciesPair() {
            const char *typeName = typeid(T).name();

            auto pIter = mSystemDependencies.find(typeName);

            if (pIter == mSystemDependencies.end())
                throw SystemNotActivated();

            return pIter;
        }

        [[nodiscard]] std::unordered_map<const char *, Archetype>::iterator getSystemDependenciesPair(const char *systemName) {
            auto pIter = mSystemDependencies.find(systemName);

            if (pIter == mSystemDependencies.end())
                throw SystemNotActivated();

            return pIter;
        }

        static void updateSystemEntitiesMapping(System &system, std::size_t deletedIndex) {
            for (auto &pair : system.mEntityToIndex) {
                if (pair.second > deletedIndex) {
                    pair.second--;
                }
            }
        }

        std::unordered_map<const char *, System *> mNameToSystem {};
        std::unordered_map<const char *, Archetype> mSystemDependencies {};
    };

    class SECS
    {
    public:

        ~SECS();

        Entity createEntity();
        void deleteEntity(Entity entity);

        [[nodiscard]] const std::vector<Entity> getEntities(const Archetype &filter = Archetype()) const;

        template<typename T>
        [[nodiscard]] ComponentId getComponentId() {
            return mComponentManager.getComponentId<T>();
        }

        template<typename T>
        void registerComponent() {
            mComponentManager.registerComponent<T>();
        }

        template<typename T>
        [[nodiscard]] T * getComponent(Entity entity) {
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
        void removeComponent(Entity entity) {
            Archetype archetype = mEntityManager.getEntityArchetype(entity);

            archetype.setComponent(this->getComponentId<T>(), false);

            mEntityManager.setEntityArchetype(entity, archetype);

            mSystemManager.updateSystemsFor(entity, archetype);

            mComponentManager.removeComponent<T>(entity);
        }

        template<typename T>
        [[nodiscard]] bool checkIfHasComponent(Entity entity) {
            return mEntityManager.getEntityArchetype(entity).isComponentSet(getComponentId<T>());
        }

        template<typename T>
        void setArchetypeComponent(secs::Archetype &archetype, bool value = true) {
            archetype.setComponent(mComponentManager.getComponentId<T>(), value);
        }

        template<typename T>
        [[nodiscard]] bool isArchetypeComponentSet(secs::Archetype &archetype) {
            return archetype.isComponentSet(mComponentManager.getComponentId<T>());
        }

        template<typename T>
        void activateSystem(System *system) {
            system->mpECS = this;

            mSystemManager.activateSystem<T>(system);

            for (const auto &iter: mEntityManager.getEntityArchetypes())
                mSystemManager.addEntityToSystem<T>(iter.first);
        }

        template<typename T>
        void deactivateSystem() {
            mSystemManager.deactivateSystem<T>();
        }

        template<typename T>
        [[nodiscard]] Archetype getSystemDependencies() {
            return mSystemManager.getSystemDependencies<T>();
        }

        template<typename T>
        void setSystemDependencies(Archetype dependencies) {
            mSystemManager.setSystemDependencies<T>(dependencies);

            for (auto &iter : mEntityManager.getEntityArchetypes()) {
                if (checkIfSubArchetype(dependencies, iter.second)) {
                    mSystemManager.addEntityToSystem<T>(iter.first);

                    continue;
                }

                mSystemManager.removeEntityFromSystem<T>(iter.first);
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
