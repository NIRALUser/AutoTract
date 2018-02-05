#ifndef REGISTRATION_H
#define REGISTRATION_H

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

class Registration : public Script
{
   public:

   // Constructor
   Registration(QString module);
   // Set
   void setDisplacementFieldPath(QString path); //output

   // Implementing Script
   void initializeScript();
   void executeRegistration();
   void implementRun();

   // Updating & Getting Output
   void update();
   QString getDisplacementField();

   private:
   QString              m_displacementFieldPath;
};


#endif // REGISTRATION_H
