
#include "SingleTractProcess.h"

SingleTractProcess::SingleTractProcess(QString module) : Script(module)
{

}

// SET //
void SingleTractProcess::setLog(QString log_path)
{
    m_log_path = log_path;
}

void SingleTractProcess::initializeScript()
{
    definePython();
    importGeneralModules();

    m_script += "import re\n";
    m_script += "import time\n";

    m_script += "Slicer ='" + m_soft_m->getsoft_slicer_lineEdit() + "'\n";
    m_script += "polydatatransform = '" + m_soft_m->getsoft_polydatatransform_lineEdit() + "'\n";
    m_script += "TractographyLabelMapSeeding = '" + m_soft_m->getsoft_TractographyLabelMapSeeding_lineEdit() + "'\n";
    m_script += "fiberprocess = '" + m_soft_m->getsoft_fiberprocess_lineEdit() + "'\n";
    m_script += "ImageMath = '" + m_soft_m->getsoft_ImageMath_lineEdit() + "'\n";
    m_script += "dtiprocess = '" + m_soft_m->getsoft_dtiprocess_lineEdit() + "'\n";
    m_script += "FiberPostProcess ='" + m_soft_m->getsoft_FiberPostProcess_lineEdit() + "'\n";
    m_script += "MDT = '" + m_soft_m->getsoft_MDT_lineEdit() + "'\n";
    m_script += "inputDTIatlas_dir = '" + m_para_m->getpara_inputDTIatlas_lineEdit() + "'\n";
    m_script += "dilationRadius = '" + QString::number(m_para_m->getpara_dilation_radius_spinBox()) + "'\n";
    m_script += "seedspacing = '" + QString::number(m_para_m->getpara_seedspacing_spinBox()) + "'\n";
    m_script += "clthreshold = '" + QString::number(m_para_m->getpara_linearmeasure_spinBox()) + "'\n";
    m_script += "minimumlength = '" + QString::number(m_para_m->getpara_minpathlength_spinBox()) + "'\n";
    m_script += "maximumlength = '" + QString::number(m_para_m->getpara_maxpathlength_spinBox()) + "'\n";
    m_script += "stoppingcurvature = '" + QString::number(m_para_m->getpara_stoppingcurvature_spinBox()) + "'\n";
    m_script += "integrationsteplength = '" + QString::number(m_para_m->getpara_integrationsteplength_spinBox()) + "'\n";
    m_script += "stoppingvalue ='" + QString::number(m_para_m->getpara_stoppingvalue_spinBox()) + "'\n";
    m_script += "output_dir ='" + m_para_m->getpara_output_dir_lineEdit() + "'\n";
    m_script += "thresholdWMmask ='" + QString::number(m_para_m->getpara_thresholdWMmask_spinBox()) + "'\n";
    m_script += "tractOverlapRatio ='" + QString::number(m_para_m->getpara_tractOverlapRatio_spinBox()) + "'\n";
    m_script += "tractMaxDistThreshold ='" + QString::number(m_para_m->getpara_tractMaxDistThreshold_spinBox()) + "'\n";

    m_script += "logger = None\n";

    m_script += "runningProcess = None\n";
    m_script += "logFile = None\n\n";
}

void SingleTractProcess::initializeLogging()
{
    m_script += "def initializeLogging(log):\n";

    m_script += "\tglobal logger\n";
    m_script += "\tlogger = logging.getLogger('SingleTractProcess')\n";
    m_script += "\tlogger.setLevel(logging.DEBUG)\n";

    m_script += "\tfileHandler = logging.FileHandler(log)\n";
    m_script += "\tfileHandler.setLevel(logging.DEBUG)\n";
    m_script += "\tfileFormatter = logging.Formatter('%(message)s')\n";
    m_script += "\tfileHandler.setFormatter(fileFormatter)\n";

    m_script += "\tlogger.addHandler(fileHandler)\n\n";
}

void SingleTractProcess::implementSingleTractProcess()
{
    m_script += "def main(name, tract, current_dir, displacementField, log):\n\n";

    m_script += "\tsignal.signal(signal.SIGINT, stop)\n";
    m_script += "\tsignal.signal(signal.SIGTERM, stop)\n\n";

    m_script += "\tlogFile = open(log, \"w\")\n";
    m_script += "\tinitializeLogging(log)\n\n";

    //m_script += "\t\ttime.sleep(0.5)\n";
    /*m_script += "\t\tmainLog = open('" + m_log_path + "','a')\n";
    m_script += "\t\tmainLog.write(name + ' PostProcess -> Skipped\\n')\n";
    m_script += "\t\tmainLog.close()\n";
    m_script += "\t\treturn\n";*/

    m_script += "\tmainLog = open('" + m_log_path + "','a')\n";
    m_script += "\tmainLog.write(name + ' PostProcess...\\n')\n";
    m_script += "\tmainLog.close()\n";

    m_log = "Mapping reference tracts";

    //m_inputs.insert("tract", tract);
    //m_inputs.insert("displacementField", displacementField);

    m_script += "\n\tref_tract_mapped = current_dir + '/' + name + '_t.vtk'";
    m_script += "\n";
    m_script += "\tref_tract_dilated = current_dir + '/' + name + '_t_dil.vtk'";
    m_script += "\n\n";

    m_argumentsList << "polydatatransform" << "'--fiber_file'" << "tract" << "'-o'" << "ref_tract_mapped" << "'-D'" << "displacementField" << "'--inverty'" << "'--invertx'";
    execute();

    m_log = "Dilation and voxelization of the mapped reference tracts - Step 1";

    m_script += "\tlabelmap = current_dir + '/' + name + '.nrrd'";
    m_script += "\n\n";
    m_argumentsList << "fiberprocess" << "'--voxelize'" << "labelmap" << "'--fiber_file'" << "ref_tract_mapped" << "'-T'" << "inputDTIatlas_dir";
    execute();

    m_log = "Dilation and voxelization of the mapped reference tracts - Step 2";
    m_script += "\n\n";
    m_script += "\tdilatedImage = current_dir + '/' + name + '_dil.nrrd'";
    m_script += "\n";
    m_argumentsList << "ImageMath" << "labelmap" << "'-dilate'" << "str(dilationRadius) + \',1\'" << "'-outfile'" << "dilatedImage";
    execute();

    m_log = "Tractography by labelmap seeding";
    m_script += "\n\n";


    m_script += "\ttractedFiber = current_dir + '/' + name + '.vtp'";
    m_script += "\n";
    m_argumentsList << "Slicer" << "'--launch'" << "TractographyLabelMapSeeding" << "inputDTIatlas_dir" << "tractedFiber" << "'-a'" << "dilatedImage" << "'-s'" << "seedspacing" << "'--clthreshold'" << "clthreshold" << "'--minimumlength'" << "minimumlength" << "'--maximumlength'" << "maximumlength" << "'--stoppingcurvature'" << "stoppingcurvature" << "'--integrationsteplength'" << "integrationsteplength" << "'--stoppingvalue'" << "stoppingvalue";
    execute();

    m_log = "Cropping reference tracts";
    m_script += "\n";
    m_script += "\trefFiberCropped = current_dir + '/' + name + '_ref_cleanEnds.vtp'";
    m_script += "\n";
    m_argumentsList << "FiberPostProcess" << "'-i'" << "ref_tract_mapped" << "'-o'" << "refFiberCropped" << "'--crop'" << "'-m'" << "output_dir + '/2.MaskCreation/WMmask.nrrd'" << "'--thresholdMode'" << "'above'";
    execute();

    m_log = "Cropping using WM mask";
    m_script += "\n";
    m_script += "\tfiberCropped = current_dir + '/' + name + '_cleanEnds.vtp'";
    m_script += "\n";
    m_argumentsList << "FiberPostProcess" << "'-i'" << "tractedFiber" << "'-o'" << "fiberCropped" << "'--crop'" << "'-m'" << "output_dir + '/2.MaskCreation/WMmask.nrrd'" << "'--thresholdMode'" << "'above'";
    execute();

    m_log = "Masking with CSF mask";
    m_script += "\n";
    m_script += "\tfiberMaskedCSF = current_dir + '/' + name + '_maskCSF.vtp'";
    m_script += "\n";
    m_argumentsList << "FiberPostProcess" << "'-i'" << "fiberCropped" << "'-o'" << "fiberMaskedCSF" << "'--mask'" << "'--clean'" << "'-m'" << "output_dir + '/2.MaskCreation/MDmask.nrrd'" << "'--thresholdMode'" << "'above'" << "'-t'" << "thresholdWMmask";
    execute();

    m_log = "Masking with dilated reference image";
    m_script += "\n";
    m_script += "\tfiberMaskedTract = current_dir + '/' + name + '_maskTract.vtp'";
    m_script += "\n";
    m_argumentsList << "FiberPostProcess" << "'-i'" << "fiberMaskedCSF" << "'-o'" << "fiberMaskedTract" << "'--mask'" << "'--clean'" << "'-m'" << "dilatedImage" << "'--thresholdMode'" << "'below'" << "'-t'" << "tractOverlapRatio";
    execute();

    m_log = "Matching length with reference tract";
    m_script += "\n";
    m_script += "\tfiberLengthMatch = current_dir + '/' + name + '_lengthMatch.vtp'";
    m_script += "\n";
    m_argumentsList << "FiberPostProcess" << "'-i'" << "fiberMaskedTract" << "'--lengthMatch'" << "refFiberCropped" << "'-o'" << "fiberLengthMatch";
    execute();

    m_log = "Computation of the distance map of the reference tract - Step 1";
    m_script += "\n";
    m_script += "\tvoxelizedImage = current_dir + '/' + name + '_voxelizedImage.nrrd'";
    m_script += "\n";
    m_argumentsList << "fiberprocess" << "'--voxelize'" << "voxelizedImage" << "'--fiber_file'" << "fiberLengthMatch" << "'-T'" << "output_dir + '/2.MaskCreation/upsampledImage.nrrd'";
    execute();

    m_log = "Computation of the distance map of the reference tract - Step 2";
    m_script += "\n";
    m_script += "\tdistanceMap = current_dir + '/' + name + '_distanceMap.nrrd'";
    m_script += "\n";
    m_argumentsList << "MDT" << "voxelizedImage" << "distanceMap";
    execute();

    m_log = "Matching tract with the distance map";
    m_script += "\n";
    m_script += "\toutputTract = current_dir + '/' + name + '_processed.vtp'";
    m_script += "\n";
    m_argumentsList << "FiberPostProcess" << "'-i'" << "fiberLengthMatch" << "'-o'" << "outputTract" << "'-m'" << "distanceMap" << "'--threshold'" << "tractMaxDistThreshold" << "'--mask'" << "'--clean'" << "'--thresholdMode'" << "'above'";
    execute();

    m_log = "Creation of a tmp txt file";
    m_script += "\n";
    m_script += "\ttmp = current_dir + '/' + name + '_tmp.txt'";
    m_script += "\n";
    m_argumentsList << "'touch'" << "'tmp'";
    execute();

}

void SingleTractProcess::writeSingleTractProcess()
{
    initializeScript();

    implementStop();
    implementCheckFileExistence();
    initializeLogging();
    implementExecute();
    implementSingleTractProcess();

    m_script += "if __name__ == '__main__':\n";
    m_script += "\tmain(sys.argv[1], sys.argv[2], sys.argv[3], sys.argv[4], sys.argv[5])\n";

    QString script_path = m_processing_dir->filePath("SingleTractProcess.py");

    std::ofstream* script_stream = new std::ofstream((script_path.toStdString()).c_str(), std::ios::out | std::ios::trunc);
    *script_stream << m_script.toStdString() << std::endl;
}

void SingleTractProcess::update()
{
    writeSingleTractProcess();
}


