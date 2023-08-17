#ifndef __FRIENDMODEL_H__
#define __FRIENDMODEL_H__

#include "admin.h"
#include "adminModel.h"

#include <vector>
#include <string>

class FriendModel
{
public:
    bool insert(std::string adminName, std::string peerName);
    std::vector<Admin> query(std::string adminName);
    bool removal(std::string adminName, std::string peerName);
};

#endif