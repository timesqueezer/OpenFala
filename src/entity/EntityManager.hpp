#ifndef ENTITYMANAGER_HPP_INCLUDED
#define ENTITYMANAGER_HPP_INCLUDED 

#include <boost/ptr_container/ptr_container.hpp>

#include "IEntity.hpp"

class EntityManager {
public:
    // Default constructor
    EntityManager() {}

    // Default destructor
    ~EntityManager() {}

    // Adds an entity to the internal entity list mEntities
    void AddEntity(IEntity entity);

    // Remove an entity from the internal entity list mEntities
    //bool RemoveEntity(IEntity entity);

    // Return internal entity list mEntities
    boost::ptr_vector<IEntity> GetEntities();

private:
    boost::ptr_vector<IEntity> mEntities;
};

#endif
