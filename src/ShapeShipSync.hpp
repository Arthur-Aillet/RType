/*
** Agartha-Software, 2023
** rtype
** File description:
** ShapeShipSync
*/

#pragma once

#include <string>

#include "cevy.hpp"
#include "Synchroniser.hpp"
#include "Schedule.hpp"
#include "network.hpp"

class ShapeShipSync : public cevy::Synchroniser {
    public:
        struct PositionSync {};
        ShapeShipSync(Synchroniser::Mode mode, const std::string& host);
        ~ShapeShipSync();

        void build(const cevy::ecs::App& app) override {
            add_sync<cevy::ecs::Schedule::PreUpdate, PositionSync, cevy::Position, Cevy::Rotation>(app);
        }

    protected:
    private:
};
