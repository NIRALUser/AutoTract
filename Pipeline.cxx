#include "Pipeline.h"


Pipeline::Pipeline()
{
    m_processing_name = "Script";
}

Pipeline::~Pipeline()
{

}

void Pipeline::setPipelineParameters(para_Model_AutoTract* para_m)
{
    m_para_m = para_m;
}

void Pipeline::setPipelineSoftwares(soft_Model_AutoTract* soft_m)
{
    m_soft_m = soft_m;
}
void Pipeline::setPlainTextEdit(QPlainTextEdit* plainTextEdit)
{
    m_plainTextEdit = plainTextEdit;
}

QProcess* Pipeline::getMainScriptProcess()
{
    return m_mainScriptProcess;
}

void Pipeline::createProcessingDirectory()
{
    QDir output_dir(m_para_m->getpara_output_dir_lineEdit());
    if(!output_dir.exists(m_processing_name))
    {
        output_dir.mkdir(m_processing_name);
    }
    m_processing_path = output_dir.filePath(m_processing_name);
}

QString Pipeline::createModuleDirectory(QString directory_name)
{
    QDir output_dir(m_para_m->getpara_output_dir_lineEdit());
    if(!output_dir.exists(directory_name))
    {
        output_dir.mkdir(directory_name);
    }
    return output_dir.filePath(directory_name);
}

void Pipeline::initializeMainScript()
{
    m_script = "#!/usr/bin/env python\n\n";

    m_script += "import os\n";
    m_script += "import sys\n";
    m_script += "import signal\n";
    m_script += "import logging\n";
    m_script += "import subprocess\n";
    m_script += "import datetime\n";
    m_script += "import shutil\n\n";
}

void Pipeline::defineSignalHandler()
{
    m_script += "def signal_handler(signal, frame):\n";
    m_script += "\tprint '***************You pressed Ctrl+C!******************'\n";
    m_script += "\tif runningProcess.poll!=1:\n";
    m_script += "\t\trunningProcess.terminate()\n";
    m_script += "\tsys.exit(0)\n\n";

    m_script += "signal.signal(signal.SIGINT, signal_handler)\n";
    m_script += "signal.signal(signal.SIGTERM, signal_handler)\n\n";
}

void Pipeline::initializeLogging()
{
    m_script += "log = \"" + m_log_path + "\"\n";
    m_script += "logFile = open(log, \"w\") \n\n";

    m_script += "logger = logging.getLogger('AutoTract')\n";
    m_script += "logger.setLevel(logging.DEBUG)\n\n";

    m_script += "fileHandler = logging.FileHandler(log)\n";
    m_script += "fileHandler.setLevel(logging.DEBUG)\n";
    m_script += "fileFormatter = logging.Formatter('%(message)s')\n";
    m_script += "fileHandler.setFormatter(fileFormatter)\n\n";

    m_script += "consoleHandler = logging.StreamHandler()\n";
    m_script += "consoleHandler.setLevel(logging.DEBUG)\n";
    m_script += "consoleFormatter = logging.Formatter('%(message)s')\n";
    m_script += "consoleHandler.setFormatter(consoleFormatter)\n\n";

    m_script += "logger.addHandler(fileHandler)\n";
    m_script += "logger.addHandler(consoleHandler)\n\n";
}

void Pipeline::writeMainScript()
{
    initializeMainScript();
    m_script += m_importingModules;
    defineSignalHandler();
    initializeLogging();
    m_script += "os.environ['ITK_GLOBAL_DEFAULT_NUMBER_OF_THREADS'] = '" + QString::number(m_para_m->getpara_nbCores_spinBox()) +  "' \n";

    m_script += "logger.info(sys.executable)\n";

    m_script += "start = datetime.datetime.now()\n\n";

    m_script += m_runningModules + "\n";

    m_script += "end = datetime.datetime.now()\n\n";

    m_script += "elapsedTime = (end - start).seconds\n";

    m_script += "hours = elapsedTime/(60*60)\n";
    m_script += "elapsedTime -= hours*(60*60)\n\n";

    m_script += "minutes = elapsedTime/60\n";
    m_script += "elapsedTime -= minutes*60\n\n";

    m_script += "seconds = elapsedTime\n\n";

    m_script += "logger.info('Pipeline took %s hour(s), %s minute(s) and %s second(s)', hours, minutes, seconds)\n";

    QDir* processing_dir = new QDir(m_processing_path);
    m_main_path = processing_dir->filePath("main.py");

    std::ofstream* script_stream = new std::ofstream((m_main_path.toStdString()).c_str());
    *script_stream << m_script.toStdString() << std::endl;

    script_stream->close();

    chmod((m_main_path.toStdString()).c_str(), 0755);
}

void Pipeline::writeRegistration()
{
    QString directory_name = "1.Registration";
    QString directory_path = createModuleDirectory(directory_name);

    QString module_name = "Registration";
    m_registration = new::Registration(module_name);
    m_registration->setOverwriting(m_para_m->getpara_overwrite_checkBox());
    m_registration->setModuleDirectory(directory_path);
    m_registration->setProcessingDirectory(m_processing_path);
    m_registration->setDisplacementFieldPath(m_displacementFieldPath);
    m_registration->setScriptParameters(m_para_m);
    m_registration->setScriptSoftwares(m_soft_m);

    /*m_registration->setStoppingIfError(m_parameters->getStoppingIfError());*/
    m_registration->update();
    m_importingModules += "import " + module_name + "\n";
    m_runningModules += module_name + ".run()\n";
}

void Pipeline::writeMaskCreation()
{
    QString directory_name = "2.MaskCreation";
    QString directory_path = createModuleDirectory(directory_name);

    QString module_name = "MaskCreation";
    m_maskCreation = new::MaskCreation(module_name);
    m_maskCreation->setScriptParameters(m_para_m);
    m_maskCreation->setScriptSoftwares(m_soft_m);
    m_maskCreation->setOutputDirectory(directory_name);
    m_maskCreation->setOverwriting(m_para_m->getpara_overwrite_checkBox());
    m_maskCreation->setModuleDirectory(directory_path);
    m_maskCreation->setProcessingDirectory(m_processing_path);


    /*m_maskCreation->setStoppingIfError(m_parameters->getStoppingIfError());*/
    m_maskCreation->update();
    m_importingModules += "import " + module_name + "\n";
    m_runningModules += module_name + ".run()\n";
}


void Pipeline::writeProcess()
{
    QString directory_name = "3.PostProcess";
    QString directory_path = createModuleDirectory(directory_name);

    QString module_name = "PostProcess";
    m_process = new::TractPopulationProcess(module_name);
    //m_process->setOverwriting(m_para_m->getpara_overwrite_checkBox());
    m_process->setModuleDirectory(directory_path);
    m_process->setProcessingDirectory(m_processing_path);
    m_process->setScriptParameters(m_para_m);
    m_process->setScriptSoftwares(m_soft_m);
    m_process->SetDisplacementFieldPath(m_displacementFieldPath);
    m_process->update();
    m_importingModules += "import " + module_name + "\n";
    m_runningModules += module_name + ".run()\n";
}

void Pipeline::writeSingleTractProcess()
{
    QString module_name = "SingleTractProcess";
    m_singleTractProcess = new::SingleTractProcess(module_name);

    m_singleTractProcess->setProcessingDirectory(m_processing_path);
    m_singleTractProcess->setLog(m_log_path);
    m_singleTractProcess->setScriptParameters(m_para_m);
    m_singleTractProcess->setScriptSoftwares(m_soft_m);
    /*m_singleTractProcess->setOverwriting(m_parameters->getOverwriting());
    m_singleTractProcess->setStoppingIfError(m_parameters->getStoppingIfError());*/
    m_singleTractProcess->update();
}


void Pipeline::cleanUp()
{
}

void Pipeline::writePipeline()
{
    m_importingModules = "";
    m_runningModules = "";

    createProcessingDirectory();
    QDir* output_dir = new QDir(m_para_m->getpara_output_dir_lineEdit());
    QFileInfo fi(m_para_m->getpara_output_dir_lineEdit());
    QString base = fi.baseName();
    m_log_path = output_dir->filePath(base + ".log");
    m_displacementFieldPath = m_para_m->getpara_output_dir_lineEdit() + "/" + "1.Registration" + "/displacementField.nrrd";
    writeRegistration();
    writeMaskCreation();
    writeProcess();
    writeSingleTractProcess();
    writeMainScript();
}

void Pipeline::runPipeline()
{
    QString command;
    //if(!(m_para_m->getpara_computingSystem_comboBox()).compare("local", Qt::CaseInsensitive) || !(m_para_m->getpara_computingSystem_comboBox()).compare("killdevil interactive", Qt::CaseInsensitive))
    //{
        command = m_main_path;
    //}

//    if(!(m_para_m->getpara_computingSystem_comboBox()).compare("killdevil", Qt::CaseInsensitive))
//    {
//        command = "bsub -q day -M 4 -n 1 -R \"span[hosts=1]\" python " +  m_main_path;
//    }
    QString python_path = m_soft_m->getsoft_python_lineEdit();
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    if(python_path!="")
    {
        QString pythonDirectory_path = ((QFileInfo(python_path)).absoluteDir()).path();
        env.insert("PATH", pythonDirectory_path + ":" + env.value("PATH"));
        env.insert("PYTHONPATH", "");
    }

    m_mainScriptProcess = new QProcess;
    m_mainScriptProcess->setProcessEnvironment(env);
    m_mainScriptProcess->start(command);
    m_mainScriptProcess->waitForStarted();
    m_mainScriptProcess->waitForFinished();
    m_timer.start();

    while (!m_mainScriptProcess->waitForFinished())
    {
        sleep(1);
    }

    if(!(m_para_m->getpara_computingSystem_comboBox()).compare("killdevil", Qt::CaseInsensitive))
    {
        bool jobRunning = true;

        QString output(m_mainScriptProcess->readAllStandardOutput());
        QRegExp regExp("(<{1})([0-9]{1,})(>{1})");
        regExp.indexIn(output);
        m_jobID = regExp.cap(2);

        std::cout<<"jobID = "<<m_jobID.toStdString()<<std::endl;

        QProcess* bjobs_process = new::QProcess();
        while(jobRunning)
        {
            bjobs_process->start("bjobs " + m_jobID);
            while (!bjobs_process->waitForFinished())
            {
                sleep(1);
            }

            QString bjobs_output(bjobs_process->readAllStandardOutput());
            if(bjobs_output.contains("DONE") || bjobs_output.contains("EXIT"))
            {
                jobRunning = false;
            }

            sleep(1);
        }
    }

    if(m_mainScriptProcess->exitCode()==0)
    {
        cleanUp();
    }
}

void Pipeline::stopPipeline()
{
    if(m_para_m->getpara_computingSystem_comboBox() == "killdevil")
    {
        QProcess* bkill_process = new::QProcess();
        bkill_process->start("bkill " + m_jobID);
        while (!bkill_process->waitForFinished())
        {
            sleep(1);
        }
    }

    m_mainScriptProcess->terminate();
}

