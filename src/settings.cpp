#include "settings.h"

SettingsManager::SettingsManager()
{
    qDebug() << QStandardPaths::writableLocation(QStandardPaths::DataLocation);
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

void SettingsManager::saveSettings(QString area)
{
    QVariant data;
    if(!area.isEmpty()) {
        data = QVariant::fromValue(area);
        settings_->setValue("area", data);
        settings_->sync();
    }
}

QString SettingsManager::loadSettings()
{
    return settings_->value("area").toString();
}
