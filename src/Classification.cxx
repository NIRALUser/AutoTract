#include "Classification.h"


Classification::Classification(QString module) : Script(module)
{
}

void Classification::setDisplacementFieldPath(QString path)
{
    m_displacementFieldPath = path;
}

void Classification::setPostProcessPath(QString path)
{
    m_postProcessPath = path;
}

void Classification::defineRefTracts()
{
    std::vector<QString> ref_tracts;
    std::map<std::pair<unsigned long,QString>,bool> ref_tracts_list = m_para_m->getpara_ref_tracts_listWidget();
    std::map<std::pair<unsigned long,QString>,bool>::iterator it;
    std::cout << ref_tracts_list.size() << std::endl;
    for (it = ref_tracts_list.begin(); it!= ref_tracts_list.end(); it++)
    {
        if(it->second == 1)
        {
            ref_tracts.push_back( it->first.second );
        }
    }
    m_tractPopulation = ref_tracts;
}

//reads the qresource python script and replaces some parameters
void Classification::implementResourceScript(){
    //reading the file
    QFile scriptFile(":/src/scripts/Classification.py");
    scriptFile.open(QIODevice::ReadOnly | QIODevice::Text);
    
    //parsing and replacing all '@QWERTYUIO@' parameters
    QRegExp rx("@\\w*@");
    while(!scriptFile.atEnd()){ //line by line
        QString line = scriptFile.readLine();

        if(rx.indexIn(line) != -1){ //if match in line
            QString cap(rx.cap());
            if(cap == QString("@inputList@"))
                line.replace(QString("['") + cap + QString("']"), m_scriptParameters[cap]);

            if(m_scriptParameters.count(cap)) //if key is present in the dictionary, replace the key by the value
                line.replace(cap, m_scriptParameters[cap]);
        }
        m_script += line;
    }
}

void Classification::setLogPath(QString path)
{
    m_log_path = path;
}

//generates a dictionary to be used for replacement of parameters in the python script
void Classification::defineScriptParameters(){ 
    //simple paths
    m_scriptParameters["@inputDirectory@"] = m_postProcessPath;
    m_scriptParameters["@outputDirectory@"] = m_module_path;
    m_scriptParameters["@displacementFieldPath@"] = m_displacementFieldPath;
    m_scriptParameters["@checkpointArchivePath@"] = m_para_m->getpara_trafic_model_dir_lineEdit();
    m_scriptParameters["@log@"] = m_log_path;
    m_scriptParameters["@traficDir@"] = m_soft_m->getsoft_Trafic_lineEdit();
    
    //flags
    if(m_para_m->getpara_no_tract_output_trafic_checkBox())
        m_scriptParameters["@generateTracts@"] = "False";
    else m_scriptParameters["@generateTracts@"] = "True";

    //list
    QString inputList = "['";
    std::vector<QString>::iterator it;
    for (it = m_tractPopulation.begin(); it != m_tractPopulation.end(); ++it){
        QString tractFilename = *it;
        QFileInfo fi(tractFilename);
        QString base = fi.baseName();
        inputList += base + "/" + base + "_cleanEnds.vtp','";
    }
    m_scriptParameters["@inputList@"] = inputList + "']";
}

void Classification::update()
{
    defineRefTracts();
    defineScriptParameters();
    implementResourceScript();

    //util functions
    implementStop();
    implementCheckFileExistence();

    writeScript();
}

