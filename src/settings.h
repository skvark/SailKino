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

class SettingsManager : public QObject
{
    Q_OBJECT
public:
    SettingsManager(QObject *parent = 0);
    ~SettingsManager();

    enum Country {
        FI,
        EE,
        LV,
        LT
    };

    void saveArea(QString area);
    QString loadArea();

    void saveCountry(QString country);
    static QStringList getCountryList();
    Country country();

    void saveLanguage(QString lang, QString languageName);
    QString loadLanguage();

    void saveFilter(bool state);
    bool getFilterState();

    QString getCountryName();
    QString getLanguageName();

signals:
    void countryChanged();

private:
    static QList<QString> countryList_;
    QSettings *settings_;
};

#endif // SETTINGS_H
