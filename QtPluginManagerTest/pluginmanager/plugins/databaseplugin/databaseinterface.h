#ifndef DATABASEINTERFACE_H
#define DATABASEINTERFACE_H

#include "../../plugininterface.h"

class DatabaseInterface:public PluginInterface
{
public:
    virtual ~DatabaseInterface() {}
    virtual void insert(QString)=0;
};

#endif // DATABASEINTERFACE_H
