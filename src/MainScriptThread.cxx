#include "MainScriptThread.h"

MainScriptThread::MainScriptThread() : QThread()
{
    setParent(0);
    moveToThread(this);
}

void MainScriptThread::setPipeline(Pipeline* pipeline) {m_pipeline=pipeline;}


void MainScriptThread::run()
{
   m_pipeline->runPipeline();
}

void MainScriptThread::terminate()
{
   m_pipeline->stopPipeline(); 

   QThread::terminate();
}

