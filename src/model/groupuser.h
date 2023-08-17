#ifndef __GROUPUSER_H__
#define __GROUPUSER_H__

#include "admin.h"

class GroupUser : public Admin
{
public:
    void setRole(std::string role) { m_role = role; }
    std::string getRole() { return m_role; }

private:
    std::string m_role;
};

#endif