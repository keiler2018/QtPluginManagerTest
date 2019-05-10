#ifndef QTPLUGINMANAGER_H
#define QTPLUGINMANAGER_H

#include "pluginmanager_global.h"
#include "plugininterface.h"
#include "./plugins/logplugin/loginterface.h"
#include "./plugins/databaseplugin/databaseinterface.h"
#include <QObject>
#include <QPluginLoader>
#include <QVariant>

class PluginsManagerPrivate ;
class QTPLUGINMANAGERSHARED_EXPORT PluginManager:public QObject
{
    Q_OBJECT
public:
    PluginManager();
    ~PluginManager();

    static PluginManager *instance(){
        if(m_instance==nullptr)
            m_instance=new PluginManager();
        return m_instance;
    }

    //加载所有插件
    void loadAllPlugins();

    //获取所有插件名称
    QList<QVariant> allPluginsName();

    /**日志插件**/
    void log(QString);//日志打印

    /**测试数据库插件**/
    void insert(QString);//插入数据
private:
    static PluginManager *m_instance;
    PluginsManagerPrivate *managerPrivate;

    //扫描JSON文件中的插件元数据
    void scanMetaData(const QString &filepath);

    //加载其中某个插件
    void loadPlugin(const QString &filepath);

    //卸载所有插件
    void unloadAllPlugins();

    //卸载某个插件
    void unloadPlugin(const QString &filepath);

    //获取所有插件
    QList<QPluginLoader *> allPlugins();

    //获取某个插件名称
    QVariant getPluginName(QPluginLoader *loader);

    //根据名称获得插件
    QPluginLoader* getPlugin(const QString &name);
};

#endif // QTPLUGINMANAGER_H
