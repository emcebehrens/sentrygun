#ifndef SETTINGS_MANAGER_H
#define SETTINGS_MANAGER_H

#include <QString>
#include <QSettings>

class SettingsManager
{
public:
    struct settingsEntry
        {
            QString Name;
            //quint32 stepsizex;
            //quint32 stepsizey;
            quint32 calservomaxx;
            quint32 calservominx;
            quint32 calservomaxy;
            quint32 calservominy;
            qreal   caldistancex;
            qreal   caldistancey;
            qreal   caldistancez;
            qreal   depth;
        };

    SettingsManager();
    ~SettingsManager();

    void loadSettings(QString filename);
    void saveSettings();
    QList<settingsEntry> *getSettingPtr();



private:
    QList<settingsEntry> settingsList_;
    QSettings *settingsPtr_;
};

#endif // SETTINGS_MANAGER_H
