#include "MaskCreation.h"

MaskCreation::MaskCreation(QString module) : Script(module)
{
}

void MaskCreation::setWMMaskPath(QString path)
{
    m_WMPath = path;
}

void MaskCreation::setCSFMaskPath(QString path)
{
    m_CSFPath = path;
}


void MaskCreation::initializeScript()
{
    definePython();
    importGeneralModules();

    m_script += "import re\n";
    m_script += "ImageMath = '" + m_soft_m->getsoft_ImageMath_lineEdit() + "'\n";
    m_script += "dtiprocess ='" + m_soft_m->getsoft_dtiprocess_lineEdit() + "'\n";
    m_script += "unu ='" + m_soft_m->getsoft_unu_lineEdit() + "'\n";
    m_script += "ResampleDTIlogEuclidean ='" + m_soft_m->getsoft_ResampleDTIlogEuclidean_lineEdit() + "'\n";
    m_script += "refDTIatlas_dir = '" + m_para_m->getpara_refDTIatlas_lineEdit() + "'\n";
    m_script += "inputDTIatlas_dir = '" + m_para_m->getpara_inputDTIatlas_lineEdit() + "'\n";
    m_script += "outputDir = '" + m_outputDir + "'\n";

    m_script += "logger = logging.getLogger('AutoTract')\n\n";

    m_script += "runningProcess = None\n\n";
}

void MaskCreation::setOutputDirectory(QString dir)
{
    m_outputDir = m_para_m->getpara_output_dir_lineEdit() + "/" +  dir;
}

void MaskCreation::executeMaskCreation()
{
    m_log = "Creation of WM mask - Step 1";
    m_script += "\n\n";
    m_script += "\tFAimage = outputDir + '/FAimage.nrrd'";
    m_script += "\n";
    m_script += "\tMDimage = outputDir + '/MDimage.nrrd'";
    m_script += "\n";
    m_script += "\totsuFA = outputDir + '/otsuFA.nrrd'";
    m_script += "\n";
    m_script += "\totsuFAeroded = outputDir + '/otsuFAeroded.nrrd'";
    m_script += "\n";
    m_script += "\tWMmask = '" + m_WMPath + "'";
    m_script += "\n";
    m_script += "\tMDmask = '" + m_CSFPath + "'";
    m_script += "\n";
    m_script += "\totsuThreshold = outputDir + '/otsuThreshold.nrrd'";
    m_script += "\n";
    m_script += "\terodedOtsuMask = outputDir + '/erodedOtsuMask.nrrd'";
    m_script += "\n";
    m_script += "\tthresholdedOtsuThreshold = outputDir + '/thresholdedOtsuThreshold.nrrd'";
    m_script += "\n";
    m_script += "\tbrainSkull = outputDir + '/brainSkull.nrrd'";
    m_script += "\n";
    m_script += "\treferenceupsampledImage = outputDir + '/referenceupsampledImage.nrrd'";
    m_script += "\n";
    m_script += "\tupsampledImage = outputDir + '/upsampledImage.nrrd'";
    m_script += "\n";
    m_argumentsList << "dtiprocess" << "'--dti_image'" << "inputDTIatlas_dir" <<"'-f'" << "FAimage";
    m_outputs.insert("WMmask", m_WMPath );
    execute();
    m_log = "Creation of WM mask - Step 2";
    m_argumentsList << "ImageMath" << "FAimage" << "'-otsu'" << "'-outfile'" << "otsuFA";
    m_outputs.insert("WMmask", m_WMPath );
    execute();
    m_log = "Creation of WM mask - Step 3";
    m_argumentsList << "ImageMath" << "otsuFA" << "'-outfile'" << "otsuFAeroded" << "'-erode'" << "'2,1'";
    m_outputs.insert("WMmask", m_WMPath );
    execute();
    m_log = "Creation of WM mask - Step 4";
    m_argumentsList << "ImageMath" << "otsuFAeroded" << "'-outfile'" << "WMmask" << "'-dilate'" << "'3,1'";
    m_outputs.insert("WMmask", m_WMPath );
    execute();
    m_script += "\n\n";
    m_log = "Creation of CSF mask - Step 1";
    m_argumentsList << "dtiprocess" << "'--dti_image'" << "inputDTIatlas_dir" <<"'-m'" << "MDimage";
    m_outputs.insert("MDmask", m_CSFPath );
    execute();
    m_log = "Creation of CSF mask - Step 2";
    m_script += "\tnbThresholds = '3'";
    m_script += "\n";
    m_script += "\tnbHistogramBins = '128'";
    m_script += "\n";
    m_script += "\tlabelOffset = '0'";
    m_script += "\n";
    m_script += "\totsuPara = nbThresholds + ',' + labelOffset + ',' + nbHistogramBins";
    m_script += "\n";
    m_argumentsList << "ImageMath" << "MDimage" << "'-outfile'" << "otsuThreshold" << "'-otsuMultipleThresholds'" << "'-otsuPara'" << "otsuPara";
    m_outputs.insert("MDmask", m_CSFPath );
    execute();
    m_log = "Creation of CSF mask - Step 3";
    m_argumentsList << "ImageMath" << "otsuThreshold" << "'-outfile'" << "thresholdedOtsuThreshold" << "'-threshold'" << "'.5,10000'";
    m_outputs.insert("MDmask", m_CSFPath );
    execute();
    m_log = "Creation of CSF mask - Step 4";
    m_argumentsList << "ImageMath" << "thresholdedOtsuThreshold" << "'-outfile'" << "erodedOtsuMask" << "'-erode'" << "'2,1'";
    m_outputs.insert("MDmask", m_CSFPath );
    execute();
    m_log = "Creation of CSF mask - Step 5";
    m_argumentsList << "ImageMath" << "erodedOtsuMask" << "'-outfile'" << "brainSkull" << "'-erode'" << "'4,1'";
    m_outputs.insert("MDmask", m_CSFPath );
    execute();
    m_log = "Creation of CSF mask - Step 6";
    m_argumentsList << "ImageMath" << "erodedOtsuMask" << "'-outfile'" << "MDmask" << "'-sub'" << "brainSkull";
    m_outputs.insert("MDmask", m_CSFPath );
    execute();
    m_script += "\n\n";
    m_log = "Upsampling of the reference image - Step 1 ";
    m_argumentsList << "unu" << "'resample'" << "'-i'" << "FAimage" << "'-o'" << "referenceupsampledImage" << "'-s'" << "'x2'" << "'x2'" << "'x2'";
    m_outputs.insert("upsampledImage", m_outputDir + "/upsampledImage.nrrd");
    execute();
    m_log = "Upsampling of the reference image - Step 2";
    m_argumentsList << "ResampleDTIlogEuclidean" << "inputDTIatlas_dir" << "upsampledImage" << "'-R'" << "referenceupsampledImage";
    m_outputs.insert("upsampledImage", m_outputDir + "/upsampledImage.nrrd");
    execute();
    m_script +="\n\n";

}

void MaskCreation::implementRun()
{
    m_script += "def run():\n\n";

    m_script += "\tsignal.signal(signal.SIGINT, stop)\n";
    m_script += "\tsignal.signal(signal.SIGTERM, stop)\n\n";

    m_script += "\tlogger.info('=== MaskCreation ===')\n";

    m_outputs.insert("WMmask", m_WMPath );
    //m_outputs.insert("MDmask", m_outputDir + "/MDmask.nrrd");
    m_outputs.insert("MDmask", m_CSFPath );
    m_outputs.insert("upsampledImage", m_outputDir + "/upsampledImage.nrrd");
    checkFinalOutputs();

    m_script += "\tlogger.info('')\n";

    executeMaskCreation();
    // Cleaning for keven data
    //    QFileInfo fi(m_para_m->getpara_refDTIatlas_lineEdit());
    //    QString base = fi.baseName();
    //    m_unnecessaryFiles << m_module_dir->filePath(base + "_ANTSAffine.txt");
    //    m_unnecessaryFiles << m_module_dir->filePath(base + "_InverseWarp.nii.gz");
    //    m_unnecessaryFiles << m_module_dir->filePath(base + "_Warp.nii.gz");
    //    m_unnecessaryFiles << m_module_dir->filePath(base + "_DTI-Reg.bms");
    //    m_unnecessaryFiles << m_module_dir->filePath(base + "_FA.nrrd");
    //    m_unnecessaryFiles << m_module_dir->filePath(base + "_warp.nrrd");
}

void MaskCreation::update()
{
    if( m_WMPath.isEmpty() )
    {
        m_WMPath = m_outputDir + "/WMmask.nrrd" ;
    }
    if( m_CSFPath.isEmpty() )
    {
        m_CSFPath = m_outputDir + "/CSFmask.nrrd" ;
    }
    initializeScript();
    implementStop();
    implementCheckFileExistence();
    implementExecute();
    implementExecutePipe();
    implementRun();

    writeScript();
}
