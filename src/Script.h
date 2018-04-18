#ifndef DEF_Script
#define DEF_Script

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <stdlib.h>

#include <QString>
#include <QDir>
#include <QMap>

#include "para_Model_AutoTract.h"
#include "soft_Model_AutoTract.h"

class Script
{
   public :

   // Constructor
   Script(QString module);

   // Set Functions
   void setProcessingDirectory(QString processing_path);
//   void SetExecutablesMap( QMap<QString, QString> executables_map);
//   void SetParametersMap( QMap<QString, QString> parameters_map);
   void setScriptParameters(para_Model_AutoTract *para_m);
   void setScriptSoftwares(soft_Model_AutoTract *soft_m);
   void setModuleDirectory(QString module_path);
   void setOverwriting(bool overwriting);
   void setStoppingIfError(bool stoppingIfError);

   // Initialize Script
   void definePython();
   void importGeneralModules();
//   void defineExecutable(QString executable);
//   void defineParameter(QString parameter);

   // Implement Functions
   void implementStop();
   void implementCheckFileExistence();
   void implementExecute();
   void implementExecutePipe();

   // Execute Command Line
   void checkFinalOutputs();
   void execute();
   void executePipe();

   // Write XML Files
   void addSubElement(QString element, QString pythonName, QString XMLname, QString value);

   // Write Script
   QString str(QString str);
   QString variable(QString variable_name);
   QString listToString(QStringList argumentsList);
   void writeScript();

   // Clean-Up Module Directory
   void cleanUp();

   protected :

   // Module & Script Names
   QString m_module_name;
   QString m_script_name;

   // Processing Directory
   QDir* m_processing_dir;

   // Module Directory
   QString m_module_path;
   QDir* m_module_dir;

   // Executable and Parameters
   para_Model_AutoTract* m_para_m;
   soft_Model_AutoTract* m_soft_m;
//   QMap<QString, QString> m_executables_map;
//   QMap<QString, QString> m_parameters_map;

   // Computation
   bool  m_overwriting;
   bool  m_stoppingIfError;


   // Execute parameters
   QString m_test;
   QString m_indentation;

   QMap<QString, QString> m_inputs;
   QMap<QString, QString> m_inputsTests;
   QMap<QString, QStringList> m_argsTests;

   QMap<QString, QString> m_outputs;

   QString m_returnValue;

   QStringList m_argumentsList;
   QStringList m_argumentsList_1;
   QStringList m_argumentsList_2;
   QString m_log;

   // Script
   QString m_script;

   // Unnecessary Files
   QStringList m_unnecessaryFiles;

};

#endif
