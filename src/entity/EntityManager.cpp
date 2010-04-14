#include "EntityManager.hpp"

void EntityManager::AddEntity(IEntity* entity) {
    mEntities.push_back(entity);
}

/*bool EntityManager::RemoveEntity(IEntity entity) {

}*/

boost::ptr_vector<IEntity>& EntityManager::GetEntities() {
    return mEntities;
}
