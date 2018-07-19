#ifndef CLASSIFICATION_H
#define CLASSIFICATION_H

// General Librairies
#include <iostream>
#include <fstream>

// Qt Librairies
#include <QString>
#include <QDir>

// My specific librairies
#include "Script.h"
#include "para_Model_AutoTract.h"
#include "soft_Model_AutoTract.h"

#include <vector>
#include <map>

class Classification : public Script
{
   public:

   // Constructor
   Classification(QString module);

   // Setters
   void setDisplacementFieldPath(QString path); //output
   void setPostProcessPath(QString path);
   void setLogPath(QString path);
   
   // Implementing Script
   void defineScriptParameters();
   void implementResourceScript();

   // Updating & Getting Output
   void update();

   void defineRefTracts();

   private:
      
   QString m_displacementFieldPath;
   QString m_postProcessPath;
   QString m_log_path;
   std::vector<QString> m_tractPopulation;
   std::map<QString, QString> m_scriptParameters;
};


#endif // CLASSIFICATION_H
