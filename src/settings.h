#ifndef SETTINGS_H
#define SETTINGS_H
#include <QCoreApplication>
#include <QStandardPaths>
#include <QDir>
#include <QSettings>
#include <QString>
#include <QList>
#include <QStringList>
#include <QVariantList>
#include <QDebug>

class SettingsManager
{
public:
    SettingsManager();
    ~SettingsManager();
    void saveSettings(QString area);
    QString loadSettings();

private:
    QSettings *settings_;
};

#endif // SETTINGS_H
