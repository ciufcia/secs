#include "secs.hpp"

#include <unordered_map>
#include <vector>

void secs::Archetype::setComponent(secs::ComponentId id, bool value) {
    if (id >= mComponents.size())
        mComponents.resize(id + 1, false);

    mComponents[id] = value;
}

bool secs::Archetype::isComponentSet(secs::ComponentId id) {
    if (id >= mComponents.size())
        return false;

    return mComponents[id];
}

bool secs::checkIfSubArchetype(const secs::Archetype &superArchetype, const secs::Archetype &subArchetype) {
    if (superArchetype == Archetype())
        return true;

    for (secs::ComponentId id = 0u; id < superArchetype.mComponents.size(); id++) {
        bool value = superArchetype.mComponents[id];

        if (value)
            if (id >= subArchetype.mComponents.size() || !(subArchetype.mComponents[id]))
                return false;
    }

    return true;
}

bool secs::operator==(const Archetype& a1, const Archetype& a2) {
    for (secs::ComponentId id = 0u; id < a1.mComponents.size(); id++) {
        if (id == a2.mComponents.size()) {
            for (secs::ComponentId i = id; i < a1.mComponents.size(); i++) {
                if (a1.mComponents[i])
                    return false;
            }

            return true;
        } else if (a1.mComponents[id] != a2.mComponents[id]) {
            return false;
        }
    }

    for (secs::ComponentId id = a1.mComponents.size(); id < a2.mComponents.size(); id++) {
        if (a2.mComponents[id])
            return false;
    }

    return true;
}

bool secs::operator!=(const Archetype& a1, const Archetype& a2) {
    for (secs::ComponentId id = 0u; id < a1.mComponents.size(); id++) {
        if (id == a2.mComponents.size()) {
            for (secs::ComponentId i = id; i < a1.mComponents.size(); i++) {
                if (a1.mComponents[i])
                    return true;
            }

            return false;
        } else if (a1.mComponents[id] != a2.mComponents[id]) {
            return true;
        }
    }

    for (secs::ComponentId id = a1.mComponents.size(); id < a2.mComponents.size(); id++) {
        if (a2.mComponents[id])
            return true;
    }

    return false;
}

const char * secs::EntityDoesntExist::what() const noexcept {
    return "Entity doesn't exist";
}

secs::Entity secs::EntityManager::createEntity() {
    secs::Entity entity;

    if (deletedEntities.empty())
        entity = generateNewId();
    else {
        entity = deletedEntities[0];
        deletedEntities.erase(deletedEntities.begin());
    }

    mEntityArchetypes.insert({entity, Archetype()});

    return entity;
}

void secs::EntityManager::deleteEntity(secs::Entity entity) {
    auto pIter = mEntityArchetypes.find(entity);

    if (pIter == mEntityArchetypes.end())
        return;

    mEntityArchetypes.erase(pIter);
    deletedEntities.push_back(entity);
}

secs::Archetype secs::EntityManager::getEntityArchetype(secs::Entity entity) const {
    auto pIter = mEntityArchetypes.find(entity);

    if (pIter == mEntityArchetypes.end())
        throw EntityDoesntExist();

    return pIter->second;
}

void secs::EntityManager::setEntityArchetype(secs::Entity entity, const secs::Archetype &archetype) {
    auto pIter = mEntityArchetypes.find(entity);

    if (pIter == mEntityArchetypes.end())
        throw EntityDoesntExist();

    pIter->second = archetype;
}

const std::unordered_map<secs::Entity, secs::Archetype> & secs::EntityManager::getEntityArchetypes() const noexcept {
    return mEntityArchetypes;
}

const std::vector<secs::Entity> secs::EntityManager::getEntities(const secs::Archetype &filter) const {
    std::vector<secs::Entity> entities {};
    for (auto &pair : this->mEntityArchetypes)
        if (checkIfSubArchetype(filter, pair.second))
            entities.push_back(pair.first);
    return entities;
}

secs::Entity secs::EntityManager::generateNewId() {
    return mCreatedEntityCount++;
}

const char * secs::ComponentDoesntExist::what() const noexcept {
    return "Component doesn't exist";
}

const char * secs::DuplicateComponent::what() const noexcept {
    return "Duplicate component was detected";
}

const char * secs::ComponentNotRecognised::what() const noexcept {
    return "Component isn't recognised";
}

void secs::ComponentManager::entityDeleted(secs::Entity entity) {
    for (auto &iter : mComponentLists)
        iter.second->entityDeleted(entity);
}

const std::vector<secs::Entity> &secs::System::getEntities() const {
    return mEntities;
}

const char * secs::ECSPointerIsNULL::what() const noexcept {
    return "The pointer to the ECS is NULL";
}

const char * secs::SystemNotActivated::what() const noexcept {
    return "System isn't activated";
}

const char * secs::SystemAlreadyActivated::what() const noexcept {
    return "System has already been activated";
}

secs::Archetype secs::SystemManager::getSystemDependencies(const char *systemName) {
    auto pIter = getSystemDependenciesPair(systemName);

    return pIter->second;
}

void secs::SystemManager::updateSystemsFor(secs::Entity entity, const secs::Archetype &entityArchetype) {
    for (auto &iter : mNameToSystem) {
        if (checkIfSubArchetype(this->getSystemDependencies(iter.first), entityArchetype))
            addEntityToSystem(entity, iter.first);
        else
            removeEntityFromSystem(entity, iter.first);
    }
}

void secs::SystemManager::entityDeleted(secs::Entity entity) {
    for (auto &iter : mNameToSystem)
        removeEntityFromSystem(entity, iter.first);
}

void secs::SystemManager::sECSDeleted() {
    for (auto &iter : mNameToSystem) {
        iter.second->mpECS = nullptr;
        iter.second->mEntities = {};
        iter.second->mEntityToIndex = {};
    }
}

secs::SECS::~SECS() {
    mSystemManager.sECSDeleted();
}

secs::Entity secs::SECS::createEntity() {
    secs::Entity entity = mEntityManager.createEntity();

    mSystemManager.updateSystemsFor(entity, Archetype());

    return entity;
}

void secs::SECS::deleteEntity(secs::Entity entity) {
    mComponentManager.entityDeleted(entity);

    mSystemManager.entityDeleted(entity);

    mEntityManager.deleteEntity(entity);
}

const std::vector<secs::Entity> secs::SECS::getEntities(const secs::Archetype &filter) const {
    return mEntityManager.getEntities(filter);
}
