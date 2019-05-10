#include "pluginmanager.h"
#include "pluginsmanagerprivate.h"
#include <QDir>
#include <QCoreApplication>
#include <QJsonArray>
#include <QDebug>
PluginManager* PluginManager::m_instance=nullptr;

PluginManager::PluginManager()
{
    managerPrivate = new PluginsManagerPrivate();
}

PluginManager::~PluginManager()
{
    if(managerPrivate)
        delete managerPrivate;
}

void PluginManager::loadAllPlugins()
{
    QDir pluginsdir = QDir(qApp->applicationDirPath());
    pluginsdir.cd("plugins");

    QFileInfoList pluginsInfo = pluginsdir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot);
    //初始化插件中的元数据
    for(QFileInfo fileinfo : pluginsInfo){
        qDebug()<<"loadAllPlugins:"<<fileinfo.absoluteFilePath();
        scanMetaData(fileinfo.absoluteFilePath());
    }

    //加载插件
    for(QFileInfo fileinfo : pluginsInfo)
        loadPlugin(fileinfo.absoluteFilePath());
}

void PluginManager::scanMetaData(const QString &filepath)
{
    //判断是否为库（后缀有效性）
    if(!QLibrary::isLibrary(filepath))
        return ;

    qDebug()<<"scanMetaData:"<<filepath;
    //获取元数据
    QPluginLoader *loader = new QPluginLoader(filepath);

    qDebug()<<loader->metaData().keys();
    QJsonObject json = loader->metaData().value("MetaData").toObject();

    QVariant var = json.value("name").toVariant();
    managerPrivate->m_names.insert(filepath, json.value("name").toVariant());
    managerPrivate->m_versions.insert(filepath, json.value("version").toVariant());
    managerPrivate->m_dependencies.insert(filepath, json.value("dependencies").toArray().toVariantList());

    delete loader;
    loader = nullptr;
}

void PluginManager::loadPlugin(const QString &filepath)
{
    if(!QLibrary::isLibrary(filepath))
        return;

    //检测依赖
    if(!managerPrivate->check(filepath))
        return;

    //加载插件
    QPluginLoader *loader = new QPluginLoader(filepath);
    if(loader->load())
    {
        PluginInterface *plugin = qobject_cast<PluginInterface *>(loader->instance());
        if(plugin)
        {
            managerPrivate->m_loaders.insert(filepath, loader);
            //plugin->connect_information(this, SLOT(onPluginInformation(QString&)), true);
        }
        else
        {
            delete loader;
            loader = nullptr;
        }
    }else{
        qDebug()<<"loadPlugin:"<<filepath<<loader->errorString();
    }
}

void PluginManager::unloadAllPlugins()
{
    for(QString filepath : managerPrivate->m_loaders.keys())
        unloadPlugin(filepath);
}

void PluginManager::unloadPlugin(const QString &filepath)
{
    QPluginLoader *loader = managerPrivate->m_loaders.value(filepath);
    //卸载插件，并从内部数据结构中移除
    if(loader->unload())
    {
        managerPrivate->m_loaders.remove(filepath);
        delete loader;
        loader = nullptr;
    }
}

QList<QPluginLoader *> PluginManager::allPlugins()
{
    return managerPrivate->m_loaders.values();
}

QList<QVariant> PluginManager::allPluginsName()
{
    return managerPrivate->m_names.values();
}

void PluginManager::log(QString str)
{
    QPluginLoader *loader = getPlugin("logPlugin");
    if(loader)
    {
        LogInterface *log = dynamic_cast<LogInterface*>(loader->instance());
        if(log)
            log->log(str);
    }
}

void PluginManager::insert(QString str)
{
    QPluginLoader *loader = getPlugin("databasePlugin");
    if(loader)
    {
        DatabaseInterface *database = dynamic_cast<DatabaseInterface*>(loader->instance());
        if(database)
            database->insert(str);
    }
}

QVariant PluginManager::getPluginName(QPluginLoader *loader)
{
    if(loader)
        return managerPrivate->m_names.value(managerPrivate->m_loaders.key(loader));
    else
        return "";
}

QPluginLoader *PluginManager::getPlugin(const QString &name)
{
    return managerPrivate->m_loaders.value(managerPrivate->m_names.key(name));
}
