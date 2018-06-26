#include "AutoTractWindow.h"
#include <iostream>
AutoTractWindow::AutoTractWindow(QWidget * parent , Qt::WFlags f): QMainWindow( parent , f )
{
    setupUi( this ) ;
    m_sync = 0 ;
}

void AutoTractWindow::SyncUiToModelStructure()
{
    if( m_sync == 1)
    {
        return;
    }
    m_sync = 1;

    /*1st tab: inputs*/
    m_para_m->setpara_inputDTIatlas_lineEdit( para_inputDTIatlas_lineEdit->text() ) ;
    m_para_m->setpara_inputWMmask_lineEdit( para_inputWMmask_lineEdit->text() );
    m_para_m->setpara_inputCSFmask_lineEdit( para_inputCSFmask_lineEdit->text() );
    m_para_m->setpara_output_dir_lineEdit( para_output_dir_lineEdit->text() );
    m_para_m->setpara_refDTIatlas_lineEdit( para_refDTIatlas_lineEdit->text() );
    m_para_m->setpara_displacementField_lineEdit( para_displacementField_lineEdit->text() );

    /*2nd tab: reference tracts*/
    m_para_m->setpara_tracts_dir_lineEdit( para_tracts_dir_lineEdit->text() );
    std::map<std::pair<unsigned long,QString>,bool> itemList ;
    for( int i = 0 ; i < para_ref_tracts_listWidget->count() ; i++ )
    {
        QListWidgetItem *currentItem = para_ref_tracts_listWidget->item(i) ;
        std::pair<unsigned long,QString> attribute ;
        attribute = std::make_pair(i,currentItem->text());
        itemList[ attribute ] = static_cast<bool>(currentItem->checkState() ) ;
    }
    m_para_m->setpara_ref_tracts_listWidget(itemList) ;

    /*3rd tab: software*/
    m_soft_m->setsoft_dtiprocess_lineEdit( soft_dtiprocess_lineEdit->text() );
    m_soft_m->setsoft_DTIReg_lineEdit( soft_DTIReg_lineEdit->text() );
    m_soft_m->setsoft_FiberPostProcess_lineEdit( soft_FiberPostProcess_lineEdit->text() );
    m_soft_m->setsoft_fiberprocess_lineEdit( soft_fiberprocess_lineEdit->text() );
    m_soft_m->setsoft_ImageMath_lineEdit( soft_ImageMath_lineEdit->text() );
    m_soft_m->setsoft_ResampleDTIlogEuclidean_lineEdit( soft_ResampleDTIlogEuclidean_lineEdit->text() );
    m_soft_m->setsoft_MDT_lineEdit( soft_MDT_lineEdit->text() );
    m_soft_m->setsoft_polydatatransform_lineEdit( soft_polydatatransform_lineEdit->text() );
    m_soft_m->setsoft_python_lineEdit( soft_python_lineEdit->text() );
    m_soft_m->setsoft_TractographyLabelMapSeeding_lineEdit( soft_TractographyLabelMapSeeding_lineEdit->text() );
    m_soft_m->setsoft_unu_lineEdit( soft_unu_lineEdit->text() );
    m_soft_m->setsoft_slicer_lineEdit( soft_slicer_lineEdit->text() );
    m_soft_m->setsoft_ANTS_lineEdit( soft_ANTS_lineEdit->text() );
    m_soft_m->setsoft_ITKTransformTools_lineEdit( soft_ITKTransformTools_lineEdit->text() );
    m_soft_m->setsoft_docker_lineEdit( soft_docker_lineEdit->text() );

    /*4th tab: registration*/
    m_para_m->setpara_registration_type_comboBox( para_registration_type_comboBox->currentText() );
    m_para_m->setpara_transformation_step_spinBox( para_transformation_step_spinBox->value() );
    m_para_m->setpara_iterations_lineEdit( para_iterations_lineEdit->text() );
    m_para_m->setpara_similarity_metric_comboBox( para_similarity_metric_comboBox->currentText() );
    m_para_m->setpara_gaussian_sigma_spinBox( para_gaussian_sigma_spinBox->value() );
    m_para_m->setpara_similarity_parameter_spinBox( para_similarity_parameter_spinBox->value() );

    /*5th tab*/
    m_para_m->setpara_dilation_radius_spinBox( para_dilation_radius_spinBox->value() );
    m_para_m->setpara_seedspacing_spinBox( para_seedspacing_spinBox->value() );
    m_para_m->setpara_linearmeasure_spinBox( para_linearmeasure_spinBox->value() );
    m_para_m->setpara_minpathlength_spinBox( para_minpathlength_spinBox->value() );
    m_para_m->setpara_maxpathlength_spinBox( para_maxpathlength_spinBox->value() );
    m_para_m->setpara_stoppingvalue_spinBox( para_stoppingvalue_spinBox->value() );
    m_para_m->setpara_stoppingcurvature_spinBox( para_stoppingcurvature_spinBox->value() );
    m_para_m->setpara_integrationsteplength_spinBox( para_integrationsteplength_spinBox->value() );

    /*6th tab*/
    m_para_m->setpara_thresholdCSFmask_spinBox( para_thresholdCSFmask_spinBox->value() );
    m_para_m->setpara_tractOverlapRatio_spinBox( para_tractOverlapRatio_spinBox->value() );
    m_para_m->setpara_tractMaxDistThreshold_spinBox( para_tractMaxDistThreshold_spinBox->value() );

    /*7th tab: classification*/
    m_para_m->setpara_trafic_model_dir_lineEdit( para_trafic_model_dir_lineEdit->text() );
    m_para_m->setpara_enable_trafic_checkBox(para_enable_trafic_checkBox->isChecked());
    m_para_m->setpara_no_tract_output_trafic_checkBox(para_no_tract_output_trafic_checkBox->isChecked());
        
    /*8th tab: Execution*/
    //m_para_m->setpara_all_radioButton( para_all_radioButton->isChecked() );
    //m_para_m->setpara_singletract_radioButton( para_singletract_radioButton->isChecked() );
    m_para_m->setpara_computingSystem_comboBox( para_computingSystem_comboBox->currentText() );
    m_para_m->setpara_nbCores_spinBox( para_nbCores_spinBox->value() );
    m_para_m->setpara_nbTractsProcessed_spinBox( para_nbTractsProcessed_spinBox->value() );
    //m_para_m->setpara_cleanup_checkBox( para_cleanup_checkBox->isChecked() );
    //m_para_m->setpara_overwrite_checkBox( para_overwrite_checkBox->isChecked() );
   // /m_para_m->setpara_singletract_comboBox( para_singletract_comboBox->currentText() );
    m_para_m->setpara_nb_memory_spinBox( para_nb_memory_spinBox->value() );
    m_para_m->setpara_nb_threads_spinBox( para_nb_threads_spinBox->value() );
    m_para_m->setpara_nb_memory_registration_spinBox( para_nb_memory_registration_spinBox->value() );

    m_sync = 0 ;
}

void AutoTractWindow::SyncUiToModelStructure( QString prefix )
{
    if( m_sync == 1)
    {
        return;
    }
    m_sync = 1;
    if(prefix =="para")
    {
        /*1st tab: inputs*/
        m_para_m->setpara_inputDTIatlas_lineEdit( para_inputDTIatlas_lineEdit->text() ) ;
        m_para_m->setpara_inputWMmask_lineEdit( para_inputWMmask_lineEdit->text() );
        m_para_m->setpara_inputCSFmask_lineEdit( para_inputCSFmask_lineEdit->text() );
        m_para_m->setpara_output_dir_lineEdit( para_output_dir_lineEdit->text() );
        m_para_m->setpara_refDTIatlas_lineEdit( para_refDTIatlas_lineEdit->text() );
        m_para_m->setpara_displacementField_lineEdit( para_displacementField_lineEdit->text() );

        /*2nd tab: reference tracts*/
        m_para_m->setpara_tracts_dir_lineEdit( para_tracts_dir_lineEdit->text() );
        std::map<std::pair<unsigned long,QString>,bool> itemList ;
        for( int i = 0 ; i < para_ref_tracts_listWidget->count() ; i++ )
        {
            QListWidgetItem *currentItem = para_ref_tracts_listWidget->item(i) ;
            std::pair<unsigned long,QString> attribute ;
            attribute = std::make_pair(i,currentItem->text());
            itemList[ attribute ] = static_cast<bool>(currentItem->checkState() ) ;
        }
        m_para_m->setpara_ref_tracts_listWidget(itemList) ;

        /*4th tab: registration*/
        m_para_m->setpara_registration_type_comboBox( para_registration_type_comboBox->currentText() );
        m_para_m->setpara_transformation_step_spinBox( para_transformation_step_spinBox->value() );
        m_para_m->setpara_iterations_lineEdit( para_iterations_lineEdit->text() );
        m_para_m->setpara_similarity_metric_comboBox( para_similarity_metric_comboBox->currentText() );
        m_para_m->setpara_gaussian_sigma_spinBox( para_gaussian_sigma_spinBox->value() );
        m_para_m->setpara_similarity_parameter_spinBox( para_similarity_parameter_spinBox->value() );

        /*5th tab*/
        m_para_m->setpara_dilation_radius_spinBox( para_dilation_radius_spinBox->value() );
        m_para_m->setpara_seedspacing_spinBox( para_seedspacing_spinBox->value() );
        m_para_m->setpara_linearmeasure_spinBox( para_linearmeasure_spinBox->value() );
        m_para_m->setpara_minpathlength_spinBox( para_minpathlength_spinBox->value() );
        m_para_m->setpara_maxpathlength_spinBox( para_maxpathlength_spinBox->value() );
        m_para_m->setpara_stoppingvalue_spinBox( para_stoppingvalue_spinBox->value() );
        m_para_m->setpara_stoppingcurvature_spinBox( para_stoppingcurvature_spinBox->value() );
        m_para_m->setpara_integrationsteplength_spinBox( para_integrationsteplength_spinBox->value() );

        /*6th tab*/
        m_para_m->setpara_thresholdCSFmask_spinBox( para_thresholdCSFmask_spinBox->value() );
        m_para_m->setpara_tractOverlapRatio_spinBox( para_tractOverlapRatio_spinBox->value() );
        m_para_m->setpara_tractMaxDistThreshold_spinBox( para_tractMaxDistThreshold_spinBox->value() );

        /*7th tab: classification*/
        m_para_m->setpara_trafic_model_dir_lineEdit( para_trafic_model_dir_lineEdit->text() );
        m_para_m->setpara_enable_trafic_checkBox(para_enable_trafic_checkBox->isChecked());
        m_para_m->setpara_no_tract_output_trafic_checkBox(para_no_tract_output_trafic_checkBox->isChecked());

        /*8th tab: Execution*/
        //m_para_m->setpara_all_radioButton( para_all_radioButton->isChecked() );
        //m_para_m->setpara_singletract_radioButton( para_singletract_radioButton->isChecked() );
        m_para_m->setpara_computingSystem_comboBox( para_computingSystem_comboBox->currentText() );
        m_para_m->setpara_nbCores_spinBox( para_nbCores_spinBox->value() );
        m_para_m->setpara_nbTractsProcessed_spinBox( para_nbTractsProcessed_spinBox->value() );
        //m_para_m->setpara_cleanup_checkBox( para_cleanup_checkBox->isChecked() );
        //m_para_m->setpara_overwrite_checkBox( para_overwrite_checkBox->isChecked() );
       // m_para_m->setpara_singletract_comboBox( para_singletract_comboBox->currentText() );
        m_para_m->setpara_nb_memory_spinBox( para_nb_memory_spinBox->value() );
        m_para_m->setpara_nb_threads_spinBox( para_nb_threads_spinBox->value() );
        m_para_m->setpara_nb_memory_registration_spinBox( para_nb_memory_registration_spinBox->value() );
    }

    if(prefix == "soft")
    {
        /*3rd tab: software*/
        m_soft_m->setsoft_dtiprocess_lineEdit( soft_dtiprocess_lineEdit->text() );
        m_soft_m->setsoft_DTIReg_lineEdit( soft_DTIReg_lineEdit->text() );
        m_soft_m->setsoft_FiberPostProcess_lineEdit( soft_FiberPostProcess_lineEdit->text() );
        m_soft_m->setsoft_fiberprocess_lineEdit( soft_fiberprocess_lineEdit->text() );
        m_soft_m->setsoft_ImageMath_lineEdit( soft_ImageMath_lineEdit->text() );
        m_soft_m->setsoft_ResampleDTIlogEuclidean_lineEdit( soft_ResampleDTIlogEuclidean_lineEdit->text() );
        m_soft_m->setsoft_MDT_lineEdit( soft_MDT_lineEdit->text() );
        m_soft_m->setsoft_polydatatransform_lineEdit( soft_polydatatransform_lineEdit->text() );
        m_soft_m->setsoft_python_lineEdit( soft_python_lineEdit->text() );
        m_soft_m->setsoft_TractographyLabelMapSeeding_lineEdit( soft_TractographyLabelMapSeeding_lineEdit->text() );
        m_soft_m->setsoft_unu_lineEdit( soft_unu_lineEdit->text() );
        m_soft_m->setsoft_slicer_lineEdit( soft_slicer_lineEdit->text() );
        m_soft_m->setsoft_ANTS_lineEdit( soft_ANTS_lineEdit->text() );
        m_soft_m->setsoft_ITKTransformTools_lineEdit( soft_ITKTransformTools_lineEdit->text() );
        m_soft_m->setsoft_docker_lineEdit( soft_docker_lineEdit->text() );

    }
    m_sync = 0 ;

}

void AutoTractWindow::SyncModelStructureToUi()
{
    if( m_sync == 1)
    {
        return;
    }
    m_sync = 1;
    /*1st tab: inputs*/
    para_inputDTIatlas_lineEdit->setText( m_para_m->getpara_inputDTIatlas_lineEdit() );
    para_inputWMmask_lineEdit->setText( m_para_m->getpara_inputWMmask_lineEdit() );
    para_inputCSFmask_lineEdit->setText( m_para_m->getpara_inputCSFmask_lineEdit() );
    para_output_dir_lineEdit->setText( m_para_m->getpara_output_dir_lineEdit() );
    para_refDTIatlas_lineEdit->setText( m_para_m->getpara_refDTIatlas_lineEdit() );
    para_displacementField_lineEdit->setText( m_para_m->getpara_displacementField_lineEdit() );

    /*2nd tab: reference tracts*/
    para_tracts_dir_lineEdit->setText( m_para_m->getpara_tracts_dir_lineEdit() );
    std::map<std::pair<unsigned long,QString>,bool> itemList ;
    itemList = m_para_m->getpara_ref_tracts_listWidget() ;
    para_ref_tracts_listWidget->clear() ;
    for( std::map<std::pair<unsigned long,QString>,bool>::iterator it = itemList.begin() ; it != itemList.end() ; it++ )
    {
std::cout<<it->first.first<<std::endl;
std::cout<<it->first.second.toStdString()<<std::endl;
std::cout<<it->second<<std::endl;
        QListWidgetItem *item = new QListWidgetItem( it->first.second , para_ref_tracts_listWidget ) ;
        item->setCheckState( it->second != 0 ? Qt::Checked : Qt::Unchecked ) ;
    }
    /*3rd tab: software*/
    soft_dtiprocess_lineEdit->setText( m_soft_m->getsoft_dtiprocess_lineEdit() );
    soft_DTIReg_lineEdit->setText( m_soft_m->getsoft_DTIReg_lineEdit() );
    soft_FiberPostProcess_lineEdit->setText( m_soft_m->getsoft_FiberPostProcess_lineEdit() );
    soft_fiberprocess_lineEdit->setText( m_soft_m->getsoft_fiberprocess_lineEdit() );
    soft_ImageMath_lineEdit->setText( m_soft_m->getsoft_ImageMath_lineEdit() );
    soft_ResampleDTIlogEuclidean_lineEdit->setText( m_soft_m->getsoft_ResampleDTIlogEuclidean_lineEdit() );
    soft_MDT_lineEdit->setText( m_soft_m->getsoft_MDT_lineEdit() );
    soft_polydatatransform_lineEdit->setText( m_soft_m->getsoft_polydatatransform_lineEdit() );
    soft_python_lineEdit->setText( m_soft_m->getsoft_python_lineEdit() );
    soft_TractographyLabelMapSeeding_lineEdit->setText( m_soft_m->getsoft_TractographyLabelMapSeeding_lineEdit() );
    soft_unu_lineEdit->setText( m_soft_m->getsoft_unu_lineEdit() );
    soft_slicer_lineEdit->setText( m_soft_m->getsoft_slicer_lineEdit() );
    soft_ANTS_lineEdit->setText( m_soft_m->getsoft_ANTS_lineEdit() );
    soft_ITKTransformTools_lineEdit->setText( m_soft_m->getsoft_ITKTransformTools_lineEdit() );
    soft_docker_lineEdit->setText( m_soft_m->getsoft_docker_lineEdit() );

    /*4th tab: registration*/
    para_registration_type_comboBox->setCurrentIndex(para_registration_type_comboBox->findText(m_para_m->getpara_registration_type_comboBox()));
    para_transformation_step_spinBox->setValue( m_para_m->getpara_transformation_step_spinBox() );
    para_iterations_lineEdit->setText( m_para_m->getpara_iterations_lineEdit() );
    para_similarity_metric_comboBox->setCurrentIndex(para_similarity_metric_comboBox->findText(m_para_m->getpara_similarity_metric_comboBox() ) );
    para_gaussian_sigma_spinBox->setValue( m_para_m->getpara_gaussian_sigma_spinBox() );
    para_similarity_parameter_spinBox->setValue( m_para_m->getpara_similarity_parameter_spinBox() );

    /*5th tab*/
    //std::cout<<m_para_m->getpara_dilation_radius_spinBox()<<std::endl;

    para_dilation_radius_spinBox->setValue( m_para_m->getpara_dilation_radius_spinBox() );
    para_seedspacing_spinBox->setValue( m_para_m->getpara_seedspacing_spinBox() );
    para_linearmeasure_spinBox->setValue( m_para_m->getpara_linearmeasure_spinBox() );
    para_minpathlength_spinBox->setValue( m_para_m->getpara_minpathlength_spinBox() );
    para_maxpathlength_spinBox->setValue( m_para_m->getpara_maxpathlength_spinBox() );
    para_stoppingvalue_spinBox->setValue( m_para_m->getpara_stoppingvalue_spinBox() );
    para_stoppingcurvature_spinBox->setValue( m_para_m->getpara_stoppingcurvature_spinBox() );
    para_integrationsteplength_spinBox->setValue( m_para_m->getpara_integrationsteplength_spinBox() );

    /*6th tab*/
    para_thresholdCSFmask_spinBox->setValue(m_para_m->getpara_thresholdCSFmask_spinBox() );
    para_tractOverlapRatio_spinBox->setValue(m_para_m->getpara_tractOverlapRatio_spinBox() );
    para_tractMaxDistThreshold_spinBox->setValue(m_para_m->getpara_tractMaxDistThreshold_spinBox() );

    /*7th tab: classification*/
    para_enable_trafic_checkBox->setChecked(m_para_m->getpara_enable_trafic_checkBox());
    para_no_tract_output_trafic_checkBox->setChecked(m_para_m->getpara_no_tract_output_trafic_checkBox());
    para_trafic_model_dir_lineEdit->setText(m_para_m->getpara_trafic_model_dir_lineEdit());
    // m_para_m->setpara_trafic_model_dir_lineEdit( para_trafic_model_dir_lineEdit->value() );


    /*8th tab: Execution*/
    //para_all_radioButton->setChecked( m_para_m->getpara_all_radioButton() );
    //para_singletract_radioButton->setChecked( m_para_m->getpara_singletract_radioButton() );
    para_computingSystem_comboBox->setCurrentIndex(para_computingSystem_comboBox->findText(m_para_m->getpara_computingSystem_comboBox() ) );
    para_nbCores_spinBox->setValue( m_para_m->getpara_nbCores_spinBox() );
    para_nbTractsProcessed_spinBox->setValue( m_para_m->getpara_nbTractsProcessed_spinBox() );
    //para_cleanup_checkBox->setChecked( m_para_m->getpara_cleanup_checkBox() );
    //para_overwrite_checkBox->setChecked( m_para_m->getpara_overwrite_checkBox() );
    //para_singletract_comboBox->setCurrentIndex( para_singletract_comboBox->findText(m_para_m->getpara_singletract_comboBox() ) ) ;
    para_nb_memory_spinBox->setValue( m_para_m->getpara_nb_memory_spinBox() );
    para_nb_threads_spinBox->setValue( m_para_m->getpara_nb_threads_spinBox());
    para_nb_memory_registration_spinBox->setValue( m_para_m->getpara_nb_memory_registration_spinBox() );

    m_sync = 0 ;
}

void AutoTractWindow::SyncModelStructureToUi( QString prefix )
{
    if( m_sync == 1)
    {
        return;
    }
    m_sync = 1;

    if(prefix == "para")
    {
        /*1st tab: inputs*/
        para_inputDTIatlas_lineEdit->setText( m_para_m->getpara_inputDTIatlas_lineEdit() );
        para_inputWMmask_lineEdit->setText( m_para_m->getpara_inputWMmask_lineEdit() );
        para_inputCSFmask_lineEdit->setText( m_para_m->getpara_inputCSFmask_lineEdit() );
        para_output_dir_lineEdit->setText( m_para_m->getpara_output_dir_lineEdit() );
        para_refDTIatlas_lineEdit->setText( m_para_m->getpara_refDTIatlas_lineEdit() );
        para_displacementField_lineEdit->setText( m_para_m->getpara_displacementField_lineEdit() );

        /*2nd tab: reference tracts*/
        para_tracts_dir_lineEdit->setText( m_para_m->getpara_tracts_dir_lineEdit() );
        std::map<std::pair<unsigned long,QString>,bool> itemList ;
        itemList = m_para_m->getpara_ref_tracts_listWidget() ;
        para_ref_tracts_listWidget->clear() ;
        for( std::map<std::pair<unsigned long,QString>,bool>::iterator it = itemList.begin() ; it != itemList.end() ; it++ )
        {
            QListWidgetItem *item = new QListWidgetItem( it->first.second , para_ref_tracts_listWidget ) ;
            item->setCheckState( it->second != 0 ? Qt::Checked : Qt::Unchecked ) ;
        }
        /*4th tab: registration*/
        para_registration_type_comboBox->setCurrentIndex(para_registration_type_comboBox->findText(m_para_m->getpara_registration_type_comboBox()));
        para_transformation_step_spinBox->setValue( m_para_m->getpara_transformation_step_spinBox() );
        para_iterations_lineEdit->setText( m_para_m->getpara_iterations_lineEdit() );
        para_similarity_metric_comboBox->setCurrentIndex(para_similarity_metric_comboBox->findText(m_para_m->getpara_similarity_metric_comboBox() ) );
        para_gaussian_sigma_spinBox->setValue( m_para_m->getpara_gaussian_sigma_spinBox() );
        para_similarity_parameter_spinBox->setValue( m_para_m->getpara_similarity_parameter_spinBox() );

        /*5th tab*/
        para_dilation_radius_spinBox->setValue( m_para_m->getpara_dilation_radius_spinBox() );
        para_seedspacing_spinBox->setValue( m_para_m->getpara_seedspacing_spinBox() );
        para_linearmeasure_spinBox->setValue( m_para_m->getpara_linearmeasure_spinBox() );
        para_minpathlength_spinBox->setValue( m_para_m->getpara_minpathlength_spinBox() );
        para_maxpathlength_spinBox->setValue( m_para_m->getpara_maxpathlength_spinBox() );
        para_stoppingvalue_spinBox->setValue( m_para_m->getpara_stoppingvalue_spinBox() );
        para_stoppingcurvature_spinBox->setValue( m_para_m->getpara_stoppingcurvature_spinBox() );
        para_integrationsteplength_spinBox->setValue( m_para_m->getpara_integrationsteplength_spinBox() );

        /*6th tab*/
        para_thresholdCSFmask_spinBox->setValue(m_para_m->getpara_thresholdCSFmask_spinBox() );
        para_tractOverlapRatio_spinBox->setValue(m_para_m->getpara_tractOverlapRatio_spinBox() );
        para_tractMaxDistThreshold_spinBox->setValue(m_para_m->getpara_tractMaxDistThreshold_spinBox() );

        /*7th tab: classification*/   
        para_enable_trafic_checkBox->setChecked(m_para_m->getpara_enable_trafic_checkBox());
        para_no_tract_output_trafic_checkBox->setChecked(m_para_m->getpara_no_tract_output_trafic_checkBox());
        para_trafic_model_dir_lineEdit->setText(m_para_m->getpara_trafic_model_dir_lineEdit());
        // m_para_m->setpara_trafic_model_dir_lineEdit( para_trafic_model_dir_lineEdit->value() );

        /*8th tab: Execution*/
        //para_all_radioButton->setChecked( m_para_m->getpara_all_radioButton() );
        //para_singletract_radioButton->setChecked( m_para_m->getpara_singletract_radioButton() );
        para_computingSystem_comboBox->setCurrentIndex(para_computingSystem_comboBox->findText(m_para_m->getpara_computingSystem_comboBox() ) );
        para_nbCores_spinBox->setValue( m_para_m->getpara_nbCores_spinBox() );
        para_nbTractsProcessed_spinBox->setValue( m_para_m->getpara_nbTractsProcessed_spinBox() );
        //para_cleanup_checkBox->setChecked( m_para_m->getpara_cleanup_checkBox() );
        //para_overwrite_checkBox->setChecked( m_para_m->getpara_overwrite_checkBox() );
        //para_singletract_comboBox->setCurrentIndex( para_singletract_comboBox->findText(m_para_m->getpara_singletract_comboBox() ) ) ;
        para_nb_memory_spinBox->setValue( m_para_m->getpara_nb_memory_spinBox() );
        para_nb_threads_spinBox->setValue( m_para_m->getpara_nb_threads_spinBox());
        para_nb_memory_registration_spinBox->setValue( m_para_m->getpara_nb_memory_registration_spinBox() );
    }
    if(prefix == "soft")
    {
        /*3rd tab: software*/
        soft_dtiprocess_lineEdit->setText( m_soft_m->getsoft_dtiprocess_lineEdit() );
        soft_DTIReg_lineEdit->setText( m_soft_m->getsoft_DTIReg_lineEdit() );
        soft_FiberPostProcess_lineEdit->setText( m_soft_m->getsoft_FiberPostProcess_lineEdit() );
        soft_fiberprocess_lineEdit->setText( m_soft_m->getsoft_fiberprocess_lineEdit() );
        soft_ImageMath_lineEdit->setText( m_soft_m->getsoft_ImageMath_lineEdit() );
        soft_ResampleDTIlogEuclidean_lineEdit->setText( m_soft_m->getsoft_ResampleDTIlogEuclidean_lineEdit() );
        soft_MDT_lineEdit->setText( m_soft_m->getsoft_MDT_lineEdit() );
        soft_polydatatransform_lineEdit->setText( m_soft_m->getsoft_polydatatransform_lineEdit() );
        soft_python_lineEdit->setText( m_soft_m->getsoft_python_lineEdit() );
        soft_TractographyLabelMapSeeding_lineEdit->setText( m_soft_m->getsoft_TractographyLabelMapSeeding_lineEdit() );
        soft_unu_lineEdit->setText( m_soft_m->getsoft_unu_lineEdit() );
        soft_slicer_lineEdit->setText( m_soft_m->getsoft_slicer_lineEdit() );
        soft_ANTS_lineEdit->setText( m_soft_m->getsoft_ANTS_lineEdit() );
        soft_ITKTransformTools_lineEdit->setText( m_soft_m->getsoft_ITKTransformTools_lineEdit() );
        soft_docker_lineEdit->setText( m_soft_m->getsoft_docker_lineEdit() );
    }
    m_sync = 0 ;
}

void AutoTractWindow::Save_Parameter_Configuration( std::string filename )
{
    SyncUiToModelStructure();
    m_para_s->save( *m_para_m, filename );
}

void AutoTractWindow::Load_Parameter_Configuration( std::string filename )
{
    m_para_l->load( *m_para_m, filename );
    SyncModelStructureToUi();
}

void AutoTractWindow::Save_Software_Configuration( std::string filename )
{
    SyncUiToModelStructure();
    m_soft_s->save( *m_soft_m, filename );
}
void AutoTractWindow::Load_Software_Configuration( std::string filename )
{
    m_soft_l->load( *m_soft_m, filename );
    SyncModelStructureToUi();
}

void AutoTractWindow::SetParaModel( para_Model_AutoTract* para_m )
{
    m_para_m = para_m;
}

void AutoTractWindow::SetParaSave( para_Save_AutoTract* para_s )
{
    m_para_s = para_s;
}

void AutoTractWindow::SetParaLoad( para_Load_AutoTract* para_l )
{
    m_para_l = para_l;
}

void AutoTractWindow::SetSoftModel( soft_Model_AutoTract* soft_m )
{
    m_soft_m = soft_m;
}

void AutoTractWindow::SetSoftSave( soft_Save_AutoTract* soft_s)
{
    m_soft_s = soft_s;
}

void AutoTractWindow::SetSoftLoad( soft_Load_AutoTract* soft_l)
{
    m_soft_l = soft_l;
}
