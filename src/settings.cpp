#include "settings.h"

SettingsManager::SettingsManager()
{
    QString data_dir = QStandardPaths::writableLocation(QStandardPaths::DataLocation);
    QSettings::setPath(QSettings::NativeFormat, QSettings::UserScope, data_dir);
    settings_ = new QSettings;
    settings_->setIniCodec("UTF-8");
}

SettingsManager::~SettingsManager()
{
    delete settings_;
    settings_ = 0;
}

bool SettingsManager::saveSettings(QList<QString> data)
{
    QVariantList datalist;
    foreach(QString setting, data){
        datalist << setting;
    }
    settings_->setValue("settings", datalist);
    settings_->sync();
    return true;
}

QList<QString> SettingsManager::loadSettings()
{
    QList<QString> list;
    return list;

}
