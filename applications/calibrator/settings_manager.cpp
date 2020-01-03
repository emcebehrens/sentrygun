#include "settings_manager.h"
#include <QDebug>
#include <QDir>

SettingsManager::SettingsManager()
{

}

SettingsManager::~SettingsManager()
{
    delete settingsPtr_;
}

void SettingsManager::loadSettings(QString filename)
{
    qDebug() << "SettingsManager::loadSettings" << filename;

    settingsPtr_ = new QSettings(filename, QSettings::IniFormat);

    /*settingsPtr_->beginGroup("Paths");
    logFilePath_ = settingsPtr_->value("LogFilePath").toString();
    defaultReportsPath_ = settingsPtr_->value("ReportsPath").toString();
    defaultLogo_ = settingsPtr_->value("DefaultLogo").toString();
    exportFilePath_ = settingsPtr_->value("ExportFilePath").toString();
    settingsPtr_->endGroup();

    settingsPtr_->beginGroup("DevOptions");
    generateBinFiles_ = settingsPtr_->value("GenerateBin").toBool();
    settingsPtr_->endGroup();*/

    settingsPtr_->beginGroup("SettingsGroup");
    int size = settingsPtr_->beginReadArray("Settings");
    for (int i = 0; i < size; ++i) {
        settingsEntry def;
        settingsPtr_->setArrayIndex(i);
        def.Name.append(settingsPtr_->value("Name").toString());
        //def.stepsizex = settingsPtr_->value("stepsizex").toUInt();
        //def.stepsizey = settingsPtr_->value("stepsizey").toUInt();
        def.calservomaxx = settingsPtr_->value("calservomaxx").toUInt();
        def.calservominx = settingsPtr_->value("calservominx").toUInt();
        def.calservomaxy = settingsPtr_->value("calservomaxy").toUInt();
        def.calservominy = settingsPtr_->value("calservominy").toUInt();

        def.caldistancex = settingsPtr_->value("caldistancex").toDouble();
        def.caldistancey = settingsPtr_->value("caldistancey").toDouble();
        def.caldistancez = settingsPtr_->value("caldistancez").toDouble();
        def.depth = settingsPtr_->value("depth").toDouble();
        settingsList_.append(def);
    }
    settingsPtr_->endArray();
    settingsPtr_->endGroup();

    if (size == 0) {
        settingsEntry def;
        def.Name.append("Default");
        def.calservomaxx = 6890;
        def.calservominx = 5027;
        def.calservomaxy = 6000;
        def.calservominy = 4460;
        def.caldistancex = 625.00;
        def.caldistancey = 470.00;
        def.caldistancez = 684.00;
        def.depth = 684.00;

        settingsList_.append(def);
        saveSettings();
    }


}

QList<SettingsManager::settingsEntry> *SettingsManager::getSettingPtr()
{
    return &settingsList_;
}

void SettingsManager::saveSettings()
{
    settingsPtr_->beginGroup("SettingsGroup");
    settingsPtr_->remove("");

    settingsPtr_->beginWriteArray("Settings");
    for (int i = 0; i < settingsList_.size(); ++i) {
        settingsPtr_->setArrayIndex(i);
        settingsPtr_->setValue("Name", settingsList_.at(i).Name);
        //settingsPtr_->setValue("stepsizex", settingsList_.at(i).stepsizex);
        //settingsPtr_->setValue("stepsizey", settingsList_.at(i).stepsizey);
        settingsPtr_->setValue("calservomaxx", settingsList_.at(i).calservomaxx);
        settingsPtr_->setValue("calservominx", settingsList_.at(i).calservominx);
        settingsPtr_->setValue("calservomaxy", settingsList_.at(i).calservomaxy);
        settingsPtr_->setValue("calservominy", settingsList_.at(i).calservominy);

        settingsPtr_->setValue("caldistancex", settingsList_.at(i).caldistancex);
        settingsPtr_->setValue("caldistancey", settingsList_.at(i).caldistancey);
        settingsPtr_->setValue("caldistancez", settingsList_.at(i).caldistancez);
        settingsPtr_->setValue("depth", settingsList_.at(i).depth);
    }
    settingsPtr_->endArray();

    settingsPtr_->sync();
    settingsPtr_->endGroup();
}
