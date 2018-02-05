#ifndef AutoTractWindow_h
#define AutoTractWindow_h

#include "ui_AutoTract.h"
#include "para_Save_AutoTract.h"
#include "para_Load_AutoTract.h"
#include "para_Model_AutoTract.h"
#include "soft_Save_AutoTract.h"
#include "soft_Load_AutoTract.h"
#include "soft_Model_AutoTract.h"
#include "MainScriptThread.h"

class AutoTractWindow :public QMainWindow, public Ui::MainWindow
{
    Q_OBJECT
public:
    AutoTractWindow( QWidget * parent = 0, Qt::WFlags f = 0);

    void SetParaModel( para_Model_AutoTract* para_m );
    void SetParaSave( para_Save_AutoTract* para_s );
    void SetParaLoad( para_Load_AutoTract* para_l );
    void SetSoftModel( soft_Model_AutoTract* soft_m );
    void SetSoftSave( soft_Save_AutoTract* soft_s );
    void SetSoftLoad( soft_Load_AutoTract* soft_l );

public slots:
    void Save_Parameter_Configuration( std::string filename );
    void Load_Parameter_Configuration( std::string filename );
    void Save_Software_Configuration( std::string filename );
    void Load_Software_Configuration( std::string filename );
    void SyncUiToModelStructure();
    void SyncModelStructureToUi();
    void SyncUiToModelStructure(QString prefix);
    void SyncModelStructureToUi(QString prefix);

public:
    para_Model_AutoTract* m_para_m;
    para_Save_AutoTract* m_para_s;
    para_Load_AutoTract* m_para_l;
    soft_Model_AutoTract* m_soft_m;
    soft_Save_AutoTract* m_soft_s;
    soft_Load_AutoTract* m_soft_l;

private:
    bool m_sync;
};

#endif
