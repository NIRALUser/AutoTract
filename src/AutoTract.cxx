#include "AutoTractWindow.h"
#include "AutoTractDerivedWindow.h"
#include "AutoTractCLP.h"
#include <QApplication>
#include <QMap>
#include <QString>
#include "para_Save_AutoTract.h"
#include "para_Load_AutoTract.h"
#include "para_Model_AutoTract.h"
#include "soft_Save_AutoTract.h"
#include "soft_Load_AutoTract.h"
#include "soft_Model_AutoTract.h"


int main( int argc , char** argv )
{
    PARSE_ARGS;
    para_Model_AutoTract para_m;
    para_Save_AutoTract para_s;
    para_Load_AutoTract para_l;
    soft_Model_AutoTract soft_m;
    soft_Save_AutoTract soft_s;
    soft_Load_AutoTract soft_l;
    if(!executables.empty())
    {
        soft_l.load( soft_m, executables );
    }
    if(!parameters.empty())
    {
        para_l.load( para_m, parameters );
    }

    if(noGUI == false)
    {
        QApplication app( argc , argv );
        AutoTractDerivedWindow window ;
        window.SetParaLoad(&para_l);
        window.SetParaSave(&para_s);
        window.SetParaModel(&para_m);
        window.SetSoftLoad(&soft_l);
        window.SetSoftModel(&soft_m);
        window.SetSoftSave(&soft_s);
        window.show() ;
        if(parameters.empty() && executables.empty())
        {
            window.initSoftware();
            window.SyncModelStructureToUi("soft");
        }
        if(parameters.empty() && !executables.empty())
        {
            window.SyncModelStructureToUi("soft");

        }
        if(!parameters.empty() && executables.empty())
        {
            window.initSoftware();
            window.SyncModelStructureToUi();
        }
        if(!parameters.empty() && !executables.empty())
        {
            window.SyncModelStructureToUi();
        }
        window.SyncUiToModelStructure();
        return app.exec() ;
    }
    else
    {
        Pipeline* pipeline;
        pipeline = new Pipeline();
        pipeline->setPipelineParameters(&para_m);
        pipeline->setPipelineSoftwares(&soft_m);
        pipeline->writePipeline();
        pipeline->runPipeline();
        delete( pipeline );
        return 0;
    }
}
