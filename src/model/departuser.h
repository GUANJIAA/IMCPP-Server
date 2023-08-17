#ifndef __DEPARTUSER_H__
#define __DEPARTUSER_H__

#include "admin.h"

class DepartUser : public Admin
{
public:
    void setRole(std::string role) { m_role = role; }
    std::string getRole() { return m_role; }

private:
    std::string m_role;
};

#endif