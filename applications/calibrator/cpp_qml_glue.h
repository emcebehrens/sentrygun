#ifndef CPP_QML_GLUE_H
#define CPP_QML_GLUE_H

#include <QObject>
#include "trackingthread.h"
#include "GenTasksThread.h"
#include "PanTiltManagerThread.h"
#include "geometry.h"
#include "settings_manager.h"

class CppQmlGlue : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QStringList comboBoxCameraListModel
               READ getComboBoxCameraListModel
               NOTIFY sig_comboBoxCameraListModelChanged)

    Q_PROPERTY(qreal tiltSliderValue
               READ getTiltSliderValue
               WRITE setTiltSliderValue
               NOTIFY sig_tiltSliderValueChanged)

    Q_PROPERTY(qreal panSliderValue
               READ getPanSliderValue
               WRITE setPanSliderValue
               NOTIFY sig_panSliderValueChanged)

    Q_PROPERTY(QStringList comboBoxSettingsNamesListModel
               READ getComboBoxSettingsNamesListModel
               NOTIFY sig_comboBoxSettingsNamesListModel)

    Q_PROPERTY(qreal maxServoTilt
               READ getMaxServoTilt
               WRITE setMaxServoTilt
               NOTIFY sig_maxServoTiltChanged)

    Q_PROPERTY(qreal minServoTilt
               READ getMinServoTilt
               WRITE setMinServoTilt
               NOTIFY sig_minServoTiltChanged)

    Q_PROPERTY(qreal maxServoPan
               READ getMaxServoPan
               WRITE setMaxServoPan
               NOTIFY sig_maxServoPanChanged)

    Q_PROPERTY(qreal minServoPan
               READ getMinServoPan
               WRITE setMinServoPan
               NOTIFY sig_minServoPanChanged)

public:
    CppQmlGlue();
    ~CppQmlGlue();

    // Q Properties' READ functions
    QStringList getComboBoxCameraListModel();
    qreal getTiltSliderValue();
    qreal getPanSliderValue();
    QStringList getComboBoxSettingsNamesListModel();
    qreal getMaxServoTilt();
    qreal getMinServoTilt();
    qreal getMaxServoPan();
    qreal getMinServoPan();

    // Q Properties' WRITE functions
    void setTiltSliderValue(qreal value);
    void setPanSliderValue(qreal value);
    void setMaxServoTilt(qreal value);
    void setMinServoTilt(qreal value);
    void setMaxServoPan(qreal value);
    void setMinServoPan(qreal value);

    Q_INVOKABLE void cameraEnable(bool yesno, int indx);
    Q_INVOKABLE void trackingEnabled(bool activate);
    Q_INVOKABLE void setTiltTarget(int target);
    Q_INVOKABLE void setPanTarget(int target);
    Q_INVOKABLE void getPanTiltPositions();
    Q_INVOKABLE int calculateStepsizeY(int tiltmax,
                                       int tiltmin,
                                       qreal calYDistance,
                                       qreal calDepth);
    Q_INVOKABLE int calculateStepsizeX(int panmax,
                                       int panmin,
                                       qreal calXDistance,
                                       qreal calDepth);
    Q_INVOKABLE qreal getCaldistanceX();
    Q_INVOKABLE qreal getCaldistanceY();
    Q_INVOKABLE qreal getCaldistanceZ();
    Q_INVOKABLE qreal getDepthDistance();
    Q_INVOKABLE void setDepth(qreal value);

public slots:
    void slot_changeAngle(int angle);
    void shutdown();    

signals:
    qreal sig_tiltSliderValueChanged();
    qreal sig_panSliderValueChanged();
    void sig_comboBoxCameraListModelChanged();
    void sig_comboBoxSettingsNamesListModel();
    void sig_maxServoTiltChanged();
    void sig_minServoTiltChanged();
    void sig_maxServoPanChanged();
    void sig_minServoPanChanged();

    void sig_updateAngle(int);
    void sig_servoControllerState(int);
    void sig_havePanPosition(int);
    void sig_haveTiltPosition(int);

private slots:
    void slot_foundPort(QString port, QString desc);
    void slot_portDisapeared(QString port);
    void slot_panTiltManOpenDone(bool success);
    void slot_panTiltManTerminated();
    void slot_newBlobPos(int x, int y);
    void slot_newPanPosistion(int pos);
    void slot_newTiltPosistion(int pos);
private:
    qreal tiltSliderValue_;
    qreal panSliderValue_;

    qreal maxServoTilt_;

    //bool haveAPort_;
    TrackingThread  *trackingThreadPtr_;
    GenTasksThread  *genTasksThreadPtr_;
    PanTiltManagerThread *panTiltManThreadPtr_;
    Geometry *geom_;

    QStringList comboBoxCameraListModel_;
    QStringList comboBoxSettingsNamesListModel_;

    SettingsManager settings_;
    QList<SettingsManager::settingsEntry> *settListPtr_;

    void startPanTiltManagerThread();
    void getCameraList();
};

#endif // CPP_QML_GLUE_H
