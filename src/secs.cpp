#include "secs.hpp"

#include <unordered_map>

void secs::Archetype::setComponent(secs::ComponentId id, bool value) {
    if (id >= mComponents.size()) {
        mComponents.resize(id + 1, false);
    }
    mComponents[id] = value;
}

bool secs::Archetype::isComponentSet(secs::ComponentId id) {
    if (id >= mComponents.size())
        return false;
    return mComponents[id];
}

bool secs::checkIfSubarchetype(const secs::Archetype &superArchetype, const secs::Archetype &subArchetype) {
    for (secs::ComponentId id = 0u; id < superArchetype.mComponents.size(); id++) {
        bool value = superArchetype.mComponents[id];
        if (value) {
            if (id >= subArchetype.mComponents.size() || !(subArchetype.mComponents[id])) {
                return false;
            }
        }
    }
    return true;
}

bool secs::operator==(const Archetype& a1, const Archetype& a2) {
    // this can be way faster
    if (checkIfSubarchetype(a1, a2) && checkIfSubarchetype(a2, a1))
        return true;
    return false;
}

bool secs::operator!=(const Archetype& a1, const Archetype& a2) {
    // this can be way faster
    if (checkIfSubarchetype(a1, a2) && checkIfSubarchetype(a2, a1))
        return false;
    return true;
}

[[nodiscard]] const char * secs::EntityDoesntExist::what() const noexcept {
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

bool secs::EntityManager::deleteEntity(secs::Entity entity) {
    auto pIter = mEntityArchetypes.find(entity);
    if (pIter == mEntityArchetypes.end())
        return false;
    mEntityArchetypes.erase(pIter);
    deletedEntities.push_back(entity);
    return true;
}

secs::Archetype secs::EntityManager::getEntityArchetype(secs::Entity entity) const {
    auto pIter = mEntityArchetypes.find(entity);
    if (pIter == mEntityArchetypes.end()) {
        throw EntityDoesntExist();
    }
    return pIter->second;
}

void secs::EntityManager::setEntityArchetype(secs::Entity entity, secs::Archetype archetype) {
    auto pIter = mEntityArchetypes.find(entity);
    if (pIter == mEntityArchetypes.end()) {
        throw EntityDoesntExist();
    }
    pIter->second = archetype;
}

const std::unordered_map<secs::Entity, secs::Archetype> & secs::EntityManager::getEntityArchetypes() const noexcept {
    return mEntityArchetypes;
}

secs::Entity secs::EntityManager::generateNewId() {
    return mCreatedEntityCount++;
}

[[nodiscard]] const char * secs::ComponentDoesntExist::what() const noexcept {
    return "Component doesn't exist";
}

[[nodiscard]] const char * secs::DuplicateComponent::what() const noexcept {
    return "Duplicate component was detected";
}

[[nodiscard]] const char * secs::ComponentNotRecognised::what() const noexcept {
    return "Component isn't recognised";
}

void secs::ComponentManager::entityDeleted(secs::Entity entity) {
    for (auto pIter = mComponentLists.begin(); pIter != mComponentLists.end(); pIter++) {
        pIter->second->entityDeleted(entity);
    }
}

[[nodiscard]] const char * secs::ECSPointerIsNULL::what() const noexcept {
    return "The pointer to the ECS is NULL";
}

[[nodiscard]] const char * secs::SystemNotActivated::what() const noexcept {
    return "System isn't activated";
}

[[nodiscard]] const char * secs::SystemAlreadyActivated::what() const noexcept {
    return "System has already been activated";
}

secs::Archetype secs::SystemManager::getSystemDependencies(const char *systemName) const {
    auto pIter = mSystemDependencies.find(systemName);
    if (pIter == mSystemDependencies.end())
        throw SystemNotActivated();
    return pIter->second;
}

void secs::SystemManager::updateSystemsFor(secs::Entity entity, secs::Archetype entityArchetype) {
    for (auto pIter = mNameToSystem.begin(); pIter != mNameToSystem.end(); pIter++) {
        secs::System *pSystem = pIter->second;
        if (checkIfSubarchetype(this->getSystemDependencies(pIter->first), entityArchetype)) {
            if (pSystem->mEntityToIndex.find(entity) == pSystem->mEntityToIndex.end()) {
                pSystem->mEntityToIndex.insert({entity, pSystem->mEntities.size()});
                pSystem->mEntities.push_back(entity);
            }
        } else {
            auto pETIIter = pSystem->mEntityToIndex.find(entity);
            if (pETIIter != pSystem->mEntityToIndex.end()) {
                pSystem->mEntities.erase(pSystem->mEntities.begin() + pETIIter->second);
                pSystem->mEntityToIndex.erase(pETIIter);
            }
        }
    }
}

void secs::SystemManager::entityDeleted(secs::Entity entity) {
    for (auto pIter = mNameToSystem.begin(); pIter != mNameToSystem.end(); pIter++) {
        secs::System *pSystem = pIter->second;
        auto pEIter = pSystem->mEntityToIndex.find(entity);
        if (pEIter == pSystem->mEntityToIndex.end())
            continue;
        pSystem->mEntities.erase(pSystem->mEntities.begin() + pEIter->second);
        pSystem->mEntityToIndex.erase(pEIter);
    }
}

void secs::SystemManager::sECSDeleted() {
    for (auto pIter = mNameToSystem.begin(); pIter != mNameToSystem.end(); pIter++) {
        pIter->second->mpECS = nullptr;
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

bool secs::SECS::deleteEntity(secs::Entity entity) {
    mComponentManager.entityDeleted(entity);
    mSystemManager.entityDeleted(entity);
    return mEntityManager.deleteEntity(entity);
}
