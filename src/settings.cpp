#include "settings.h"

QList<QString> SettingsManager::countryList_ = QList<QString>()
        << QString("Finland")
        << QString("Estonia")
        << QString("Latvia")
        << QString("Lithuania");

SettingsManager::SettingsManager(QObject *parent):
    QObject(parent)
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

void SettingsManager::saveArea(QString area)
{
    QVariant data;

    if(!area.isEmpty()) {
        data = QVariant::fromValue(area);
        settings_->setValue("area", data);
        settings_->sync();
    }
}

void SettingsManager::saveCountry(QString country)
{
    QVariant data;
    int index = countryList_.indexOf(country);

    if(!country.isEmpty()) {
        data = QVariant::fromValue(index);
        settings_->setValue("country", data);
        settings_->sync();
        emit countryChanged();
    }
}

QString SettingsManager::getCountryName()
{
    if(settings_->value("country").toInt() >= 0) {
        return countryList_[settings_->value("country").toInt()];
    } else {
        return QString("");
    }
}

QString SettingsManager::getLanguageName()
{
    return settings_->value("languagename").toString();
}

void SettingsManager::saveLanguage(QString lang, QString languageName)
{
    QVariant data;

    if(!lang.isEmpty()) {
        data = QVariant::fromValue(lang);
        settings_->setValue("language", data);
        data = QVariant::fromValue(languageName);
        settings_->setValue("languagename", data);
        settings_->sync();
    }
}

QString SettingsManager::loadArea()
{
    return settings_->value("area").toString();
}

QString SettingsManager::loadLanguage()
{
    return settings_->value("language").toString();
}

void SettingsManager::saveFilter(bool state)
{
    QVariant data;
    data = QVariant::fromValue(state);
    settings_->setValue("filterstate", data);
    settings_->sync();
}

bool SettingsManager::getFilterState()
{
    return settings_->value("filterstate").toBool();
}

QStringList SettingsManager::getCountryList()
{
    return countryList_;
}

SettingsManager::Country SettingsManager::country()
{
    return static_cast<SettingsManager::Country>(settings_->value("country").toInt());
}
