#include "shortcutmanager.h"
#include <QtXml/QDomElement>
#include <QtXml/QDomDocument>
#include <QFile>
#include "commandregistry.h"
#include <QDebug>
// #include "qxtglobalshortcut.h"

ShortcutManager::ShortcutManager():mShortcuts(),mGlobalShortcuts()
{
}

void ShortcutManager::globalShortcutActived(QxtGlobalShortcut *shortcut)
{
    if(mGlobalShortcuts.contains(shortcut))
        mGlobalShortcuts[shortcut]->execute(mViewer);
}


void ShortcutManager::loadFromXmlFile(QString fileName)
{
    QDomDocument doc("shortcut");
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug()<<"error load shortcut file";
        qDebug()<<file.fileName();
        return;
    }
    if (!doc.setContent(&file)) {
        file.close();
        return;
    }
    file.close();

    QDomElement root = doc.documentElement();
    QDomElement child = root.firstChildElement();

    while(!child.isNull())
    {
        QString command = child.attribute("command");
        QDomElement key=child.firstChildElement();
        while(!key.isNull())
        {
            if(!key.text().isEmpty())
            {
                // if(key.hasAttribute("global"))
                // {
                //     QString globalShortcut=key.text();
                //     QxtGlobalShortcut *shortCut=new QxtGlobalShortcut();
                //     QKeySequence seq(globalShortcut);
                //     bool ok=shortCut->setShortcut(seq);
                //     if(ok)
                //     {
                //         qDebug()<<"register success:"<<seq;
                //         mGlobalShortcuts[shortCut]=CommandRegistry::get("Viewer"+command+"Command");
                //         QObject::connect(shortCut,SIGNAL(activated(QxtGlobalShortcut*)),this,SLOT(globalShortcutActived(QxtGlobalShortcut*)));
                //     }
                //     else
                //     {
                //         qDebug()<<"register failed:"<<seq;
                //        emit registerGlobalShortcutFailed(seq);
                //     }

                // }
                // else
                {
                    mShortcuts[key.text().toUpper()]=CommandRegistry::get("Viewer"+command+"Command");
                }
            }

            key=key.nextSiblingElement();
        }
        child = child.nextSiblingElement();
    }
}


ViewerCommand *ShortcutManager::getCommand(QString shortcut)
{
    shortcut=shortcut.toUpper();
    if(mShortcuts.contains(shortcut))
        return mShortcuts[shortcut];
    else
        return CommandRegistry::get("");
}
