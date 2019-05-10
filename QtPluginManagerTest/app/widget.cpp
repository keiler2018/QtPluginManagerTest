#include "widget.h"
#include <QDebug>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    PluginManager::instance()->loadAllPlugins();//插件管理器 加载所有插件


    PluginManager::instance()->log(QStringLiteral("测试日志插件"));

    PluginManager::instance()->insert(QStringLiteral("测试数据库插件"));
}

Widget::~Widget()
{

}
