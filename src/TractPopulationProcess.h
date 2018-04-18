#ifndef TRACTPOPULATIONPROCESS_H
#define TRACTPOPULATIONPROCESS_H

// General Librairies
#include <iostream>
#include <fstream>
#include <vector>

// Qt Librairies
#include <QString>
#include <QDir>
#include <QFileInfo>

// My specific librairies
#include "Script.h"
#include "para_Model_AutoTract.h"
#include "soft_Model_AutoTract.h"

class TractPopulationProcess : public Script
{
   public:

   // Constructor
   TractPopulationProcess(QString module);
   // Set


   // Implementing Script
   void initializeScript();
   void defineRefTracts();
   void createTractDirectories();
   void implementProcessTractPopulation();
   void SetDisplacementFieldPath(QString displacementFieldPath);

   void SubmitTractPostProcessJob(QString tract, int i);
   void ExecuteTractPostProcess(QString tract, int i);

   // Updating & Getting Output
   void update();
   bool fileExists(QString path);

   private:
   std::vector<QString> m_tractPopulation;
   QString m_displacementFieldPath;
};


#endif // TRACTPOPULATIONPROCESS_H
