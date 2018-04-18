#ifndef SINGLETRACTPROCESS_H
#define SINGLETRACTPROCESS_H


// My Specific Librairies
#include "Script.h"
#include "para_Model_AutoTract.h"
#include "soft_Model_AutoTract.h"

class SingleTractProcess : public Script
{

   public:

   // Constructor //
   SingleTractProcess(QString module);

   // Set //
   void setLog(QString log_path);

   // Scripts //
   void initializeScript();
   void initializeLogging();

   // Register Atlas Script //
   void implementSingleTractProcess();
   void writeSingleTractProcess();

   // Output //
   void update();
   QString getOutput();


   private:

   // Log Path
   QString m_log_path;

   // Input //
};

#endif // SINGLETRACTPROCESS_H
