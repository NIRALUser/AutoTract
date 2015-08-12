#ifndef MASKCREATION_H
#define MASKCREATION_H

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

class MaskCreation : public Script
{
   public:

   // Constructor
   MaskCreation(QString module);
   // Set
   void setOutputDirectory(QString dir);
   void setWMMaskPath(QString path);
   void setCSFMaskPath(QString path);
   // Updating & Getting Output
   void update();

   private:
      void executeMaskCreation();
      // Implementing Script
      void initializeScript();
      void implementRun();


   QString m_outputDir;
   QString m_CSFPath ;
   QString m_WMPath ;
};


#endif // MASKCREATION_H
