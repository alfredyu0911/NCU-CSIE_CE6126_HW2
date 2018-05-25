#include <QFileDialog>
#include <QScrollBar>
#include <QTime>

#include "MainWindow.h"
#include "TrainDataLoader.h"
#include "GA_ThreadRunner.h"
#include "RBFN.h"

MainWindow::MainWindow(QWidget *parent):QWidget(parent)
{
    m_layout = NULL;
    m_init = NULL;
    m_data = NULL;
    m_GA_runner = NULL;
    m_GA_experimentRunner = NULL;
    m_thread = new QThread();
    m_timer = new QTimer(this);
    m_timer_experiment = new QTimer(this);

    m_bIsExperimentMode = false;

    allCompoentInit();
    addCompoentToContainer();
    compoentEventInit();

    setGridLayout();
}

MainWindow::~MainWindow()
{
    while ( m_vAllWidget.size() >= 1 )
    {
        QWidget *obj = m_vAllWidget.back();
        m_vAllWidget.pop_back();
        if ( obj != NULL )
            delete obj;
    }

    if ( m_layout != NULL )
        delete m_layout;

    if ( m_init != NULL )
        delete m_init;

    if ( m_data != NULL )
        delete m_data;

    if ( m_timer != NULL )
        delete m_timer;

    if ( m_GA_runner != NULL )
        delete m_GA_runner;

    if ( m_GA_experimentRunner!= NULL )
        delete m_GA_experimentRunner;

    if ( m_timer_experiment != NULL )
        delete m_timer_experiment;

//    if ( m_thread != NULL )
//        delete m_thread;
}

void MainWindow::allCompoentInit()
{
    m_sliderGeneration = new QSlider(Qt::Horizontal);
    m_sliderGeneration->setRange(200, 1000);
    m_sliderGeneration->setValue(1000);

    m_sliderPopulation = new QSlider(Qt::Horizontal);
    m_sliderPopulation->setRange(100, 500);
    m_sliderPopulation->setValue(500);

    m_sliderCrossoverRate = new QSlider(Qt::Horizontal);
    m_sliderCrossoverRate->setRange(0, 100);
    m_sliderCrossoverRate->setValue(45);

    m_sliderMutationRate = new QSlider(Qt::Horizontal);
    m_sliderMutationRate->setRange(0, 20);
    m_sliderMutationRate->setValue(9);

    m_sliderRBFN_hiddenNeural_J = new QSlider(Qt::Horizontal);
    m_sliderRBFN_hiddenNeural_J->setRange(1, 15);
    m_sliderRBFN_hiddenNeural_J->setValue(1);

    m_lb_titleGeneration = new QLabel(QString("繁衍次數"));
    m_lb_titlePopulation = new QLabel(QString("族群數量"));
    m_lb_titleCrossoverRate = new QLabel(QString("交配機率"));
    m_lb_titleMutationRate = new QLabel(QString("突變機率"));
    m_lb_titleRBFN_hiddenNeural_J = new QLabel(QString("J"));

    m_lb_valueGeneration = new QLabel(QString::number(m_sliderGeneration->value()));
    m_lb_valuePopulation = new QLabel(QString::number(m_sliderPopulation->value()));
    m_lb_valueCrossoverRate = new QLabel(QString::number(m_sliderCrossoverRate->value())+QString("%"));
    m_lb_valueMutationRate = new QLabel(QString::number(m_sliderMutationRate->value())+QString("%"));
    m_lb_valueRBFN_hiddenNeural_J = new QLabel(QString::number(m_sliderRBFN_hiddenNeural_J->value()));

    m_btn_selectCaseData = new QPushButton(QString("載入車軌設定"));
    m_btn_selectTrainData = new QPushButton(QString("載入訓練資料"));
    m_btn_restart = new QPushButton(QString("清除重來"));
    m_btn_restart->setEnabled(false);
    m_btn_consolClear = new QPushButton(QString("Console clear"));

    m_txtConsole = new QPlainTextEdit();
    QTextDocument *doc = m_txtConsole->document();
    QFont font = doc->defaultFont();
    font.setFamily("Courier New");
    doc->setDefaultFont(font);
    m_txtConsole->setReadOnly(true);

    m_btn_GA_start = new QPushButton(QString("GA Start once"));
    m_btn_GA_start->setEnabled(false);
    m_progressBar = new QProgressBar();
    m_progressBar->setRange(0, 1000);
    m_progressBar->setValue(0);

    m_lb_experimentTimes = new QLabel(QString("實驗次數"));
    m_slider_experimentTimes = new QSlider(Qt::Horizontal);
    m_slider_experimentTimes->setRange(10, 100);
    m_slider_experimentTimes->setValue(20);
    m_lb_experimentTimes_value = new QLabel(QString::number(m_slider_experimentTimes->value()));
    m_nExperimentTime = 20;

    m_btn_experiment = new QPushButton(QString("進行多次實驗"));
    m_btn_experiment->setEnabled(false);
    m_experimentProgressBar = new QProgressBar();
    m_experimentProgressBar->setRange(0, 1000);
    m_experimentProgressBar->setValue(0);

    m_data = new SystemDataCenter();
    m_mainCanvasView = new Canvas();
}

void MainWindow::addCompoentToContainer()
{
    m_vAllWidget.push_back(m_sliderGeneration);
    m_vAllWidget.push_back(m_sliderPopulation);
    m_vAllWidget.push_back(m_sliderCrossoverRate);
    m_vAllWidget.push_back(m_sliderMutationRate);
    m_vAllWidget.push_back(m_sliderRBFN_hiddenNeural_J);

    m_vAllWidget.push_back(m_lb_titleGeneration);
    m_vAllWidget.push_back(m_lb_titlePopulation);
    m_vAllWidget.push_back(m_lb_titleCrossoverRate);
    m_vAllWidget.push_back(m_lb_titleMutationRate);
    m_vAllWidget.push_back(m_lb_titleRBFN_hiddenNeural_J);

    m_vAllWidget.push_back(m_lb_valueGeneration);
    m_vAllWidget.push_back(m_lb_valuePopulation);
    m_vAllWidget.push_back(m_lb_valueCrossoverRate);
    m_vAllWidget.push_back(m_lb_valueMutationRate);
    m_vAllWidget.push_back(m_lb_valueRBFN_hiddenNeural_J);

    m_vAllWidget.push_back(m_btn_selectCaseData);
    m_vAllWidget.push_back(m_btn_selectTrainData);
    m_vAllWidget.push_back(m_btn_restart);
    m_vAllWidget.push_back(m_btn_consolClear);

    m_vAllWidget.push_back(m_btn_GA_start);
    m_vAllWidget.push_back(m_progressBar);

    m_vAllWidget.push_back(m_lb_experimentTimes);
    m_vAllWidget.push_back(m_slider_experimentTimes);
    m_vAllWidget.push_back(m_lb_experimentTimes_value);

    m_vAllWidget.push_back(m_btn_experiment);
    m_vAllWidget.push_back(m_experimentProgressBar);

    m_vAllWidget.push_back(m_txtConsole);

    m_vAllWidget.push_back(m_mainCanvasView);
}

void MainWindow::setGridLayout()
{
    m_layout = new QGridLayout();

    m_layout->addWidget(m_lb_titleGeneration, 0, 0, 1, 1, 0);
    m_layout->addWidget(m_lb_titlePopulation, 1, 0, 1, 1, 0);
    m_layout->addWidget(m_lb_titleCrossoverRate, 2, 0, 1, 1, 0);
    m_layout->addWidget(m_lb_titleMutationRate, 3, 0, 1, 1, 0);
    m_layout->addWidget(m_lb_titleRBFN_hiddenNeural_J, 4, 0, 1, 1, 0);
    m_layout->addWidget(m_lb_experimentTimes, 5, 0, 1, 1, 0);

    m_layout->addWidget(m_sliderGeneration, 0, 1, 1, 6, 0);
    m_layout->addWidget(m_sliderPopulation, 1, 1, 1, 6, 0);
    m_layout->addWidget(m_sliderCrossoverRate, 2, 1, 1, 6, 0);
    m_layout->addWidget(m_sliderMutationRate, 3, 1, 1, 6, 0);
    m_layout->addWidget(m_sliderRBFN_hiddenNeural_J, 4, 1, 1, 6, 0);
    m_layout->addWidget(m_slider_experimentTimes, 5, 1, 1, 6, 0);

    m_layout->addWidget(m_lb_valueGeneration, 0, 7, 1, 1, 0);
    m_layout->addWidget(m_lb_valuePopulation, 1, 7, 1, 1, 0);
    m_layout->addWidget(m_lb_valueCrossoverRate, 2, 7, 1, 1, 0);
    m_layout->addWidget(m_lb_valueMutationRate, 3, 7, 1, 1, 0);
    m_layout->addWidget(m_lb_valueRBFN_hiddenNeural_J, 4, 7, 1, 1, 0);
    m_layout->addWidget(m_lb_experimentTimes_value, 5, 7, 1, 1, 0);

    m_layout->addWidget(m_btn_selectCaseData, 6, 0, 1, 2, 0);
    m_layout->addWidget(m_btn_selectTrainData, 6, 2, 1, 2, 0);
    m_layout->addWidget(m_btn_restart, 6, 4, 1, 2, 0);
    m_layout->addWidget(m_btn_consolClear, 6, 6, 1, 2, 0);

    m_layout->addWidget(m_btn_GA_start, 7, 0, 1, 2, 0);
    m_layout->addWidget(m_progressBar, 7, 2, 1, 6, 0);

    m_layout->addWidget(m_btn_experiment, 8, 0, 1, 2, 0);
    m_layout->addWidget(m_experimentProgressBar, 8, 2, 1, 6, 0);

    m_layout->addWidget(m_txtConsole, 9, 0, 3, 8, 0);

    m_layout->addWidget(m_mainCanvasView, 0, 8, 12, 10, 0);

    setLayout(m_layout);
}

void MainWindow::compoentEventInit()
{
    connect(m_sliderGeneration, SIGNAL(valueChanged(int)), this, SLOT(onSliderValueChange()));
    connect(m_sliderPopulation, SIGNAL(valueChanged(int)), this, SLOT(onSliderValueChange()));
    connect(m_sliderCrossoverRate, SIGNAL(valueChanged(int)), this, SLOT(onSliderValueChange()));
    connect(m_sliderMutationRate, SIGNAL(valueChanged(int)), this, SLOT(onSliderValueChange()));
    connect(m_sliderRBFN_hiddenNeural_J, SIGNAL(valueChanged(int)), this, SLOT(onSliderValueChange()));
    connect(m_slider_experimentTimes, SIGNAL(valueChanged(int)), this, SLOT(onSliderValueChange()));

    connect(m_btn_selectCaseData, SIGNAL(clicked()), this , SLOT(onBtnClick_CaseDataSelect()));
    connect(m_btn_selectTrainData, SIGNAL(clicked()), this , SLOT(onBtnClick_TrainDataSelect()));
    connect(m_btn_restart, SIGNAL(clicked()), this , SLOT(onBtnClick_Restart()));
    connect(m_btn_consolClear, SIGNAL(clicked()), this , SLOT(onBtnClick_ConsoleClear()));

    connect(m_btn_GA_start, SIGNAL(clicked()), this , SLOT(onBtnClick_GA()));
    connect(m_btn_experiment, SIGNAL(clicked()), this , SLOT(onBtnClick_Experiment()));

    connect(m_timer, SIGNAL(timeout()), this, SLOT(onTimeOut()));
    connect(m_timer_experiment, SIGNAL(timeout()), this, SLOT(onExperimentTimeOut()));
}

void MainWindow::onSliderValueChange()
{
    m_lb_valueGeneration->setText(QString::number(m_sliderGeneration->value()));
    m_lb_valuePopulation->setText(QString::number(m_sliderPopulation->value()));
    m_lb_valueCrossoverRate->setText(QString::number(m_sliderCrossoverRate->value())+QString("%"));
    m_lb_valueMutationRate->setText(QString::number(m_sliderMutationRate->value())+QString("%"));
    m_lb_valueRBFN_hiddenNeural_J->setText(QString::number(m_sliderRBFN_hiddenNeural_J->value()));
    m_lb_experimentTimes_value->setText(QString::number(m_slider_experimentTimes->value()));
    m_nExperimentTime = m_slider_experimentTimes->value();
}

void MainWindow::onBtnClick_CaseDataSelect()
{
    QString strOutput = QFileDialog::getOpenFileName(this, QString("select input file"), "", QString("Text Files (*.txt)"));
    m_init = new CaseDataLoader(string(strOutput.toLocal8Bit()));
    fileLoadStatue status = m_init->initCheck();
    if ( status == FLS_NO_ERROR )
    {
        consoleAddMsg(QString("Load success."));
        consoleAddMsg(strOutput);
        consoleAddMsg(QString("\n"));
        m_btn_selectCaseData->setEnabled(false);

        m_data->m_carPosition = m_init->getCarInitCoordinate();
        m_data->m_carAngle = m_init->getCarInitAngle();
        dataInit();
    }
    else if ( status == FLS_FILE_PATH_INVAILD )
    {
        consoleAddMsg(QString("File path error, the path must contain no Chinese letter"));
    }
    else
    {
        consoleAddMsg(QString("Error, load fail."));
    }

    if ( !m_btn_selectCaseData->isEnabled() && !m_btn_selectTrainData->isEnabled() )
    {
        m_btn_GA_start->setEnabled(true);
        m_btn_experiment->setEnabled(true);
    }
}

void MainWindow::onBtnClick_TrainDataSelect()
{
    QString strOutput = QFileDialog::getOpenFileName(this, QString("select input file"), "", QString("Text Files (*.txt)"));

    TrainDataLoader *loader = new TrainDataLoader();
    m_data->m_vTrainData = loader->load(string(strOutput.toLocal8Bit()));
    fileLoadStatue status = loader->checkStatus();

    if ( status == FLS_FILE_PATH_INVAILD )
    {
        consoleAddMsg(QString("File path error, the path must contain no Chinese letter"));
    }
    else if ( m_data->m_vTrainData.size() < 1 || status != FLS_NO_ERROR )
    {
        consoleAddMsg(QString("Error, load fail."));
    }
    else
    {
        consoleAddMsg(QString("Load success."));
        consoleAddMsg(strOutput);
        consoleAddMsg(QString("\n"));
        m_btn_selectTrainData->setEnabled(false);
    }

    if ( !m_btn_selectCaseData->isEnabled() && !m_btn_selectTrainData->isEnabled() )
    {
        m_btn_GA_start->setEnabled(true);
        m_btn_experiment->setEnabled(true);
    }
}

void MainWindow::onBtnClick_GA()
{
    m_timer->start(100);
    m_progressBar->setMaximum(m_sliderGeneration->value());
    m_progressBar->setValue(0);

    if ( m_GA_runner == NULL )
    {
        m_GA_runner = new GA_ThreadRunner();
        connect(m_GA_runner, SIGNAL(runner_didStart()), this, SLOT(GA_DidStart()));
        connect(m_GA_runner, SIGNAL(runner_error()), this, SLOT(GA_DidFail()));
        connect(m_GA_runner, SIGNAL(runner_didFinish()), this, SLOT(GA_DidFinish()));
    }

    m_GA_runner->input_GA_parameter(m_sliderRBFN_hiddenNeural_J->value(), m_data->m_vTrainData);
    m_GA_runner->update_GA_setting(m_sliderPopulation->value(), m_sliderCrossoverRate->value(), \
                                   m_sliderMutationRate->value(), m_sliderGeneration->value());
    m_GA_runner->moveToThread(m_thread);
    m_thread->start();

    m_sliderCrossoverRate->setEnabled(false);
    m_sliderGeneration->setEnabled(false);
    m_sliderMutationRate->setEnabled(false);
    m_sliderPopulation->setEnabled(false);
    m_sliderRBFN_hiddenNeural_J->setEnabled(false);
    m_slider_experimentTimes->setEnabled(false);
    m_btn_GA_start->setEnabled(false);
    m_btn_experiment->setEnabled(false);
    m_btn_restart->setEnabled(false);
}

void MainWindow::onBtnClick_Restart()
{
    m_btn_restart->setEnabled(false);
    m_data->clearCarRecord();
    dataInit();
    m_btn_GA_start->setEnabled(true);
    m_btn_experiment->setEnabled(true);
}

void MainWindow::onBtnClick_ConsoleClear()
{
    m_txtConsole->setPlainText(QString(""));
}

void MainWindow::onBtnClick_Experiment()
{
    m_timer_experiment->start(100);
    m_experimentProgressBar->setMaximum(m_sliderGeneration->value()*m_nExperimentTime);
    m_experimentProgressBar->setValue(0);

    if ( m_GA_experimentRunner == NULL )
    {
        m_GA_experimentRunner = new GA_ThreadMultipleRunner();
        connect(m_GA_experimentRunner, SIGNAL(multipleRunner_didStart()), this, SLOT(GA_ExperimentDidStart()));
        connect(m_GA_experimentRunner, SIGNAL(multipleRunner_error()), this, SLOT(GA_ExperimentDidFail()));
        connect(m_GA_experimentRunner, SIGNAL(multipleRunner_didFinish()), this, SLOT(GA_ExperimentDidFinish()));
    }

    m_GA_experimentRunner->setExperimentTime(m_nExperimentTime);
    m_GA_experimentRunner->input_GA_parameter(m_sliderRBFN_hiddenNeural_J->value(), m_data->m_vTrainData);
    m_GA_experimentRunner->update_GA_setting(m_sliderPopulation->value(), m_sliderCrossoverRate->value(), \
                                             m_sliderMutationRate->value(), m_sliderGeneration->value());
    m_GA_experimentRunner->moveToThread(m_thread);
    m_thread->start();

    m_sliderCrossoverRate->setEnabled(false);
    m_sliderGeneration->setEnabled(false);
    m_sliderMutationRate->setEnabled(false);
    m_sliderPopulation->setEnabled(false);
    m_sliderRBFN_hiddenNeural_J->setEnabled(false);
    m_slider_experimentTimes->setEnabled(false);
    m_btn_GA_start->setEnabled(false);
    m_btn_experiment->setEnabled(false);
    m_btn_restart->setEnabled(false);
}

void MainWindow::GA_DidStart()
{
    consoleAddMsg(QString("Genetic Algorithm did start running"));
}

void MainWindow::GA_DidFinish()
{
    m_timer->stop();
    m_progressBar->setValue(m_progressBar->maximum());
    consoleAddMsg(QString("Genetic Algorithm did finish running"));

    TrainDataType type = m_data->m_vTrainData[m_data->m_vTrainData.size()-1].dataType;

    consoleAddMsg(QString("=======以下為RBFN網路使用基因演算法訓練出之最佳解的輸出結果======="));
    if ( type == TrainDataType_6D )
        consoleAddMsg(QString("(x, y)  distance(Front, Right, Left)  [RBFN output angle]"));
    else
        consoleAddMsg(QString("distance(Front, Right, Left)  [RBFN output angle]"));

    QString msg;
    CarObject *car = new CarObject(m_data);

    while ( true )
    {
        if ( type == TrainDataType_6D )
        {
            QString strX, strY;
            strX.sprintf("%+06.2f", m_data->m_carPosition.x);
            strY.sprintf("%+06.2f", m_data->m_carPosition.y);
            msg += QString("(") + strX + QString(",") + strY + QString(") ");
        }

        float angle;
        carMoveOnce(car, angle);

        QString F, R, L, a;
        F.sprintf("%05.2f", m_data->m_vRecord[m_data->m_vRecord.size()-1].distanceF);
        R.sprintf("%05.2f", m_data->m_vRecord[m_data->m_vRecord.size()-1].distanceR);
        L.sprintf("%05.2f", m_data->m_vRecord[m_data->m_vRecord.size()-1].distanceL);
        a.sprintf("%+06.2f", m_data->m_vRecord[m_data->m_vRecord.size()-1].wheelAngle);

        msg += QString("(") + F + QString(",") + R + QString(",") + L + QString(") ");
        msg += QString("[angle: ") + a + QString("]\n");

        if ( car->collisionCheck() == true )
        {
            msg += QString("碰撞發生，提前終止.");
            break;
        }
        if ( isCarReachEndZone() == true )
        {
            msg += QString("成功通過軌道！");
            break;
        }
    }
    delete car;

    int time = m_GA_runner->checkExecuteTime();
    QString ss, mm, hh;
    convertTime(time, hh, mm, ss);

    consoleAddMsg(msg);
    consoleAddMsg(QString("適應函數值： ") + QString::number(m_GA_runner->getBestFitnessValue()));
    consoleAddMsg(QString("耗時： ") + hh + QString("小時 ") + mm + QString("分 ") + ss + QString("秒") );
    consoleAddMsg(QString("平均誤差： ") + QString::number(m_GA_runner->getBsetAverageError()));
    consoleAddMsg(QString("=======以上為RBFN網路使用基因演算法訓練出之最佳解的輸出結果======="));
    consoleAddMsg(QString("\n"));

    allCompoentRefresh();

    m_sliderCrossoverRate->setEnabled(true);
    m_sliderGeneration->setEnabled(true);
    m_sliderMutationRate->setEnabled(true);
    m_sliderPopulation->setEnabled(true);
    m_sliderRBFN_hiddenNeural_J->setEnabled(true);
    m_slider_experimentTimes->setEnabled(true);
    m_btn_restart->setEnabled(true);

    delete m_GA_runner;
    m_GA_runner = NULL;
}

void MainWindow::GA_DidFail()
{
    consoleAddMsg(QString("Genetic Algorithm fail"));
    m_progressBar->setValue(0);

    m_sliderCrossoverRate->setEnabled(true);
    m_sliderGeneration->setEnabled(true);
    m_sliderMutationRate->setEnabled(true);
    m_sliderPopulation->setEnabled(true);
    m_sliderRBFN_hiddenNeural_J->setEnabled(true);
    m_slider_experimentTimes->setEnabled(true);
    m_btn_GA_start->setEnabled(true);
    m_btn_experiment->setEnabled(true);
    m_btn_restart->setEnabled(true);
}

void MainWindow::GA_ExperimentDidStart()
{
    consoleAddMsg(QString("Genetic Algorithm Experiment did start running"));
}

void MainWindow::GA_ExperimentDidFinish()
{
    m_timer_experiment->stop();
    m_experimentProgressBar->setValue(m_experimentProgressBar->maximum());

    consoleAddMsg(QString("Genetic Algorithm Experiment did finish running"));
    consoleAddMsg(QString("=============以下為基因演算法運行 ") + QString::number(m_nExperimentTime) + QString(" 次的輸出結果============="));
    consoleAddMsg(QString("實驗參數 [") + QString::number(m_sliderGeneration->value()) + QString(", ") + \
                                         QString::number(m_sliderPopulation->value()) + QString(", ") + \
                                         QString::number(m_sliderCrossoverRate->value()) + QString(", ") + \
                                         QString::number(m_sliderMutationRate->value()) + QString(", ") + \
                                         QString::number(m_sliderRBFN_hiddenNeural_J->value()) + QString("]"));
    consoleAddMsg(QString("[Best fitness], [Average Error], [是否成功通過軌道]"));

    vector<float> vAvgError = m_GA_experimentRunner->getAllBestAverageError();
    vector<float> vBestFit = m_GA_experimentRunner->getAllBestFitnessValue();
    for ( int i=0 ; i < m_nExperimentTime ; i++ )
    {
        QString msg = QString("");
        QString err, fit, pass;
        err.sprintf("%10.7f  ", vAvgError[i]);
        fit.sprintf("%10.7f   ", vBestFit[i]);
        pass = experimentCheckIsCarSuccessfullyPassTrack(i) ? QString("Yes") : QString("No ");
        msg += QString("[") + fit + QString("], ");
        msg += QString("[") + err + QString("], ");
        msg += QString("[") + pass + QString("]");
        consoleAddMsg(msg);
    }

    int time = m_GA_experimentRunner->checkExecuteTime();
    QString ss, mm, hh, ss2, mm2, hh2;
    convertTime(time, hh, mm, ss);
    convertTime(time/m_nExperimentTime, hh2, mm2, ss2);

    consoleAddMsg(QString("全部執行時間: ") + hh + QString("小時 ") + mm + QString("分 ") + ss + QString("秒"));
    consoleAddMsg(QString("平均執行時間: ") + hh2 + QString("小時 ") + mm2 + QString("分 ") + ss2 + QString("秒"));
    consoleAddMsg(QString("=============以上為基因演算法運行 ") + QString::number(m_nExperimentTime) + QString(" 次的輸出結果============="));

    m_sliderCrossoverRate->setEnabled(true);
    m_sliderGeneration->setEnabled(true);
    m_sliderMutationRate->setEnabled(true);
    m_sliderPopulation->setEnabled(true);
    m_sliderRBFN_hiddenNeural_J->setEnabled(true);
    m_slider_experimentTimes->setEnabled(true);
    m_btn_restart->setEnabled(true);

    delete m_GA_experimentRunner;
    m_GA_experimentRunner = NULL;
}

void MainWindow::GA_ExperimentDidFail()
{
    consoleAddMsg(QString("Genetic Algorithm Experiment fail"));
    m_experimentProgressBar->setValue(0);

    m_sliderCrossoverRate->setEnabled(true);
    m_sliderGeneration->setEnabled(true);
    m_sliderMutationRate->setEnabled(true);
    m_sliderPopulation->setEnabled(true);
    m_sliderRBFN_hiddenNeural_J->setEnabled(true);
    m_slider_experimentTimes->setEnabled(true);
    m_btn_GA_start->setEnabled(true);
    m_btn_experiment->setEnabled(true);
    m_btn_restart->setEnabled(true);
}

void MainWindow::onTimeOut()
{
    m_progressBar->setValue(m_GA_runner->checkProgress());
}

void MainWindow::onExperimentTimeOut()
{
    m_experimentProgressBar->setValue(m_GA_experimentRunner->checkProgress());
}

void MainWindow::carMoveOnce(CarObject *car, float &lastWheelAngle)
{
    Geometry geoObj;

    m_data->m_intersectPt_L = m_data->nextIntersectionPoint(m_data->m_carAngle.getDegree() + 45.0);
    m_data->m_intersectPt_R = m_data->nextIntersectionPoint(m_data->m_carAngle.getDegree() - 45.0);
    m_data->m_intersectPt_F = m_data->nextIntersectionPoint(m_data->m_carAngle.getDegree());

    float F = geoObj.getDistance(m_data->m_carPosition, m_data->m_intersectPt_F);
    float R = geoObj.getDistance(m_data->m_carPosition, m_data->m_intersectPt_R);
    float L = geoObj.getDistance(m_data->m_carPosition, m_data->m_intersectPt_L);

    lastWheelAngle = m_GA_runner->getAngle(F, R, L);

    if ( lastWheelAngle > 0 )
    {
        m_data->carTurnRight(lastWheelAngle);
    }
    else
    {
        m_data->carTurnLeft(lastWheelAngle);
    }

    car->move();

    Record rec = Record(m_data->m_carPosition, L, R, F, m_data->m_carWheelAngle.getDegree());
    m_data->addCarNewRecord(rec);
}

bool MainWindow::experimentCheckIsCarSuccessfullyPassTrack(int i)
{
    Geometry geoObj;
    CarObject *car = new CarObject(m_data);
    m_data->m_carAngle = m_init->getCarInitAngle();
    m_data->m_carPosition = m_init->getCarInitCoordinate();
    TrainDataType type = m_data->m_vTrainData[m_data->m_vTrainData.size()-1].dataType;

    while ( true )
    {
        m_data->m_intersectPt_L = m_data->nextIntersectionPoint(m_data->m_carAngle.getDegree() + 45.0);
        m_data->m_intersectPt_R = m_data->nextIntersectionPoint(m_data->m_carAngle.getDegree() - 45.0);
        m_data->m_intersectPt_F = m_data->nextIntersectionPoint(m_data->m_carAngle.getDegree());

        float F = geoObj.getDistance(m_data->m_carPosition, m_data->m_intersectPt_F);
        float R = geoObj.getDistance(m_data->m_carPosition, m_data->m_intersectPt_R);
        float L = geoObj.getDistance(m_data->m_carPosition, m_data->m_intersectPt_L);

        RBFN *network = new RBFN(m_GA_experimentRunner->getAllBestResults()[i]);

        InputVector input(type);
        int idx=0;
        if ( type == TrainDataType_6D )
        {
            input.aryElement[idx++] = (m_data->m_carPosition.x-30.0) / 40.0;
            input.aryElement[idx++] = (m_data->m_carPosition.y-30.0) / 40.0;
        }
        input.aryElement[idx++] = (F-40.0) / 40.0;
        input.aryElement[idx++] = (R-40.0) / 40.0;
        input.aryElement[idx++] = (L-40.0) / 40.0;

        float angle = network->getResultByInputVector(input) * 40.0;
        delete network;
        (angle > 0) ? m_data->carTurnRight(angle) : m_data->carTurnLeft(angle);
        car->move();

        if ( car->collisionCheck() == true )
        {
            delete car;
            return false;
        }
        else if ( isCarReachEndZone() == true )
        {
            break;
        }
    }

    delete car;
    return true;
}

void MainWindow::dataInit()
{
    if ( m_data->m_vTrackBorderInfo.empty() )
        m_data->m_vTrackBorderInfo = m_init->getTrackInfo();

    if ( m_data->m_vEndZoneBorderInfo.empty() )
        m_data->m_vEndZoneBorderInfo = m_init->getEndZoneInfo();

    m_data->m_carAngle = m_init->getCarInitAngle();
    m_data->m_carPosition = m_init->getCarInitCoordinate();

    m_mainCanvasView->setDataCenter(m_data);

    allCompoentRefresh();
}

void MainWindow::allCompoentRefresh()
{
    m_data->m_intersectPt_L = m_data->nextIntersectionPoint(m_data->m_carAngle.getDegree() + 45.0);
    m_data->m_intersectPt_R = m_data->nextIntersectionPoint(m_data->m_carAngle.getDegree() - 45.0);
    m_data->m_intersectPt_F = m_data->nextIntersectionPoint(m_data->m_carAngle.getDegree());

    m_mainCanvasView->repaint();
}

void MainWindow::consoleAddMsg(QString strMsg)
{
    if ( m_txtConsole->toPlainText().toStdString() == "" )
    {
        m_txtConsole->setPlainText(m_txtConsole->toPlainText() + strMsg);
    }
    else
    {
        if ( m_txtConsole->toPlainText().toStdString().back() == '\n' )
        {
            m_txtConsole->setPlainText(m_txtConsole->toPlainText() + strMsg);
        }
        else
        {
            m_txtConsole->setPlainText(m_txtConsole->toPlainText() + QString("\n") + strMsg);
        }
    }

    m_txtConsole->verticalScrollBar()->setValue(m_txtConsole->verticalScrollBar()->maximum());
}

bool MainWindow::isCarReachEndZone()
{
    float xMax=-1000, xMin=1000, yMax=-1000, yMin=1000;

    for ( vector<Point>::iterator it=m_data->m_vEndZoneBorderInfo.begin() ; it != m_data->m_vEndZoneBorderInfo.end() ; it++ )
    {
        Point pt = (*it);

        if ( pt.x > xMax )
            xMax = pt.x;

        if ( pt.x < xMin )
            xMin = pt.x;

        if ( pt.y > yMax )
            yMax = pt.y;

        if ( pt.y < yMin )
            yMin = pt.y;
    }

    if ( m_data->m_carPosition.x >= xMin && m_data->m_carPosition.x <= xMax )
    {
        if ( m_data->m_carPosition.y >= yMin && m_data->m_carPosition.y <= yMax )
            return true;
    }

    return false;
}

void MainWindow::convertTime(const int microSecond, QString &strHour, QString &strMinute, QString &strSecond)
{
    float ss = (int)((microSecond / 1000) % 60) + ((float)(microSecond % 1000) / 1000.0);
    int mm = (microSecond / 1000) / 60 % 60;
    int hh = (microSecond / 1000) / 3600 % 24;
    strHour.sprintf("%02d", hh);
    strMinute.sprintf("%02d", mm);
    strSecond.sprintf("%07.4f", ss);
}
