#include "TractPopulationProcess.h"

TractPopulationProcess::TractPopulationProcess(QString module) : Script(module)
{
}

bool TractPopulationProcess::fileExists(QString path) {
    QFileInfo checkFile(path);
    // check if file exists and if yes: Is it really a file and no directory?
    if (checkFile.exists() && checkFile.isFile()) {
        return true;
    } else {
        return false;
    }
}

void TractPopulationProcess::initializeScript()
{
    definePython();
    importGeneralModules();
    m_script += "import re\n";
    m_script += "import time\n";
    m_script += "import array\n";
    m_script += "from collections import namedtuple\n\n";

    m_script += "tracts_dir = '" + m_para_m->getpara_tracts_dir_lineEdit() + "'\n";
    m_script += "displacementFieldPath = '" + m_displacementFieldPath + "'\n";
    m_script += "nbCores = " + QString::number(m_para_m->getpara_nbCores_spinBox()) + "\n";
    m_script += "nbMemory = " + QString::number(m_para_m->getpara_nb_memory_spinBox()) + "\n";
    m_script += "nbTractProcessedMax = " + QString::number(m_para_m->getpara_nbTractsProcessed_spinBox()) + "\n";

    m_script += "logger = logging.getLogger('AutoTract')\n\n";

    m_script += "runningTract = None\n\n";
}
void TractPopulationProcess::SetDisplacementFieldPath(QString displacementFieldPath)
{
    m_displacementFieldPath = displacementFieldPath;
}


void TractPopulationProcess::defineRefTracts()
{
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
    m_tractPopulation = ref_tracts;
}

// Creating Directories //
void TractPopulationProcess::createTractDirectories()
{
    std::vector<QString>::iterator it;

    for (it = m_tractPopulation.begin(); it != m_tractPopulation.end(); ++it)
    {
        QFileInfo fi(*it);
        QString base = fi.baseName();
        if (!m_module_dir->exists((base)))
        {
            m_module_dir->mkdir((base));
        }
    }
}

void TractPopulationProcess::implementProcessTractPopulation()
{
    int i;
    if(m_para_m->getpara_computingSystem_comboBox() == "local")
    {
        m_script += "def stop(signal, frame):\n";
        m_script += "\tprint 'Signal received'\n";
        m_script += "\tfor tract in runningTract:\n";
        m_script += "\t\t(tract.process).terminate()\n";
        m_script += "\tsys.exit(0)\n\n";

        m_script += "def checkRunningTract(runningTract):\n";
        m_script += "\tfor tract in runningTract :\n";
        m_script += "\t\tif (tract.process).poll()==0:\n";
        m_script += "\t\t\trunningTract.remove(tract)\n\n";
    }

    if(m_para_m->getpara_computingSystem_comboBox() == "killdevil" || m_para_m->getpara_computingSystem_comboBox() == "killdevil interactive")
    {
        m_script += "def stop(signal, frame):\n";
        m_script += "\tprint 'Signal received'\n";
        m_script += "\tfor tract in runningTract:\n";
        m_script += "\t\tbkill_tract = subprocess.Popen(['bkill', tract.job])\n";
        m_script += "\tsys.exit(0)\n\n";

        m_script += "def checkRunningTract():\n";
        m_script += "\tfor tract in runningTract :\n";
        m_script += "\t\tbjobs_process = subprocess.Popen(['bjobs', tract.job], stdout=subprocess.PIPE)\n";
        m_script += "\t\tbjobs_output = bjobs_process.stdout.read()\n";
        m_script += "\t\tif 'DONE' in bjobs_output or 'EXIT' in bjobs_output :\n";
        m_script += "\t\t\trunningTract.remove(tract)\n\n";
    }

    m_script += "def run():\n";

    m_script += "\tlogger.info('=== Tract Postprocess ===')\n";

    m_script += "\tsignal.signal(signal.SIGINT, stop)\n";
    m_script += "\tsignal.signal(signal.SIGTERM, stop)\n\n";

    if(m_para_m->getpara_nbCores_spinBox() != 0)
    {
        m_script += "\tos.environ['ITK_GLOBAL_DEFAULT_NUMBER_OF_THREADS'] = str(nbCores)\n";
    }

    m_script += "\trunningTract = []\n\n";
    m_script += "\tglobal runningTract\n";

    if(m_para_m->getpara_computingSystem_comboBox() == "killdevil" || m_para_m->getpara_computingSystem_comboBox() == "killdevil interactive")
    {
        m_script += "\tpostprocess = namedtuple('postprocess', 'name job')\n\n";
    }
    else
    {
        m_script += "\tpostprocess = namedtuple('postprocess', 'name process')\n\n";
    }

    QString postProcess_script = m_processing_dir->filePath("SingleTractProcess.py");
    m_script += "\tpostProcess_script = '" + postProcess_script + "'\n";

    std::vector<QString>::iterator it;



    for (it = m_tractPopulation.begin(); it != m_tractPopulation.end(); ++it)
    {
        i = std::distance(m_tractPopulation.begin(), it);

        QString tract = *(it);
        QFileInfo fi(tract);
        QString base = fi.baseName();
        m_script += "\tname = '" + base + "'\n";
        m_script +="\ttract = '" + m_para_m->getpara_tracts_dir_lineEdit() + "/" + tract + "'\n";
        QString tract_path = m_module_dir->filePath(base);
        QDir* tract_dir = new QDir(tract_path);
        QString output_path = tract_dir->absolutePath();
        m_script += "\toutput_dir = '" + output_path +  "'\n";

        QString log_name = base + ".log";
        QString log = tract_dir->filePath(log_name);
        m_script += "\tlog = '" + log + "'\n\n";

        /*QString affine_path = atlas_dir->filePath(basename + "_Affine.txt");
        QString affineTemp_path = atlas_dir->filePath(basename + "_Affine.txt~");
        QString warp_path = atlas_dir->filePath(basename + "_Warp.nii.gz");
        QString inverseWarp_path = atlas_dir->filePath(basename + "_InverseWarp.nii.gz");*/

        // for keven lee cleaning
        //QString smoothedMask_path = atlas_dir->filePath(m_prefix + "mask-smoothed" + m_suffix + ".nrrd");

        /*m_unnecessaryFiles << affine_path;
        m_unnecessaryFiles << affineTemp_path;
        m_unnecessaryFiles << warp_path;
        m_unnecessaryFiles << inverseWarp_path;
        m_unnecessaryFiles << smoothedMask_path;*/

        if(m_para_m->getpara_computingSystem_comboBox() == "killdevil" || m_para_m->getpara_computingSystem_comboBox() == "killdevil interactive")
        {
            SubmitTractPostProcessJob(tract, i);
        }

        if (m_para_m->getpara_computingSystem_comboBox() == "local")
        {
            ExecuteTractPostProcess(tract, i);
        }
    }

    if(m_para_m->getpara_computingSystem_comboBox() == "local")
    {
        m_script += "\texit_codes = [(tract.process).wait() for tract in runningTract]\n";
    }

    if(m_para_m->getpara_computingSystem_comboBox() == "killdevil" || m_para_m->getpara_computingSystem_comboBox() == "killdevil interactive")
    {
        m_script += "\twhile len(runningTract) !=0 :\n";
        m_script += "\t\tcheckRunningTract()\n";
        m_script += "\t\ttime.sleep(1)\n";
    }

//    for( it = m_tractPopulation.begin(); it != m_tractPopulation.end(); ++it )
//    {
//        QString tract = *(it);
//        QFileInfo fi(tract);
//        QString base = fi.baseName();
//        QString tract_path = m_module_dir->filePath(base);
//        QDir* tract_dir = new QDir(tract_path);
//        QString output_path = tract_dir->absolutePath();

//        std::cout<<base.toStdString()<<std::endl;
//        QString tmp_file = output_path + "/" + base + "_tmp.txt";
//        std::cout<<fileExists(tmp_file)<<std::endl;
//        while( fileExists(tmp_file) == false )
//        {
//            std::cout<<"wait"<<std::endl;
//        }
//    }
    m_script += "\tlogger.info('')\n";
}




void TractPopulationProcess::SubmitTractPostProcessJob(QString tract, int i)
{
    QString QString_i;
    QString_i.setNum(i);

    QString tract_name = "tract_" + QString_i;

    QString args = "'bsub', '-q', 'hour', '-M', str(nbMemory), '-n', str(nbCores), '-R', 'span[hosts=1]', ";
    args += "'python', postProcess_script, name, tract, output_dir, displacementFieldPath, log";
    m_script += "\targs = [" + args + "]\n";
    m_script += "\tbsub_process = subprocess.Popen(args, stdout=subprocess.PIPE)\n";
    m_script += "\tbsub_output = bsub_process.stdout.read()\n";
    m_script += "\tlogger.info(bsub_output)\n";
    m_script += "\tjobID = re.search('(<{1})([0-9]{1,})(>{1})', bsub_output).group(2)\n";
    m_script += "\tlogger.info(jobID)\n";
    m_script += "\t" + tract_name + " = postprocess('" + tract + "', jobID)\n";
    m_script += "\trunningTract.append(" + tract_name + ")\n\n";

}

void TractPopulationProcess::ExecuteTractPostProcess(QString tract, int i)
{
    QString QString_i;
    QString_i.setNum(i);

    if(m_para_m->getpara_nbTractsProcessed_spinBox() != 0)
    {
        m_script += "\ttime.sleep(1)\n";
        m_script += "\twhile len(runningTract) >= nbTractProcessedMax :\n";
        m_script += "\t\tcheckRunningTract(runningTract)\n";
        m_script += "\t\ttime.sleep(1)\n";
    }

    QString process_name = "process_" + QString_i;
    QString tract_name = "tract_" + QString_i;

    QString command;
    command = "['python', postProcess_script, name, tract, output_dir, displacementFieldPath, log]";

    m_script += "\t" + tract_name + " = subprocess.Popen(" + command + ")\n";

    m_script += "\t" + process_name + " = postprocess('" + tract + "', " + tract_name + ")\n";
    m_script += "\trunningTract.append(" + process_name + ")\n\n";
}

void TractPopulationProcess::update()
{
    defineRefTracts();
    createTractDirectories();
    initializeScript();
    implementCheckFileExistence();
    implementProcessTractPopulation();
    writeScript();
}


