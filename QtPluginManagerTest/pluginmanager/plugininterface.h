#ifndef PLUGININTERFACE_H
#define PLUGININTERFACE_H
#include <QStringList>
#include <QtWidgets/qwidget.h>

class PluginInterface
{
public:
    virtual ~PluginInterface(){}
};
#define PluginInterface_iid "PluginManager.PluginInterface"

Q_DECLARE_INTERFACE(PluginInterface,PluginInterface_iid)
#endif // PLUGININTERFACE_H
