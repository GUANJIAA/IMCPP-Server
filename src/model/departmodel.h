#ifndef __DEPARTMODEL_H__
#define __DEPARTMODEL_H__

#include "depart.h"

#include <vector>
#include <string>

class DepartModel
{
public:
    bool createDepart(Depart &depart);
    bool addDepart(std::string departName, std::string userName, std::string userRole);
    bool delDepart(std::string departName,std::string userName);
    bool quitDepart(std::string userName);
    Depart queryDepart(std::string userName);
    std::vector<std::string> queryDepartUsers(std::string departName, std::string userName);
};

#endif