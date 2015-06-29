#ifndef DEF_DerivedWindow
#define DEF_DerivedWindow

#include "AutoTractWindow.h"
#include "Pipeline.h"
#include <QFileDialog>
#include <QSignalMapper>
#include <iterator>
#include <iostream>
#include <fstream>
#include <QtGui>
#include <itksys/SystemTools.hxx>

class AutoTractDerivedWindow :public AutoTractWindow
{
    Q_OBJECT
public:
    AutoTractDerivedWindow();
    void displayTracts();
    void checkSelectedTracts();
    void checkTracts();
    void initSoftware();
public slots:
    void UpdateTractPopulationDirectoryDisplay();
    void selectTracts(QListWidgetItem* item);
    void selectTractsPopulationDirectory();
    void enterTractPopulationDirectory();
    void selectOutputDirectory();
    void enterOutputDirectory();
    void initializePipelineLogging();
    void printPipelineLog();
    void checkAllTracts();
    void uncheckAllTracts();
    void changeComputingSystem();
private slots:
    void SaveParaConfigFile();
    void LoadParaConfigFile();
    void SaveSoftConfigFile();
    void LoadSoftConfigFile();
    void selectExecutable(QString executable_name);
    void enterExecutable(QString executable_name);
    void resetExecutable(QString executable_name);
    void selectParameters(QString parameters_name);
    void enterParameters(QString parameters_name);
    void runPipeline();
    void stopPipeline();


private:
    struct Executable
    {
        QPushButton* select_button;
        QLineEdit* enter_lineEdit;
        QPushButton* reset_button;
    };
    struct Parameters
    {
        QPushButton* select_button;
        QLineEdit* enter_lineEdit;
    };
    void closeEvent(QCloseEvent *event);
    QMap<QString, Executable> m_executables_map;
    QMap<QString, Parameters> m_parameters_map;
    QTextStream* m_log_textStream;
    MainScriptThread* m_thread;
    void initializeExecutablesMap();
    void initializeParametersMap();
    Pipeline* m_pipeline;
    QStringList m_selectedTracts;
    QString m_tractPopulationDirectory;
    QString m_script;
};

#endif
