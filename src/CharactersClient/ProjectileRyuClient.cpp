//
// Created by IgVelasco on 6/22/19.
//

#include "ProjectileRyuClient.h"

ProjectileRyuClient::ProjectileRyuClient(bool secondaryColor) : ProjectileClient(secondaryColor) {
    mvcFilePath = "/MVC2_Ryu_";
    projectileFilePath = "images/ryu/ryu_projectile_";
}
