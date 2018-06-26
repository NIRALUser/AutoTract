#ifndef DEF_Pipeline
#define DEF_Pipeline

// General Librairies
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string>
#include <cstring>
#include <vector>
#include <utility>
#include <sys/stat.h>

// Qt Librairies
#include <QDir>
#include <QStringList>
#include <QProcess>
#include <QTime>
#include <QPlainTextEdit>
#include <QFileInfo>

#include "para_Model_AutoTract.h"
#include "soft_Model_AutoTract.h"

#include "Registration.h"
#include "TractPopulationProcess.h"
#include "SingleTractProcess.h"
#include "MaskCreation.h"
#include "Classification.h"

class Pipeline
{
public:

    // Constructor
    Pipeline();

    // Destructor
    ~Pipeline();

    // Parameters
    void setPipelineParameters(para_Model_AutoTract *para_m);
    void setPipelineSoftwares(soft_Model_AutoTract *soft_m);
    void setPlainTextEdit(QPlainTextEdit* plainTextEdit);

    // Process
    QProcess* getMainScriptProcess();

    // Write Pipeline
    void writePipeline();
    void runPipeline();
    void stopPipeline();

private:
    void createProcessingDirectory();
    QString createModuleDirectory(QString directory_name);

    // Write Main Script
    void initializeMainScript();
    void initializeLogging();
    void defineSignalHandler();
    void writeMainScript();
    void writeRegistration();
    void writeMaskCreation();
    void writeSingleTractProcess();
    void writeProcess();
    void writeClassification();
    void executeMainScript();
    void copySegmentations();

    // Clean-Up
    void cleanUp();

    Registration* m_registration;
    MaskCreation* m_maskCreation;
    TractPopulationProcess* m_process;
    SingleTractProcess* m_singleTractProcess;
    Classification *m_classification;

    QString m_importingModules;
    QString m_runningModules;

    para_Model_AutoTract* m_para_m;
    soft_Model_AutoTract* m_soft_m;

    QString m_processing_name;
    QString m_processing_path;

    QString m_script;

    QString m_main_path;

    QString m_log_path;

    QString m_jobID;

    QString m_displacementFieldPath;

    // QProcess
    QProcess* m_mainScriptProcess;

    // PlainTextEdit
    QPlainTextEdit* m_plainTextEdit;

    // Timer
    QTime m_timer;
};

#endif
