#include "AutoTractDerivedWindow.h"

/* Management of software tab: https://github.com/marie-cherel2013/NeosegPipeline*/

AutoTractDerivedWindow::AutoTractDerivedWindow()
{
    m_thread = new MainScriptThread();
    m_pipeline = new Pipeline();
    log_textEdit->setReadOnly(true);

    connect( this->actionSave_Parameter_Configuration, SIGNAL( triggered() ), SLOT( SaveParaConfigFile() ) );
    connect( this->actionLoad_Parameter_Configuration, SIGNAL( triggered() ), SLOT( LoadParaConfigFile() ) );
    connect( this->actionSave_Software_Configuration, SIGNAL( triggered() ), SLOT( SaveSoftConfigFile() ) );
    connect( this->actionLoad_Software_Configuration, SIGNAL( triggered() ), SLOT( LoadSoftConfigFile() ) );

    /*1st tab: Inputs*/
    // Select Parameters Signal Mapper
    QSignalMapper* selectParameters_signalMapper = new QSignalMapper(this);
    connect(selectParameters_signalMapper, SIGNAL(mapped(QString)), this, SLOT(selectParameters(QString)));

    // Enter Parameters Signal Mapper
    QSignalMapper* enterParameters_signalMapper = new QSignalMapper(this);
    connect(enterParameters_signalMapper, SIGNAL(mapped(QString)), this, SLOT(enterParameters(QString)));

    initializeParametersMap();
    QMap<QString, Parameters>::iterator it_parameters;
    for(it_parameters = m_parameters_map.begin(); it_parameters != m_parameters_map.end(); ++it_parameters)
    {
        QString name = it_parameters.key();
        Parameters parameters = it_parameters.value();

        selectParameters_signalMapper->setMapping(parameters.select_button, name);
        connect(parameters.select_button, SIGNAL(clicked()), selectParameters_signalMapper, SLOT(map()));

        enterParameters_signalMapper->setMapping(parameters.enter_lineEdit, name);
        connect(parameters.enter_lineEdit, SIGNAL(editingFinished()), enterParameters_signalMapper, SLOT(map()));
    }
    connect(output_dir_pushButton, SIGNAL(clicked()), this, SLOT(selectOutputDirectory()));
    connect(para_output_dir_lineEdit, SIGNAL(editingFinished()), this, SLOT(enterOutputDirectory()));

    /*2nd tab: reference tracts*/

    connect(tracts_dir_pushButton, SIGNAL(clicked()), this, SLOT(selectTractsPopulationDirectory()));
    connect(para_tracts_dir_lineEdit, SIGNAL(editingFinished()), this, SLOT(enterTractPopulationDirectory()));
    connect(para_ref_tracts_listWidget,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(selectTracts(QListWidgetItem*)));
    connect(check_all_tracts_pushButton, SIGNAL(clicked()), this, SLOT(checkAllTracts()));
    connect(uncheck_all_tracts_pushButton, SIGNAL(clicked()), this, SLOT(uncheckAllTracts()));

    /*3rd tab: software*/
    // Select Executable Signal Mapper
    QSignalMapper* selectExecutable_signalMapper = new QSignalMapper(this);
    connect(selectExecutable_signalMapper, SIGNAL(mapped(QString)), this, SLOT(selectExecutable(QString)));

    // Enter Executable Signal Mapper
    QSignalMapper* enterExecutable_signalMapper = new QSignalMapper(this);
    connect(enterExecutable_signalMapper, SIGNAL(mapped(QString)), this, SLOT(enterExecutable(QString)));

    // Reset Executable Signal Mapper
    QSignalMapper* resetExecutable_signalMapper = new QSignalMapper(this);
    connect(resetExecutable_signalMapper, SIGNAL(mapped(QString)), this, SLOT(resetExecutable(QString)));

    initializeExecutablesMap();
    QMap<QString, Executable>::iterator it_exec;
    for(it_exec = m_executables_map.begin(); it_exec != m_executables_map.end(); ++it_exec)
    {
        QString name = it_exec.key();
        Executable executable = it_exec.value();

        selectExecutable_signalMapper->setMapping(executable.select_button, name);
        connect(executable.select_button, SIGNAL(clicked()), selectExecutable_signalMapper, SLOT(map()));

        enterExecutable_signalMapper->setMapping(executable.enter_lineEdit, name);
        connect(executable.enter_lineEdit, SIGNAL(editingFinished()), enterExecutable_signalMapper, SLOT(map()));

        resetExecutable_signalMapper->setMapping(executable.reset_button, name);
        connect(executable.reset_button, SIGNAL(clicked()), resetExecutable_signalMapper, SLOT(map()));
    }

    /*4th tab: Registration*/
    connect(para_registration_type_comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(SyncUiToModelStructure()));
    connect(para_transformation_step_spinBox, SIGNAL(valueChanged(double)), this, SLOT(SyncUiToModelStructure()));
    connect(para_iterations_lineEdit, SIGNAL(editingFinished()), this, SLOT(SyncUiToModelStructure()));
    connect(para_similarity_metric_comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(SyncUiToModelStructure()));
    connect(para_similarity_parameter_spinBox, SIGNAL(valueChanged(int)), this, SLOT(SyncUiToModelStructure()));
    connect(para_gaussian_sigma_spinBox, SIGNAL(valueChanged(double)), this, SLOT(SyncUiToModelStructure()));


    /*5th tab: Tractography*/
    connect(para_dilation_radius_spinBox, SIGNAL(valueChanged(int)), this, SLOT(SyncUiToModelStructure()));
    connect(para_seedspacing_spinBox, SIGNAL(valueChanged(double)), this, SLOT(SyncUiToModelStructure()));
    connect(para_linearmeasure_spinBox, SIGNAL(valueChanged(double)), this, SLOT(SyncUiToModelStructure()));
    connect(para_minpathlength_spinBox, SIGNAL(valueChanged(double)), this, SLOT(SyncUiToModelStructure()));
    connect(para_maxpathlength_spinBox, SIGNAL(valueChanged(double)), this, SLOT(SyncUiToModelStructure()));
    connect(para_stoppingvalue_spinBox, SIGNAL(valueChanged(double)), this, SLOT(SyncUiToModelStructure()));
    connect(para_integrationsteplength_spinBox, SIGNAL(valueChanged(double)), this, SLOT(SyncUiToModelStructure()));
    connect(para_stoppingcurvature_spinBox, SIGNAL(valueChanged(double)), this, SLOT(SyncUiToModelStructure()));

    /*6th tab: Tract Processing Parameters*/
    connect(para_thresholdCSFmask_spinBox, SIGNAL(valueChanged(double)), this, SLOT(SyncUiToModelStructure()));
    connect(para_tractOverlapRatio_spinBox, SIGNAL(valueChanged(double)), this, SLOT(SyncUiToModelStructure()));
    connect(para_tractMaxDistThreshold_spinBox,SIGNAL(valueChanged(double)), this, SLOT(SyncUiToModelStructure()));


    /*7th tab: Classification*/
    connect(trafic_model_dir_pushButton, SIGNAL(clicked()), this, SLOT(selectTraficModelDirectory()));
    connect(para_trafic_model_dir_lineEdit, SIGNAL(editingFinished()), this, SLOT(enterTraficModelDirectory()));

    connect(para_enable_trafic_checkBox, SIGNAL(stateChanged(int)), this, SLOT(SyncUiToModelStructure()));
    connect(para_no_tract_output_trafic_checkBox, SIGNAL(stateChanged(int)), this, SLOT(SyncUiToModelStructure()));

    /*8th tab: Execution*/
    connect(runPipeline_pushButton, SIGNAL(clicked()), this, SLOT(runPipeline()));
    connect(stopPipeline_pushButton, SIGNAL(clicked()), this, SLOT(stopPipeline()));
    //connect(para_all_radioButton, SIGNAL(clicked()), this, SLOT(SyncUiToModelStructure()));
    //connect(para_singletract_radioButton, SIGNAL(clicked()), this, SLOT(SyncUiToModelStructure()));
    //connect(para_cleanup_checkBox, SIGNAL(clicked()), this, SLOT(SyncUiToModelStructure()));
    //connect(para_overwrite_checkBox, SIGNAL(clicked()), this, SLOT(SyncUiToModelStructure()));

    connect(para_nb_threads_spinBox, SIGNAL(valueChanged(int)), this, SLOT(SyncUiToModelStructure()));
    connect(para_nb_memory_registration_spinBox, SIGNAL(valueChanged(int)), this, SLOT(SyncUiToModelStructure()));

    connect(para_computingSystem_comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(changeComputingSystem()));
    connect(para_nbTractsProcessed_spinBox, SIGNAL(valueChanged(int)), this, SLOT(SyncUiToModelStructure()));

    connect(para_nb_memory_spinBox, SIGNAL(valueChanged(int)), this, SLOT(SyncUiToModelStructure()));
    connect(para_nbCores_spinBox, SIGNAL(valueChanged(int)), this, SLOT(SyncUiToModelStructure()));
}

void AutoTractDerivedWindow::closeEvent(QCloseEvent *event)
{
    QMessageBox::StandardButton resBtn = QMessageBox::question( this, "AutoTract",
                                                                tr("Do you want to kill the processes running?\n"),
                                                                QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes, QMessageBox::Yes);
    if(resBtn  == QMessageBox::Cancel)
    {
        event->ignore();
    }
    else if (resBtn == QMessageBox::Yes)
    {
        if(m_thread->isRunning())
        {
            m_thread->terminate();
        }

        event->accept();
    }
    else
    {
        event->accept();
    }
}

void AutoTractDerivedWindow::initSoftware()
{
    std::string soft = "dtiprocess";
    m_soft_m->setsoft_dtiprocess_lineEdit(QString::fromStdString( itksys::SystemTools::FindProgram( soft.c_str() ) ) ) ;

    soft = "DTI-Reg";
    m_soft_m->setsoft_DTIReg_lineEdit(QString::fromStdString( itksys::SystemTools::FindProgram( soft.c_str() ) ) ) ;

    soft = "FiberPostProcess";
    m_soft_m->setsoft_FiberPostProcess_lineEdit(QString::fromStdString( itksys::SystemTools::FindProgram( soft.c_str() ) ) ) ;

    soft = "fiberprocess";
    m_soft_m->setsoft_fiberprocess_lineEdit(QString::fromStdString( itksys::SystemTools::FindProgram( soft.c_str() ) ) ) ;

    soft = "ImageMath";
    m_soft_m->setsoft_ImageMath_lineEdit(QString::fromStdString( itksys::SystemTools::FindProgram( soft.c_str() ) ) ) ;

    soft = "ResampleDTIlogEuclidean";
    m_soft_m->setsoft_ResampleDTIlogEuclidean_lineEdit(QString::fromStdString( itksys::SystemTools::FindProgram( soft.c_str() ) ) ) ;

    soft = "MaurerDistanceTransform";
    m_soft_m->setsoft_MDT_lineEdit(QString::fromStdString( itksys::SystemTools::FindProgram( soft.c_str() ) ) ) ;

    soft = "polydatatransform";
    m_soft_m->setsoft_polydatatransform_lineEdit(QString::fromStdString( itksys::SystemTools::FindProgram( soft.c_str() ) ) ) ;

    soft = "python";
    m_soft_m->setsoft_python_lineEdit(QString::fromStdString( itksys::SystemTools::FindProgram( soft.c_str() ) ) ) ;

    soft = "TractographyLabelMapSeeding";
    m_soft_m->setsoft_TractographyLabelMapSeeding_lineEdit(QString::fromStdString( itksys::SystemTools::FindProgram( soft.c_str() ) ) ) ;

    soft = "unu";
    m_soft_m->setsoft_unu_lineEdit(QString::fromStdString( itksys::SystemTools::FindProgram( soft.c_str() ) ) ) ;

    soft = "Slicer";
    m_soft_m->setsoft_slicer_lineEdit(QString::fromStdString( itksys::SystemTools::FindProgram( soft.c_str() ) ) ) ;

    soft = "ANTS";
    m_soft_m->setsoft_ANTS_lineEdit(QString::fromStdString( itksys::SystemTools::FindProgram( soft.c_str() ) ) ) ;

    soft = "ITKTransformTools";
    m_soft_m->setsoft_ITKTransformTools_lineEdit(QString::fromStdString( itksys::SystemTools::FindProgram( soft.c_str() ) ) ) ;

    soft = "docker";
    m_soft_m->setsoft_docker_lineEdit(QString::fromStdString( itksys::SystemTools::FindProgram( soft.c_str() ) ) );
}


void AutoTractDerivedWindow::checkAllTracts()
{
    std::map<std::pair<unsigned long,QString>,bool> map = m_para_m->getpara_ref_tracts_listWidget();
    std::map<std::pair<unsigned long,QString>,bool>::iterator it = map.begin();
    for( int count = 0 ; it != map.end() ; count++ , it++ )
    {
        map[it->first] = true;
    }
    m_para_m->setpara_ref_tracts_listWidget( map );
    SyncModelStructureToUi();
}

void AutoTractDerivedWindow::uncheckAllTracts()
{
    std::map<std::pair<unsigned long,QString>,bool> map = m_para_m->getpara_ref_tracts_listWidget();
    std::map<std::pair<unsigned long,QString>,bool>::iterator it = map.begin();
    for( int count = 0 ; it != map.end() ; count++ , it++ )
    {
        map[it->first] = false;
    }
    m_para_m->setpara_ref_tracts_listWidget( map );
    SyncModelStructureToUi();

}

void AutoTractDerivedWindow::runPipeline()
{
    runPipeline_pushButton->setEnabled(false);
    stopPipeline_pushButton->setEnabled(true);
    SyncUiToModelStructure();

    m_pipeline->setPipelineParameters(m_para_m);
    m_pipeline->setPipelineSoftwares(m_soft_m);
    initializePipelineLogging();
    m_pipeline->writePipeline();

    m_thread->setPipeline(m_pipeline);
    m_thread->start();
    //runPipeline_pushButton->setEnabled(true);
    //stopPipeline_pushButton->setEnabled(false);
}

void AutoTractDerivedWindow::stopPipeline()
{
    m_thread->terminate();

    runPipeline_pushButton->setEnabled(true);
    runPipeline_action->setEnabled(true);

    stopPipeline_pushButton->setEnabled(false);
    stopPipeline_action->setEnabled(false);
}

void AutoTractDerivedWindow::initializePipelineLogging()
{
    log_textEdit->show();
    log_textEdit->clear();
    log_textEdit->setMaximumBlockCount(500);
    // Log path
    QDir* output_dir = new QDir(m_para_m->getpara_output_dir_lineEdit());
    QFileInfo fi(m_para_m->getpara_output_dir_lineEdit());
    QString base = fi.baseName();
    QString log_path = output_dir->filePath(base + ".log");

    // Log File
    QFile* log_file = new::QFile(log_path);
    log_file->open(QIODevice::ReadWrite);
    m_log_textStream = new::QTextStream(log_file);

    // QFileSystemWatcher
    QFileSystemWatcher* log_watcher = new::QFileSystemWatcher(this);
    log_watcher->addPath(log_path);
    connect(log_watcher, SIGNAL(fileChanged(QString)), this, SLOT(printPipelineLog()));
}

void AutoTractDerivedWindow::printPipelineLog()
{   
    QScrollBar *scrollBar = log_textEdit->verticalScrollBar();

    QString line = m_log_textStream->readAll();

    if(scrollBar->value() == scrollBar->maximum())
    {
        log_textEdit->insertPlainText(line);
        scrollBar->setValue(scrollBar->maximum());
    }
    else
    {
        log_textEdit->insertPlainText(line);
    }
}

void AutoTractDerivedWindow::SaveParaConfigFile()
{
    QString m_DialogDir;
    QString filename = QFileDialog::getSaveFileName( this , "Save Parameter Configuration File" , m_DialogDir , "XML files (*.xml)" );
    if( filename != "" )
    {
        QFileInfo fi( filename ) ;
        m_DialogDir = fi.dir().absolutePath() ;
        Save_Parameter_Configuration( filename.toStdString() );
    }

}

void AutoTractDerivedWindow::LoadParaConfigFile()
{
    QString m_DialogDir;
    QString filename = QFileDialog::getOpenFileName( this , "Open Parameter Configuration File" , m_DialogDir , "XML files (*.xml)" );
    if( filename != "" )
    {
        QFileInfo fi( filename ) ;
        m_DialogDir = fi.dir().absolutePath() ;
        Load_Parameter_Configuration( filename.toStdString() );
    }
}

void AutoTractDerivedWindow::SaveSoftConfigFile()
{
    QString m_DialogDir;
    QString filename = QFileDialog::getSaveFileName( this , "Save Software Configuration File" , m_DialogDir , "XML files (*.xml)" );
    if( filename != "" )
    {
        QFileInfo fi( filename ) ;
        m_DialogDir = fi.dir().absolutePath() ;
        Save_Software_Configuration( filename.toStdString() );
    }

}

void AutoTractDerivedWindow::LoadSoftConfigFile()
{
    QString m_DialogDir;
    QString filename = QFileDialog::getOpenFileName( this , "Open Software Configuration File" , m_DialogDir , "XML files (*.xml)" );
    if( filename != "" )
    {
        QFileInfo fi( filename ) ;
        m_DialogDir = fi.dir().absolutePath() ;
        Load_Software_Configuration( filename.toStdString() );
    }

}

void AutoTractDerivedWindow::initializeExecutablesMap()
{
    Executable DTIReg = {DTIReg_pushButton, soft_DTIReg_lineEdit, reset_DTIReg_pushButton};
    m_executables_map.insert("DTIReg", DTIReg);

    Executable fiberprocess = {fiberprocess_pushButton, soft_fiberprocess_lineEdit, reset_fiberprocess_pushButton};
    m_executables_map.insert("fiberprocess", fiberprocess);

    Executable ResampleDTIlogEuclidean = {ResampleDTIlogEuclidean_pushButton, soft_ResampleDTIlogEuclidean_lineEdit, reset_ResampleDTIlogEuclidean_pushButton};
    m_executables_map.insert("ResampleDTIlogEuclidean", ResampleDTIlogEuclidean);

    Executable ImageMath = {ImageMath_pushButton, soft_ImageMath_lineEdit, reset_ImageMath_pushButton};
    m_executables_map.insert("ImageMath", ImageMath);

    Executable TractographyLabelMapSeeding = {TractographyLabelMapSeeding_pushButton, soft_TractographyLabelMapSeeding_lineEdit, reset_TractographyLabelMapSeeding_pushButton};
    m_executables_map.insert("TractographyLabelMapSeeding", TractographyLabelMapSeeding);

    Executable FiberPostProcess = {FiberPostProcess_pushButton, soft_FiberPostProcess_lineEdit, reset_FiberPostProcess_pushButton};
    m_executables_map.insert("FiberPostProcess", FiberPostProcess);

    Executable polydatatransform = {polydatattransform_pushButton, soft_polydatatransform_lineEdit, reset_polydatatransform_pushButton};
    m_executables_map.insert("polydatatransform", polydatatransform);

    Executable unu = {unu_pushButton, soft_unu_lineEdit, reset_unu_pushButton};
    m_executables_map.insert("unu", unu);

    Executable MDT = {MDT_pushButton, soft_MDT_lineEdit, reset_MDT_pushButton};
    m_executables_map.insert("MDT", MDT);

    Executable python = {python_pushButton, soft_python_lineEdit, reset_python_pushButton};
    m_executables_map.insert("python", python);

    Executable dtiprocess = {dtiprocess_pushButton, soft_dtiprocess_lineEdit, reset_dtiprocess_pushButton};
    m_executables_map.insert("dtiprocess", dtiprocess);

    Executable Slicer = {slicer_pushButton, soft_slicer_lineEdit, reset_slicer_pushButton};
    m_executables_map.insert("Slicer", Slicer);

    Executable ANTS = {ANTS_pushButton, soft_ANTS_lineEdit, reset_ANTS_pushButton};
    m_executables_map.insert("ANTS", ANTS);

    Executable ITKTransformTools = {ITKTransformTools_pushButton, soft_ITKTransformTools_lineEdit, reset_ITKTransformTools_pushButton};
    m_executables_map.insert("ITKTransformTools", ITKTransformTools);

    Executable Docker = {docker_pushButton, soft_docker_lineEdit, reset_docker_pushButton};
    m_executables_map.insert("docker", Docker);
}

void AutoTractDerivedWindow::selectExecutable(QString executable_name)
{
    Executable executable = m_executables_map[executable_name];
    QString executable_path = (executable.enter_lineEdit)->text();
    QString dir_path = "";

    if(!(executable_path.isEmpty()))
    {
        dir_path = (QFileInfo(executable_path).dir()).absolutePath();
    }

    executable_path = QFileDialog::getOpenFileName(this, tr("Select executable"), dir_path);
    if(!executable_path.isEmpty())
    {
        (executable.enter_lineEdit)->setText(executable_path) ;
    }
    SyncUiToModelStructure();
}

void AutoTractDerivedWindow::enterExecutable(QString executable_name)
{
    Executable executable = m_executables_map[executable_name];
    QString executable_path = (executable.enter_lineEdit)->text();
    (executable.enter_lineEdit)->setText(executable_path) ;
    SyncUiToModelStructure();
}

void AutoTractDerivedWindow::resetExecutable(QString executable_name)
{
    if(executable_name == "dtiprocess")
    {
        m_soft_m->setsoft_dtiprocess_lineEdit(QString::fromStdString( itksys::SystemTools::FindProgram( executable_name.toStdString().c_str() ) ) ) ;
    }
    if(executable_name == "DTIReg")
    {
        std::string soft = "DTI-Reg";
        m_soft_m->setsoft_DTIReg_lineEdit(QString::fromStdString( itksys::SystemTools::FindProgram( soft.c_str() ) ) ) ;
    }
    if(executable_name == "FiberPostProcess")
    {
        m_soft_m->setsoft_FiberPostProcess_lineEdit(QString::fromStdString( itksys::SystemTools::FindProgram( executable_name.toStdString().c_str() ) ) ) ;
    }
    if(executable_name == "fiberprocess")
    {
        m_soft_m->setsoft_fiberprocess_lineEdit(QString::fromStdString( itksys::SystemTools::FindProgram( executable_name.toStdString().c_str() ) ) ) ;
    }
    if(executable_name == "ImageMath")
    {
        m_soft_m->setsoft_ImageMath_lineEdit(QString::fromStdString( itksys::SystemTools::FindProgram( executable_name.toStdString().c_str() ) ) ) ;
    }
    if(executable_name == "ResampleDTIlogEuclidean")
    {
        m_soft_m->setsoft_ResampleDTIlogEuclidean_lineEdit(QString::fromStdString( itksys::SystemTools::FindProgram( executable_name.toStdString().c_str() ) ) ) ;
    }
    if(executable_name == "MDT")
    {
        std::string soft = "MaurerDistanceTransform";
        m_soft_m->setsoft_MDT_lineEdit(QString::fromStdString( itksys::SystemTools::FindProgram( soft.c_str() ) ) ) ;
    }
    if(executable_name == "polydatatransform")
    {
        m_soft_m->setsoft_polydatatransform_lineEdit(QString::fromStdString( itksys::SystemTools::FindProgram( executable_name.toStdString().c_str() ) ) ) ;
    }
    if(executable_name == "python")
    {
        m_soft_m->setsoft_python_lineEdit(QString::fromStdString( itksys::SystemTools::FindProgram( executable_name.toStdString().c_str() ) ) ) ;
    }
    if(executable_name == "TractographyLabelMapSeeding")
    {
        m_soft_m->setsoft_TractographyLabelMapSeeding_lineEdit(QString::fromStdString( itksys::SystemTools::FindProgram( executable_name.toStdString().c_str() ) ) ) ;
    }
    if(executable_name == "unu")
    {
        m_soft_m->setsoft_unu_lineEdit(QString::fromStdString( itksys::SystemTools::FindProgram( executable_name.toStdString().c_str() ) ) ) ;
    }
    if(executable_name == "Slicer")
    {
        m_soft_m->setsoft_slicer_lineEdit(QString::fromStdString( itksys::SystemTools::FindProgram( executable_name.toStdString().c_str() ) ) ) ;
    }
    if(executable_name == "ANTS")
    {
        m_soft_m->setsoft_ANTS_lineEdit(QString::fromStdString( itksys::SystemTools::FindProgram( executable_name.toStdString().c_str() ) ) ) ;
    }
    if(executable_name == "ITKTransformTools")
    {
        m_soft_m->setsoft_ITKTransformTools_lineEdit(QString::fromStdString( itksys::SystemTools::FindProgram( executable_name.toStdString().c_str() ) ) ) ;
    }
    if(executable_name == "docker")
    {
        m_soft_m->setsoft_docker_lineEdit(QString::fromStdString( itksys::SystemTools::FindProgram( executable_name.toStdString().c_str() ) ) ) ;
    }

    SyncModelStructureToUi("soft");
}

void AutoTractDerivedWindow::initializeParametersMap()
{
    Parameters inputDTIatlas_dir = { inputDTIatlas_pushButton, para_inputDTIatlas_lineEdit};
    m_parameters_map.insert("inputDTIatlas_dir", inputDTIatlas_dir);

    Parameters inputWMmask_dir = {inputWMmask_pushButton, para_inputWMmask_lineEdit};
    m_parameters_map.insert("inputWMmask_dir", inputWMmask_dir);

    Parameters inputCSFmask_dir = {inputCSFmask_pushButton, para_inputCSFmask_lineEdit};
    m_parameters_map.insert("inputCSFmask_dir", inputCSFmask_dir);

    Parameters refDTIatlas_dir = {refDTIatlas_pushButton, para_refDTIatlas_lineEdit};
    m_parameters_map.insert("refDTIatlas_dir", refDTIatlas_dir);

    Parameters displacementField_dir = {inputdisplacementField_pushButton, para_displacementField_lineEdit};
    m_parameters_map.insert("displacementField", displacementField_dir);
}

void AutoTractDerivedWindow::selectParameters(QString parameters_name)
{
    Parameters parameters = m_parameters_map[parameters_name];
    QString parameters_path = (parameters.enter_lineEdit)->text();
    QString dir_path = "";

    if(!(parameters_path.isEmpty()))
    {
        dir_path = (QFileInfo(parameters_path).dir()).absolutePath();
    }

    parameters_path = QFileDialog::getOpenFileName(this, tr("Select path"), dir_path);
    if(!parameters_path.isEmpty())
    {
        (parameters.enter_lineEdit)->setText(parameters_path) ;
    }
    SyncUiToModelStructure();
}

void AutoTractDerivedWindow::enterParameters(QString parameters_name)
{
    Parameters parameters = m_parameters_map[parameters_name];
    QString parameters_path = (parameters.enter_lineEdit)->text();
    (parameters.enter_lineEdit)->setText(parameters_path) ;
    SyncUiToModelStructure();
}

void AutoTractDerivedWindow::selectTractsPopulationDirectory()
{
    QString tractsPopulationDirectory = QFileDialog::getExistingDirectory (this, tr("Open Directory"), para_tracts_dir_lineEdit->text(), QFileDialog::ShowDirsOnly);
    para_tracts_dir_lineEdit->setText(tractsPopulationDirectory);
    m_tractPopulationDirectory = tractsPopulationDirectory;
    UpdateTractPopulationDirectoryDisplay() ;
    SyncUiToModelStructure();
}

//***** Select/Unselect Tracts *****//
void AutoTractDerivedWindow::selectTracts(QListWidgetItem* item)
{
    if(item->checkState())
    {
        m_selectedTracts << item->text();
    }
    else
    {
        m_selectedTracts.removeAt(m_selectedTracts.indexOf(item->text()));
    }
    SyncUiToModelStructure();
}

void AutoTractDerivedWindow::UpdateTractPopulationDirectoryDisplay()
{
    checkTracts() ;
    displayTracts() ;
    checkSelectedTracts() ;
}
//***** Display Tracts *****//
void AutoTractDerivedWindow::displayTracts()
{
    para_ref_tracts_listWidget->clear();
    QStringList::const_iterator it;
    int count = 0;
    for (it = m_selectedTracts.constBegin(); it != m_selectedTracts.constEnd(); ++it)
    {
        QListWidgetItem* item = new QListWidgetItem(*it, para_ref_tracts_listWidget);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable | Qt::ItemIsSelectable);
        item->setCheckState(Qt::Unchecked);count++;
    }
}

void AutoTractDerivedWindow::enterTractPopulationDirectory()
{
    QString tractPopulationDirectory = para_tracts_dir_lineEdit->text();
    if(!tractPopulationDirectory.isEmpty())
    {
        m_tractPopulationDirectory =  para_tracts_dir_lineEdit->text() ;
    }
    UpdateTractPopulationDirectoryDisplay() ;
    SyncUiToModelStructure();
}

void AutoTractDerivedWindow::selectTraficModelDirectory()
{
    // QString traficModelDirectory = QFileDialog::getExistingDirectory (this, tr("Open Directory"), para_trafic_model_dir_lineEdit->text(), QFileDialog::ShowDirsOnly);
    QString traficModelDirectory = QFileDialog::getOpenFileName (this, tr("Open File"), para_trafic_model_dir_lineEdit->text());
    para_trafic_model_dir_lineEdit->setText(traficModelDirectory);
    m_traficModelDirectory = traficModelDirectory;
    SyncUiToModelStructure();
}

void AutoTractDerivedWindow::enterTraficModelDirectory()
{
    QString traficModelDirectory = para_trafic_model_dir_lineEdit->text();
    if(!traficModelDirectory.isEmpty())
    {
        m_traficModelDirectory =  para_trafic_model_dir_lineEdit->text() ;
    }
    SyncUiToModelStructure();
}


void AutoTractDerivedWindow::checkSelectedTracts()
{
    QStringList::const_iterator it;
    for (it = m_selectedTracts.constBegin(); it != m_selectedTracts.constEnd(); ++it)
    {
        QList<QListWidgetItem *> items = para_ref_tracts_listWidget->findItems(*it, Qt::MatchExactly);

        QList<QListWidgetItem *>::iterator item;
        for(item=items.begin(); item!=items.end(); ++item)
        {
            (*item)->setCheckState(Qt::Checked);
        }
    }
}

//***** Checking Tracts *****//
void AutoTractDerivedWindow::checkTracts()
{
    QDir* m_tractPopulation_dir = new QDir(para_tracts_dir_lineEdit ->text());
    QStringList tractPopulation_list = m_tractPopulation_dir->entryList( QDir::Files | QDir::NoSymLinks);

    QStringList::const_iterator it;
    for (it = tractPopulation_list.constBegin(); it != tractPopulation_list.constEnd(); ++it)
    {
        if((*it).endsWith(".vtk") || (*it).endsWith(".vtp"))
        {
            m_selectedTracts << *it;
        }
    }
}

void AutoTractDerivedWindow::selectOutputDirectory()
{
    QString outputDirectory = QFileDialog::getExistingDirectory (this, tr("Open Directory"), para_output_dir_lineEdit->text(), QFileDialog::ShowDirsOnly);
    m_para_m->setpara_output_dir_lineEdit( outputDirectory );
    SyncModelStructureToUi();
}

void AutoTractDerivedWindow::enterOutputDirectory()
{
    QString outputDirectory = para_output_dir_lineEdit->text();
    if(!outputDirectory.isEmpty())
    {
        SyncUiToModelStructure();
    }
}

void AutoTractDerivedWindow::changeComputingSystem()
{
    SyncUiToModelStructure();
    std::vector<QString> ref_tracts;
    std::map<std::pair<unsigned long,QString>,bool> ref_tracts_list = m_para_m->getpara_ref_tracts_listWidget();
    std::map<std::pair<unsigned long,QString>,bool>::iterator it;
    for (it = ref_tracts_list.begin(); it!= ref_tracts_list.end(); it++)
    {
        if(it->second == 1)
        {
            ref_tracts.push_back( it->first.second );
        }
    }
    int nb_tracts = ref_tracts.size();
    if(m_para_m->getpara_computingSystem_comboBox() == "killdevil")
    {
        para_nbTractsProcessed_spinBox->setValue(nb_tracts);
        para_nbTractsProcessed_spinBox->setDisabled(true);
    }
    else if(m_para_m->getpara_computingSystem_comboBox() == "local")
    {
        para_nbTractsProcessed_spinBox->setEnabled(true);
    }
}
