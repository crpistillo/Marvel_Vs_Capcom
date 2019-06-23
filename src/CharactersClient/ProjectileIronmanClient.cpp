//
// Created by IgVelasco on 6/22/19.
//

#include "ProjectileIronmanClient.h"

ProjectileIronmanClient::ProjectileIronmanClient(bool secondaryColor) : ProjectileClient(secondaryColor) {
    mvcFilePath = "/MVC2_IronMan_";
    projectileFilePath = "images/iron_man/iron_man_projectile_";
}
