#include "MaskCreation.h"

MaskCreation::MaskCreation(QString module) : Script(module)
{
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
    m_script += "\tWMmask = outputDir + '/WMmask.nrrd'";
    m_script += "\n";
    m_script += "\tMDmask = outputDir + '/MDmask.nrrd'";
    m_script += "\n";
    m_script += "\tbrainSkull = outputDir + '/brainSkull.nrrd'";
    m_script += "\n";
    m_script += "\tupsampledImage = outputDir + '/upsampledImage.nrrd'";
    m_script += "\n";
    m_argumentsList << "dtiprocess" << "'--dti_image'" << "inputDTIatlas_dir" <<"'-f'" << "FAimage";
    execute();
    m_log = "Creation of WM mask - Step 2";
    m_argumentsList << "ImageMath" << "FAimage" << "'-outfile'" << "WMmask" << "'-dilate'" << "'10,10'";
    execute();
    m_log = "Creation of WM mask - Step 3";
    m_argumentsList << "ImageMath" << "WMmask" << "'-otsu'" << "'-outfile'" << "WMmask";
    execute();
    m_script += "\n\n";

    m_log = "Creation of CSF mask - Step 1";
    m_argumentsList << "dtiprocess" << "'--dti_image'" << "inputDTIatlas_dir" <<"'-m'" << "MDimage";
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
    m_argumentsList << "ImageMath" << "MDimage" << "'-outfile'" << "MDmask" << "'-otsuMultipleThresholds'" << "'-otsuPara'" << "otsuPara";
    execute();
    m_log = "Creation of CSF mask - Step 3";
    m_argumentsList << "ImageMath" << "MDmask" << "'-outfile'" << "MDmask" << "'-erode'" << "'2,1'";
    execute();
    m_log = "Creation of CSF mask - Step 4";
    m_argumentsList << "ImageMath" << "MDmask" << "'-outfile'" << "brainSkull" << "'-erode'" << "'4,1'";
    execute();
    m_log = "Creation of CSF mask - Step 5";
    m_argumentsList << "ImageMath" << "MDmask" << "'-outfile'" << "MDmask" << "'-sub'" << "brainSkull";
    execute();
    m_script += "\n\n";
    m_log = "Upsampling of the reference image - Step 1 ";
    m_argumentsList << "unu" << "'resample'" << "'-i'" << "FAimage" << "'-o'" << "upsampledImage" << "'-s'" << "'x2'" << "'x2'" << "'x2'";
    execute();
    m_log = "Upsampling of the reference image - Step 2";
    m_argumentsList << "ResampleDTIlogEuclidean" << "inputDTIatlas_dir" << "upsampledImage" << "'-R'" << "upsampledImage";
    execute();
    m_script +="\n\n";

}

void MaskCreation::implementRun()
{
    m_script += "def run():\n\n";

    m_script += "\tsignal.signal(signal.SIGINT, stop)\n";
    m_script += "\tsignal.signal(signal.SIGTERM, stop)\n\n";

    m_script += "\tlogger.info('=== MaskCreation ===')\n";

    m_outputs.insert("WMmask", m_outputDir + "/WMmask.nrrd");
    m_outputs.insert("MDmask", m_outputDir + "/MDmask.nrrd");
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
    initializeScript();
    implementStop();
    implementCheckFileExistence();
    implementExecute();
    implementExecutePipe();
    implementRun();

    writeScript();
}
