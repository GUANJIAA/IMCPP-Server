#ifndef __ADMINMODEL_H__
#define __ADMINMODEL_H__

#include "mysqldb.h"
#include "admin.h"

class AdminModel
{
public:
    bool insert(Admin &admin);
    Admin query(std::string name);
    bool update(Admin &adminn);
    bool updateState(Admin &admin);
    bool resetState();
};
#endif