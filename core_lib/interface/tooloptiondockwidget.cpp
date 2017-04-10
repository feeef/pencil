#include <QLabel>
#include <QToolButton>
#include <QCheckBox>
#include <QSpinBox>
#include <QGridLayout>
#include <QGroupBox>
#include <QRadioButton>
#include <QSettings>
#include <QDebug>
#include "spinslider.h"
#include "toolmanager.h"
#include "tooloptiondockwidget.h"
#include "editor.h"
#include "util.h"
#include "layer.h"
#include "layermanager.h"

ToolOptionWidget::ToolOptionWidget( QWidget* parent ) : BaseDockWidget( parent )
{
    setWindowTitle( tr( "Options", "Window title of tool options like pen width, feather etc.." ) );
}

ToolOptionWidget::~ToolOptionWidget()
{
}

void ToolOptionWidget::initUI()
{
    createUI();
}

void ToolOptionWidget::updateUI()
{
    BaseTool* currentTool = editor()->tools()->currentTool();
    Q_ASSERT( currentTool );

    disableAllOptions();

    mSizeSlider->setVisible( currentTool->isPropertyEnabled( WIDTH ) );
    mBrushSpinBox->setVisible( currentTool->isPropertyEnabled( WIDTH) );
    mFeatherSlider->setVisible( currentTool->isPropertyEnabled( FEATHER ) );
    mUseFeatherBox->setVisible( currentTool->isPropertyEnabled( FEATHER ) );
    mFeatherSpinBox->setVisible( currentTool->isPropertyEnabled( FEATHER) );
    mUseBezierBox->setVisible( currentTool->isPropertyEnabled( BEZIER ) );
    mUsePressureBox->setVisible( currentTool->isPropertyEnabled( PRESSURE ) );
    mMakeInvisibleBox->setVisible( currentTool->isPropertyEnabled( INVISIBILITY ) );
    mPreserveAlphaBox->setVisible( currentTool->isPropertyEnabled( PRESERVEALPHA ) );
    mUseAABox->setVisible(currentTool->isPropertyEnabled( ANTI_ALIASING ) );
    mInpolLevelsBox->setVisible(currentTool->isPropertyEnabled( INTERPOLATION ) );

    auto currentLayerType = editor()->layers()->currentLayer()->type();

    if(currentLayerType == Layer::VECTOR)
    {
        mVectorMergeBox->setVisible( currentTool->isPropertyEnabled( VECTORMERGE) );
    }

    const Properties& p = currentTool->properties;

    setPenWidth( p.width );
    setPenFeather( p.feather );
    setPressure( p.pressure );
    setPenInvisibility( p.invisibility );
    setPreserveAlpha( p.preserveAlpha );
    setVectorMergeEnabled( p.vectorMergeEnabled );
    setAA(p.useAA);
    setInpolLevel(p.inpolLevel);
}

void ToolOptionWidget::createUI()
{
    setMinimumWidth( 115 );
    setMaximumWidth(300);

    QFrame* optionGroup = new QFrame();
    QGridLayout* pLayout = new QGridLayout();
    pLayout->setMargin( 8 );
    pLayout->setSpacing( 8 );

    QSettings settings( PENCIL2D, PENCIL2D );

    mSizeSlider = new SpinSlider( tr( "Brush" ), SpinSlider::EXPONENT, SpinSlider::INTEGER, 1, 200, this );
    mSizeSlider->setValue( settings.value( "brushWidth" ).toDouble() );
    mSizeSlider->setToolTip( tr( "Set Pen Width <br><b>[SHIFT]+drag</b><br>for quick adjustment" ) );

    mBrushSpinBox = new QSpinBox(this);
    mBrushSpinBox->setRange(1,200);
    mBrushSpinBox->setValue(settings.value( "brushWidth" ).toDouble() );

    mFeatherSlider = new SpinSlider( tr( "Feather" ), SpinSlider::LOG, SpinSlider::INTEGER, 2, 64, this );
    mFeatherSlider->setValue( settings.value( "brushFeather" ).toDouble() );
    mFeatherSlider->setToolTip( tr( "Set Pen Feather <br><b>[CTRL]+drag</b><br>for quick adjustment" ) );

    mFeatherSpinBox = new QSpinBox(this);
    mFeatherSpinBox->setRange(2,64);
    mFeatherSpinBox->setValue(settings.value( "brushFeather" ).toDouble() );

    mUseFeatherBox = new QCheckBox( tr( "Use Feather" ) );
    mUseFeatherBox->setToolTip( tr( "Enable or disable feathering" ) );
    mUseFeatherBox->setFont( QFont( "Helvetica", 10 ) );
    mUseFeatherBox->setChecked( settings.value( "brushUseFeather" ).toBool() );

    mUseBezierBox = new QCheckBox( tr( "Bezier" ) );
    mUseBezierBox->setToolTip( tr( "Bezier curve fitting" ) );
    mUseBezierBox->setFont( QFont( "Helvetica", 10 ) );
    mUseBezierBox->setChecked( false );

    mUsePressureBox = new QCheckBox( tr( "Pressure" ) );
    mUsePressureBox->setToolTip( tr( "Size with pressure" ) );
    mUsePressureBox->setFont( QFont( "Helvetica", 10 ) );
    mUsePressureBox->setChecked( true );

    mUseAABox = new QCheckBox( tr( "Anti-Aliasing" ) );
    mUseAABox->setToolTip( tr( "Enable Anti-Aliasing" ) );
    mUseAABox->setFont( QFont( "Helvetica", 10 ) );
    mUseAABox->setChecked( true );

    mInpolLevelsBox = new QGroupBox ( tr( "Stabilization level" ) );
    mInpolLevelsBox->setFlat(true);
    mInpolLevelsBox->setFont(QFont( "Helvetica", 10 ) );
    mInpolLevelsBox->setStyleSheet(
                                    "QGroupBox"
                                    "{"
                                        "margin-top: 1.0em"
                                    "}"
                                    "QGroupBox::title"
                                    "{"
                                        "subcontrol-origin: margin;"
                                        "left: 5px;"
                                        "padding: 0.7em 3px 0 3px;"
                                    "}");

    mNoInpol = new QRadioButton ( tr( "" ) );
    mNoInpol->setToolTip( tr( "No line interpolation" ) );
    mNoInpol->setFont( QFont( "Helvetica", 10) );
    mNoInpol->setChecked ( true );

    mSimpleInpol = new QRadioButton (tr( "" ) );
    mSimpleInpol->setToolTip( tr( "Simple line interpolation" ) );
    mSimpleInpol->setChecked ( false );

    mStrongInpol = new QRadioButton (tr( "" ) );
    mStrongInpol->setToolTip( tr( "Strong line interpolation" ) );
    mStrongInpol->setChecked ( false );

//    mExtremeInpol = new QRadioButton (tr( "" ) );
//    mExtremeInpol->setToolTip( tr( "Extreme line interpolation" ) );
//    mExtremeInpol->setFont( QFont( "Helvetica", 10) );
//    mExtremeInpol->setChecked ( false );

    QGridLayout* inpolLayout = new QGridLayout();
    inpolLayout->addWidget( mNoInpol, 16, 0, 2, 1 );
    inpolLayout->addWidget( mSimpleInpol, 16, 1, 2, 1 );
    inpolLayout->addWidget( mStrongInpol, 16, 2, 2, 1 );
//    inpolLayout->addWidget( mExtremeInpol, 16, 3, 2, 1 );
    mInpolLevelsBox->setLayout(inpolLayout);
    inpolLayout->setSpacing(2);

    mMakeInvisibleBox = new QCheckBox( tr( "Invisible" ) );
    mMakeInvisibleBox->setToolTip( tr( "Make invisible" ) );
    mMakeInvisibleBox->setFont( QFont( "Helvetica", 10 ) );
    mMakeInvisibleBox->setChecked( false );

    mPreserveAlphaBox = new QCheckBox( tr( "Alpha" ) );
    mPreserveAlphaBox->setToolTip( tr( "Preserve Alpha" ) );
    mPreserveAlphaBox->setFont( QFont( "Helvetica", 10 ) );
    mPreserveAlphaBox->setChecked( false );

    mVectorMergeBox = new QCheckBox( tr( "Merge" ) );
    mVectorMergeBox->setToolTip( tr( "Merge vector lines when they are close together" ) );
    mVectorMergeBox->setFont( QFont( "Helvetica", 10 ) );
    mVectorMergeBox->setChecked( false );

    pLayout->addWidget( mSizeSlider, 1, 0, 1, 2 );
    pLayout->addWidget( mBrushSpinBox, 1, 2, 1, 2);
    pLayout->addWidget( mFeatherSlider, 2, 0, 1, 2 );
    pLayout->addWidget( mFeatherSpinBox, 2, 2, 1, 2 );
    pLayout->addWidget( mUseFeatherBox, 3, 0, 1, 2 );
    pLayout->addWidget( mUseBezierBox, 4, 0, 1, 2 );
    pLayout->addWidget( mUsePressureBox, 5, 0, 1, 2 );
    pLayout->addWidget( mUseAABox, 6, 0, 1, 2);
    pLayout->addWidget( mPreserveAlphaBox, 7, 0, 1, 2 );
    pLayout->addWidget( mMakeInvisibleBox, 8, 0, 1, 2 );
    pLayout->addWidget( mVectorMergeBox, 9, 0, 1, 2 );
    pLayout->addWidget( mInpolLevelsBox, 10, 0, 1, 4);

    pLayout->setRowStretch( 17, 1 );

    optionGroup->setLayout( pLayout );

    setWidget( optionGroup );
}

void ToolOptionWidget::makeConnectionToEditor( Editor* editor )
{
    auto toolManager = editor->tools();

    connect( mUseBezierBox, &QCheckBox::clicked, toolManager, &ToolManager::setBezier );
    connect( mUsePressureBox, &QCheckBox::clicked, toolManager, &ToolManager::setPressure );
    connect( mMakeInvisibleBox, &QCheckBox::clicked, toolManager, &ToolManager::setInvisibility );
    connect( mPreserveAlphaBox, &QCheckBox::clicked, toolManager, &ToolManager::setPreserveAlpha );

    connect( mSizeSlider, &SpinSlider::valueChanged, toolManager, &ToolManager::setWidth );
    connect( mFeatherSlider, &SpinSlider::valueChanged, toolManager, &ToolManager::setFeather );

    connect( mBrushSpinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), toolManager, &ToolManager::setWidth );
    connect( mFeatherSpinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), toolManager, &ToolManager::setFeather );

    connect( mUseFeatherBox, &QCheckBox::clicked, toolManager, &ToolManager::setUseFeather );

    connect( mVectorMergeBox, &QCheckBox::clicked, toolManager, &ToolManager::setVectorMergeEnabled );
    connect( mUseAABox, &QCheckBox::clicked, toolManager, &ToolManager::setAA );

    connect( mNoInpol, &QRadioButton::clicked, toolManager, &ToolManager::noInpolSelected);
    connect( mSimpleInpol, &QRadioButton::clicked, toolManager, &ToolManager::SimplepolSelected);
    connect( mStrongInpol, &QRadioButton::clicked, toolManager, &ToolManager::StrongpolSelected);
    connect( mExtremeInpol, &QRadioButton::clicked, toolManager, &ToolManager::ExtremepolSelected);


    connect( toolManager, &ToolManager::toolChanged, this, &ToolOptionWidget::onToolChanged );
    connect( toolManager, &ToolManager::toolPropertyChanged, this, &ToolOptionWidget::onToolPropertyChanged );
}

void ToolOptionWidget::onToolPropertyChanged( ToolType, ToolPropertyType ePropertyType )
{
    const Properties& p = editor()->tools()->currentTool()->properties;

    switch ( ePropertyType )
    {
        case WIDTH:
            setPenWidth( p.width );
            break;
        case FEATHER:
            setPenFeather( p.feather );
            break;
        case PRESSURE:
            setPressure( p.pressure );
            break;
        case INVISIBILITY:
            setPenInvisibility( p.invisibility );
            break;
        case PRESERVEALPHA:
            setPreserveAlpha( p.preserveAlpha );
            break;
        case VECTORMERGE:
            setVectorMergeEnabled(p.vectorMergeEnabled);
            break;
        case ANTI_ALIASING:
            setAA(p.useAA);
            break;
        case INTERPOLATION:
            setInpolLevel(p.inpolLevel);
            break;
    }
}

void ToolOptionWidget::onToolChanged( ToolType )
{
    updateUI();
}

void ToolOptionWidget::setPenWidth( qreal width )
{
    SignalBlocker b( mSizeSlider );
    mSizeSlider->setEnabled( true );
    mSizeSlider->setValue( width );

    SignalBlocker b2( mBrushSpinBox );
    mBrushSpinBox->setEnabled( true );
    mBrushSpinBox->setValue( width );
}

void ToolOptionWidget::setPenFeather( qreal featherValue )
{
    SignalBlocker b( mFeatherSlider );
    mFeatherSlider->setEnabled( true );
    mFeatherSlider->setValue( featherValue );
    
    SignalBlocker b2( mFeatherSpinBox );
    mFeatherSpinBox->setEnabled( true );
    mFeatherSpinBox->setValue( featherValue );
}

void ToolOptionWidget::setPenInvisibility( int x )
{
    SignalBlocker b( mMakeInvisibleBox );
    mMakeInvisibleBox->setEnabled( true );
    mMakeInvisibleBox->setChecked( x > 0 );
}

void ToolOptionWidget::setPressure( int x )
{
    SignalBlocker b( mUsePressureBox );
    mUsePressureBox->setEnabled( true );
    mUsePressureBox->setChecked( x > 0 );
}

void ToolOptionWidget::setPreserveAlpha( int x )
{
    qDebug() << "Setting - Preserve Alpha=" << x;

    SignalBlocker b( mPreserveAlphaBox );
    mPreserveAlphaBox->setEnabled( true );
    mPreserveAlphaBox->setChecked( x > 0 );
}

void ToolOptionWidget::setVectorMergeEnabled(int x)
{
    qDebug() << "Setting - Vector Merge Enabled=" << x;

    SignalBlocker b( mVectorMergeBox );
    mVectorMergeBox->setEnabled( true );
    mVectorMergeBox->setChecked( x > 0 );
}

void ToolOptionWidget::setAA(int x)
{
    qDebug() << "Setting - Pen AA Enabled=" << x;

    SignalBlocker b( mUseAABox );
    mUseAABox->setEnabled( true );

    if (x == -1) {
        mUseAABox->setEnabled(false);
        mUseAABox->hide();
    } else {
        mUseAABox->show();
    }
    mUseAABox->setChecked( x > 0 );
}

void ToolOptionWidget::setInpolLevel(int x)
{
    qDebug() << "Setting - Interpolation level:" << x;

    SignalBlocker b( mNoInpol );
    SignalBlocker c( mSimpleInpol );
    SignalBlocker d( mStrongInpol );
    if (x == 0) {
        mNoInpol->setChecked(true);
    }
    else if (x == 1) {
        mSimpleInpol->setChecked(true);
    } else if (x == 2) {
        mStrongInpol->setChecked(true);
    } else if (x == 3) {
        mExtremeInpol->setChecked(true);
    } else if (x == -1) {
        mNoInpol->setChecked(true);
    }
}

void ToolOptionWidget::disableAllOptions()
{
    mSizeSlider->hide();
    mBrushSpinBox->hide();
    mFeatherSlider->hide();
    mFeatherSpinBox->hide();
    mUseFeatherBox->hide();
    mUseBezierBox->hide();
    mUsePressureBox->hide();
    mMakeInvisibleBox->hide();
    mPreserveAlphaBox->hide();
    mVectorMergeBox->hide();
    mUseAABox->hide();
    mInpolLevelsBox->hide();
}
