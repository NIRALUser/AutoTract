#ifndef DEF_MainScriptThread
#define DEF_MainScriptThread

#include <iostream>

#include <QString>
#include <QObject>
#include <QThread>

#include "Pipeline.h"


class MainScriptThread : public QThread 
{
   Q_OBJECT;

   public:

   MainScriptThread();

   void setPipeline(Pipeline* pipeline);

   void terminate(); 

   private:

   void run();

   Pipeline* m_pipeline;

}; 

#endif

