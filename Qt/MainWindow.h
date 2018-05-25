#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QSlider>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QTimer>
#include <QProgressBar>
#include <QThread>

#include "CaseDataLoader.h"
#include "SystemDataCenter.h"
#include "GA_ThreadRunner.h"
#include "Canvas.h"
#include "CarObject.hpp"

using namespace std;

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void allCompoentInit();
    void addCompoentToContainer();
    void compoentEventInit();
    void setGridLayout();

    void dataInit();
    void allCompoentRefresh();

    void consoleAddMsg(QString strMsg);
    bool isCarReachEndZone();
    void carMoveOnce(CarObject *car, float &lastWheelAngle);
    bool experimentCheckIsCarSuccessfullyPassTrack(int i);
    void convertTime(const int microSecond, QString &strHour, QString &strMinute, QString &strSecond);

private slots:
    // view event
    void onSliderValueChange();
    void onBtnClick_CaseDataSelect();
    void onBtnClick_TrainDataSelect();
    void onBtnClick_GA();
    void onBtnClick_Restart();
    void onBtnClick_ConsoleClear();
    void onBtnClick_Experiment();

private slots:
    // other event
    void GA_DidStart();
    void GA_DidFinish();
    void GA_DidFail();
    void GA_ExperimentDidStart();
    void GA_ExperimentDidFinish();
    void GA_ExperimentDidFail();
    void onTimeOut();
    void onExperimentTimeOut();

private:
    // other object
    CaseDataLoader *m_init;
    SystemDataCenter *m_data;
    GA_ThreadRunner *m_GA_runner;
    GA_ThreadMultipleRunner *m_GA_experimentRunner;
    QThread *m_thread;
    QTimer *m_timer, *m_timer_experiment;
    bool m_bIsExperimentMode;
    int m_nExperimentTime;

// view object
private:
    QGridLayout *m_layout;

    vector<QWidget*> m_vAllWidget;
    QSlider *m_sliderGeneration, *m_sliderPopulation, *m_sliderCrossoverRate, *m_sliderMutationRate, *m_sliderRBFN_hiddenNeural_J;
    QLabel *m_lb_titleGeneration, *m_lb_titlePopulation, *m_lb_titleCrossoverRate, *m_lb_titleMutationRate, *m_lb_titleRBFN_hiddenNeural_J;
    QLabel *m_lb_valueGeneration, *m_lb_valuePopulation, *m_lb_valueCrossoverRate, *m_lb_valueMutationRate, *m_lb_valueRBFN_hiddenNeural_J;

    QPushButton *m_btn_selectCaseData, *m_btn_selectTrainData, *m_btn_restart, *m_btn_consolClear;
    QPushButton *m_btn_GA_start, *m_btn_experiment;

    QProgressBar *m_progressBar, *m_experimentProgressBar;

    QLabel *m_lb_experimentTimes, *m_lb_experimentTimes_value;
    QSlider *m_slider_experimentTimes;

    QPlainTextEdit *m_txtConsole;

    Canvas *m_mainCanvasView;
};

#endif // MAINWINDOW_H
