#ifndef THREADRUNNER_H
#define THREADRUNNER_H

#include <QObject>
#include <QTimer>

#include "GeneticAlgorithm.h"
#include "AllStructure.h"
#include "AllEnumerate.h"

class GA_ThreadRunner : public QObject
{
    Q_OBJECT

public:
    explicit GA_ThreadRunner(QObject *parent = nullptr);

    void input_GA_parameter(int numberOfHiddenNeural_J, vector<DataContent> vInputData);
    void update_GA_setting(int population, int crossoverRate, int mutationRate, int generation);

    float getAngle(float disF, float disR, float disL, float x=0, float y=0);
    float getBestFitnessValue();
    float getBsetAverageError();

    virtual int checkProgress();
    virtual int checkExecuteTime();

protected:
    GeneticAlgorithm *m_GA;
    QTimer *m_timer;

    void initRunner();

signals:
    void runner_didStart();
    void runner_didFinish();
    void runner_error();

private slots:
    virtual void doRun();
};

class GA_ThreadMultipleRunner : public GA_ThreadRunner
{
    Q_OBJECT

public:
    explicit GA_ThreadMultipleRunner(GA_ThreadRunner *parent = nullptr);

    void update_GA_setting(int population, int crossoverRate, int mutationRate, int generation);

    vector<float> getAllBestFitnessValue() { return m_vBestFitnessValue; }
    vector<float> getAllBestAverageError() { return m_vBestAverageError; }

    int checkProgress();
    int checkExecuteTime() { return m_nExecuteTime; }

protected:
    vector<float> m_vBestFitnessValue;
    vector<float> m_vBestAverageError;
    int m_nProgress, m_nGeneration, m_nExecuteTime;

signals:
    void multipleRunner_didStart();
    void multipleRunner_didFinish();
    void multipleRunner_error();

private slots:
    void doRun();
};

#endif // THREADRUNNER_H
