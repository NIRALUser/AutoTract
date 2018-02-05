#include "Script.h"

Script::Script(QString module)
{
   m_module_name = module;
   m_script_name = module + ".py";
}

void Script::setProcessingDirectory(QString processing_path)
{
   m_processing_dir = new QDir(processing_path);
}
void Script::setModuleDirectory(QString module_path)
{
   m_module_path = module_path;
   m_module_dir = new QDir(module_path);
}
//void Script::SetExecutablesMap( QMap<QString, QString> executables_map)
//{
//    QMap<QString, QString>::iterator i;
//    for (i = executables_map.begin(); i != executables_map.end(); ++i)
//    {
//        m_executables_map[i.key()] =  i.value() ;
//    }
//}

//void Script::SetParametersMap( QMap<QString, QString> parameters_map)
//{
//    QMap<QString, QString>::iterator i;
//    for (i = parameters_map.begin(); i != parameters_map.end(); ++i)
//    {
//        m_parameters_map[i.key()] =  i.value() ;
//    }
//}

void Script::setScriptParameters(para_Model_AutoTract* para_m)
{
    m_para_m = para_m;
}
void Script::setScriptSoftwares(soft_Model_AutoTract* soft_m)
{
    m_soft_m = soft_m;
}
void Script::setOverwriting(bool overwriting)
{
   m_overwriting = overwriting;
}
void Script::setStoppingIfError(bool stoppingIfError)
{
   m_stoppingIfError = stoppingIfError;
}

QString Script::variable(QString variable_name)
{
   return "\" + " + variable_name + " + \"";
}

QString Script::str(QString str)
{
   return "\"" + str + "\"";
}

QString Script::listToString(QStringList argumentsList)
{
   QString arguments = argumentsList[0];

   QStringList::const_iterator it;
   for (it = argumentsList.constBegin()+1; it != argumentsList.constEnd(); ++it)
   {
      arguments += "," + (*it) + "";
   }

   return "[" + arguments + "]";
}

void Script::definePython()
{
   m_script = "";
   m_script += "#!/usr/bin/env python\n\n";
}

void Script::importGeneralModules()
{
   m_script += "import os\n";
   m_script += "import sys\n";
   m_script += "import logging\n";
   m_script += "import signal\n";
   m_script += "import subprocess\n\n";
}

//void Script::defineExecutable(QString executable)
//{
//    m_script += executable + " = '" + m_executables_map[executable] + "'\n";
//}
//void Script::defineParameter(QString parameter)
//{
//    m_script += parameter + " = '" + m_parameters_map[parameter] + "'\n";
//}
void Script::implementStop()
{
   m_script += "def stop(signal, frame):\n";
   m_script += "\tprint '*************** Signal stop received! ******************'\n";
   m_script += "\trunningProcess.terminate()\n";
   m_script += "\tsys.exit(0)\n\n";
}

void Script::implementCheckFileExistence()
{
   m_script += "def checkFileExistence(fileName):\n";
   m_script += "\ttry:\n";
   m_script += "\t\twith open(fileName):\n";
   m_script += "\t\t\treturn True\n";
   m_script += "\texcept IOError:\n";
   m_script += "\t\treturn False\n\n";
}

void Script::implementExecute()
{
   m_script += "def execute(args):\n";

   m_script += "\tglobal runningProcess\n";

   m_script += "\tlogger.debug(subprocess.list2cmdline(args))\n";
   m_script += "\trunningProcess = subprocess.Popen(args,stdout=subprocess.PIPE,stderr=subprocess.PIPE)\n";

   m_script += "\tstdout = ''\n";

   m_script += "\twhile True:\n";
   m_script += "\t\tline = runningProcess.stdout.readline()\n";
   m_script += "\t\tif line:\n";
   m_script += "\t\t\tlogger.debug(line.rstrip())\n";
   m_script += "\t\t\tstdout = stdout + line\n";
   m_script += "\t\tif not line: break\n";

   m_script += "\twhile True:\n";
   m_script += "\t\tline = runningProcess.stderr.readline()\n";
   m_script += "\t\tif line:\n";
   m_script += "\t\t\tlogger.error(line.rstrip())\n";
   m_script += "\t\tif not line: break\n";

   m_script += "\trunningProcess.wait()\n";

   if (m_stoppingIfError)
   {
      m_script += "\tif runningProcess.returncode != 0 :\n";
      m_script += "\t\tlogger.error('Error(s) occured, pipeline stopped!')\n";
      m_script += "\t\tsys.exit(1)\n";
   }
   m_script += "\tlogger.debug('')\n\n";
   m_script += "\treturn stdout\n\n";
}

void Script::implementExecutePipe()
{
   m_script += "def executePipe(args1, args2):\n";

   m_script += "\tglobal runningProcess\n";

   m_script += "\tlogger.debug(subprocess.list2cmdline(args1) + ' | ' + subprocess.list2cmdline(args2))\n";

   m_script += "\tpreProcess = subprocess.Popen(args1,stdout=subprocess.PIPE,stderr=subprocess.PIPE)\n";
   m_script += "\trunningProcess = subprocess.Popen(args2,stdin=preProcess.stdout,stdout=subprocess.PIPE,stderr=subprocess.PIPE)\n";

   m_script += "\twhile True:\n";
   m_script += "\t\tline = runningProcess.stdout.readline()\n";
   m_script += "\t\tif line:\n";
   m_script += "\t\t\tlogger.debug(line.rstrip())\n";
   m_script += "\t\tif not line: break\n";

   m_script += "\twhile True:\n";
   m_script += "\t\tline = runningProcess.stderr.readline()\n";
   m_script += "\t\tif line:\n";
   m_script += "\t\t\tlogger.error(line.rstrip())\n";
   m_script += "\t\tif not line: break\n";

   m_script += "\trunningProcess.wait()\n";

   if (m_stoppingIfError)
   {
      m_script += "\tif runningProcess.returncode != 0 :\n";
      m_script += "\t\tlogger.error('Error(s) occured, pipeline stopped!')\n";
      m_script += "\t\tsys.exit(1)\n";
   }
   m_script += "\tlogger.debug('')\n\n";
   m_script += "\tstdout, stderr = runningProcess.communicate()\n";
   m_script += "\treturn stdout\n\n";
}

void Script::checkFinalOutputs()
{
   QMap<QString, QString>::iterator it = m_outputs.begin();

   if(!m_overwriting)
   {
      QString checking = "\tif ";

      if(!m_outputs.isEmpty())
      {
         for(it = m_outputs.begin(); it != m_outputs.end(); ++it )
         {
            m_script += "\t" + it.key() + " = '" + it.value() + "'\n";

            checking += "checkFileExistence(" + it.key() + ")==True";

            if(it+1 != m_outputs.end())
            {
               checking += " and ";
            }
         }

         m_script += checking + ":\n";

         m_script += "\t\tlogger.info('" + m_log + " -> Skipped')\n";
         m_script += "\t\tlogger.info('')\n";
         m_script += "\t\treturn\n\n";
      }
   }
   m_outputs.clear();
}

void Script::execute()
{
   QMap<QString, QString>::iterator it;

   if(!m_test.isEmpty())
   {
      m_script += "\tif " + m_test + " :\n";
      m_indentation = "\t\t";
   }
   else
   {
      m_indentation = "\t";
   }

   QString checking = m_indentation + "if ";

   if(!m_outputs.isEmpty())
   {
      // Outputs
      for(it = m_outputs.begin(); it != m_outputs.end(); ++it )
      {
         m_script += m_indentation + it.key() + " = '" + it.value() + "'\n";

         checking += "checkFileExistence(" + it.key() + ")==False";

         if(it+1 != m_outputs.end())
         {
            checking += " or ";
         }
      }

      if(!m_overwriting)
      {
         m_script += checking + ":\n";
      }
   }

   if(!m_outputs.isEmpty() && !m_overwriting)
   {
      m_script += m_indentation + "\tlogger.info('" + m_log + "...')\n";

      // Inputs
      for(it = m_inputs.begin(); it != m_inputs.end(); ++it )
      {
         if(m_inputsTests.contains(it.key()))
         {
            m_script += m_indentation + "\tif " + m_inputsTests[it.key()] + " :\n";
            m_script += m_indentation + "\t\t" + it.key() + " = '" + it.value() + "'\n";
         }
         else
         {
            m_script += m_indentation + "\t" + it.key() + " = '" + it.value() + "'\n";
         }
      }

      if(m_argsTests.size() > 0)
      {
         QMap<QString, QStringList>::iterator it_args;
         for(it_args = m_argsTests.begin(); it_args != m_argsTests.end(); ++it_args )
         {
            if(it_args == m_argsTests.begin())
            {
               m_script += m_indentation + "\tif not " + it_args.key() + " :\n";
            }

            else
            {
               m_script += m_indentation + "\telif not " + it_args.key() + " :\n";
            }

            m_script += m_indentation + "\t\targs = " + listToString(it_args.value()) + "\n";
            m_script += m_indentation + "\t\texecute(args)\n";
         }

         m_script += m_indentation + "\telse :\n";
         m_script += m_indentation + "\t\targs = " + listToString(m_argumentsList) + "\n";

         if(!m_returnValue.isEmpty())
         {
            m_script += m_indentation + "\t\t" + m_returnValue + " = execute(args)\n";
         }
         else
         {
            m_script += m_indentation + "\t\texecute(args)\n";
         }
      }
      else
      {
         m_script += m_indentation + "\targs = " + listToString(m_argumentsList) + "\n";
         if(!m_returnValue.isEmpty())
         {
            m_script += m_indentation + "\t" + m_returnValue + " = execute(args)\n";
         }
         else
         {
            m_script += m_indentation + "\texecute(args)\n";
         }
      }

      m_script += m_indentation + "else:\n";
      m_script += m_indentation + "\tlogger.info('" + m_log + " -> Skipped')\n\n";
   }

   else
   {
      m_script += m_indentation + "logger.info('" + m_log + "...')\n";

      // Inputs
      for(it = m_inputs.begin(); it != m_inputs.end(); ++it )
      {
         m_script += "\t" + it.key() + " = '" + it.value() + "'\n";
      }

      m_script += m_indentation + "args = " + listToString(m_argumentsList) + "\n";
      if(!m_returnValue.isEmpty())
      {
         m_script += m_indentation + "" + m_returnValue + " = execute(args)\n";
      }
      else
      {
         m_script += m_indentation + "execute(args)\n";
      }
   }

   m_test.clear();
   m_inputs.clear();
   m_inputsTests.clear();
   m_outputs.clear();
   m_returnValue.clear();
   m_argumentsList.clear();
   m_argsTests.clear();
}

void Script::executePipe()
{
   QMap<QString, QString>::iterator it = m_outputs.begin();

   QString checking = "\tif ";

   if(!m_outputs.isEmpty())
   {
      // Outputs
      for(it = m_outputs.begin(); it != m_outputs.end(); ++it )
      {
         m_script += "\t" + it.key() + " = '" + it.value() + "'\n";
         checking += "checkFileExistence(" + it.key() + ")==False";

         if(it+1 != m_outputs.end())
         {
            checking += " and ";
         }
      }

      if(!m_overwriting)
      {
         m_script += checking + ":\n";
      }
   }

   if(!m_outputs.isEmpty() && !m_overwriting)
   {
      m_script += "\t\tlogger.info('" + m_log + "...')\n";

      // Inputs
      for(it = m_inputs.begin(); it != m_inputs.end(); ++it )
      {
         m_script += "\t\t" + it.key() + " = '" + it.value() + "'\n";
      }

      m_script += "\t\targs1 = " + listToString(m_argumentsList_1) + "\n";
      m_script += "\t\targs2 = " + listToString(m_argumentsList_2) + "\n";
      m_script += "\t\texecutePipe(args1, args2)\n";

      m_script += "\telse:\n";
      m_script += "\t\tlogger.info('" + m_log + " -> Skipped')\n\n";

   }

   else
   {
      m_script += "\tlogger.info('" + m_log + "...')\n";

      // Inputs
      for(it = m_inputs.begin(); it != m_inputs.end(); ++it )
      {
         m_script += "\t" + it.key() + " = '" + it.value() + "'\n";
      }

      m_script += "\targs1 = " + listToString(m_argumentsList_1) + "\n";
      m_script += "\targs2 = " + listToString(m_argumentsList_2) + "\n";
      m_script += "\texecutePipe(args1, args2)\n";
   }

   m_inputs.clear();
   m_outputs.clear();
   m_argumentsList.clear();
   m_argumentsList_1.clear();
   m_argumentsList_2.clear();
}

void Script::addSubElement(QString element, QString pythonName, QString XMLname, QString value)
{
   m_script += "\t\t" + pythonName + " = SubElement(" + element + ", '" + XMLname + "')\n";
   m_script += "\t\t" + pythonName + ".text = '" + value + "'\n";
}

void Script::writeScript()
{
   QString script_path = m_processing_dir->filePath(m_script_name);

   std::ofstream* script_stream = new std::ofstream((script_path.toStdString()).c_str(), std::ios::out | std::ios::trunc);
   *script_stream << m_script.toStdString() << std::endl;

   script_stream->close();
}

void Script::cleanUp()
{
   for (int i = 0; i < m_unnecessaryFiles.size(); ++i)
   {
      QFile(m_unnecessaryFiles.at(i)).remove();
   }
}
