#pragma once

#include <memory>
#include <string>
#include <vector>
#include <array>
#include <bitset>
#include <algorithm>

namespace TrimanaCore
{
    class Entity;
    class Component;

    using ComponentID = unsigned int;

    inline ComponentID GetUniqueComponentID()
    {
        static ComponentID last_id = 0;
        return last_id++;
    }

    template <typename T>
    inline ComponentID GetComponentID()
    {
        static_assert(std::is_base_of<Component, T>::value, "Type is not based on component");
        static const ComponentID type_id = GetUniqueComponentID();
        return type_id;
    }

    constexpr unsigned int QGL_MAX_ENTITIES = 5000;
    constexpr unsigned int QGL_MAX_COMPONENTS = 32;

    using ComponentBitset = std::bitset<QGL_MAX_ENTITIES>;
    using ComponentList = std::array<std::shared_ptr<Component>, QGL_MAX_COMPONENTS>;

    class Component
    {
    public:
        std::unique_ptr<Entity> EntityPtr;

    public:
        Component() = default;
        virtual ~Component() = default;

        virtual bool Init() { return true; }
        virtual void Update() {}
        virtual void Draw() {}
    };

    class Entity
    {
    public:
        Entity() = default;
        ~Entity() = default;

        template <class T, typename... TArgs>
        void InsertComponent(TArgs &&...args)
        {
            std::shared_ptr<T> component = std::make_shared<T>(std::forward<TArgs>(args)...);
            component->EntityPtr = std::make_unique<Enitiy>(this);
            mComponentList[GetComponentID<T>()] = component;
            mComponentBitSet[GetComponentID<T>()] = true;
        }

        template<class T>
        T& GetComponent() 
        {
            auto cptr(mComponentList[GetComponentID<T>()]);
            return *static_cast<T*>(cptr.get());
        }

        template<class T>
        bool HasComponent() const
        {
            return mComponentBitSet[GetComponentID<T>()];
        }

        void Update()
        {
            for(auto& component : mComponentList)
            {
                component->Update();
            }
        }

        void Draw()
        {
            for(auto& component : mComponentList)
            {
                component->Draw();
            }
        }

    private:
        ComponentBitset mComponentBitSet;
        ComponentList mComponentList;
    };

    class EntityManager
    {
        public:
            EntityManager() = default;
            ~EntityManager() = default;

            void Manage(std::unique_ptr<Entity>&& entity)
            {
                mEntities.emplace_back(std::move(entity));
            }

            void Update()
            {
                for(auto& entity : mEntities)
                {
                    entity->Update();
                }
            }

            void Draw()
            {
                for(auto& entity: mEntities)
                {
                    entity->Draw();
                }
            }

        private:
            std::vector<std::unique_ptr<Entity>> mEntities;
    };
}