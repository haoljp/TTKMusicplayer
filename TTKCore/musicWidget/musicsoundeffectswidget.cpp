#include "musicsoundeffectswidget.h"
#include "ui_musicsoundeffectswidget.h"
#include "musicuiobject.h"
///qmmp incldue
#include "effect.h"
#include "effectfactory.h"
///
#include <QBoxLayout>
#include <QStyledItemDelegate>

MusicSoundEffectsItemWidget::MusicSoundEffectsItemWidget(QWidget *parent)
    : QWidget(parent)
{
    m_type = Null;
    m_enable = false;

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    m_textLabel = new QLabel(this);
    m_textLabel->setStyleSheet(MusicUIObject::MBackgroundStyle10);

    setText(tr("null"));

    QWidget *func = new QWidget(this);
    func->setStyleSheet(MusicUIObject::MBackgroundStyle11);
    QHBoxLayout *funcLayout = new QHBoxLayout(func);
    funcLayout->setContentsMargins(0, 5, 5, 0);
    funcLayout->setSpacing(0);

    m_settingButton = new QPushButton(func);
    m_settingButton->setStyleSheet(MusicUIObject::MBackgroundStyle01 + MusicUIObject::MBackgroundStyle01);
    m_settingButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_settingButton->setFixedWidth(40);
    m_settingButton->setText(tr("Sets"));
    m_settingButton->setEnabled(false);
    connect(m_settingButton, SIGNAL(clicked()), SLOT(soundEffectValueChanged()));

    m_openButton = new QPushButton(func);
    m_openButton->setStyleSheet(MusicUIObject::MBackgroundStyle01);
    m_openButton->setIcon(QIcon(":/tiny/btn_effect_on"));
    m_openButton->setToolTip(tr("On"));
    m_openButton->setCursor(QCursor(Qt::PointingHandCursor));
    m_openButton->setFixedSize(16, 16);
    connect(m_openButton, SIGNAL(clicked()), SLOT(setPluginEnable()));

    QLabel *iconLabel = new QLabel(func);
    iconLabel->setStyleSheet(MusicUIObject::MBackgroundStyle01);
    iconLabel->setPixmap(QPixmap(":/tiny/lb_arrow_down_normal"));
    iconLabel->setFixedSize(16, 16);

    funcLayout->addWidget(m_settingButton);
    funcLayout->addWidget(iconLabel);
    funcLayout->addStretch(1);
    funcLayout->addWidget(m_openButton);

    layout->addWidget(m_textLabel);
    layout->addWidget(func);

    setLayout(layout);
}

MusicSoundEffectsItemWidget::~MusicSoundEffectsItemWidget()
{
    delete m_textLabel;
    delete m_openButton;
    delete m_settingButton;
}

QString MusicSoundEffectsItemWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicSoundEffectsItemWidget::setText(const QString &text)
{
    m_textLabel->setText(" " + text);
}

void MusicSoundEffectsItemWidget::setType(Type type)
{
    m_type = type;
}

void MusicSoundEffectsItemWidget::setPluginEnable(bool enable)
{
    m_enable = !enable;
    setPluginEnable();
}

void MusicSoundEffectsItemWidget::setPluginEnable()
{
    if(!m_enable)
    {
        m_enable = true;
        m_openButton->setIcon(QIcon(":/tiny/btn_effect_off"));
        soundEffectCheckBoxChanged(true);
        m_settingButton->setEnabled(true);
        m_openButton->setToolTip(tr("Off"));
    }
    else
    {
        m_enable = false;
        m_openButton->setIcon(QIcon(":/tiny/btn_effect_on"));
        soundEffectCheckBoxChanged(false);
        m_settingButton->setEnabled(false);
        m_openButton->setToolTip(tr("On"));
    }
}

void MusicSoundEffectsItemWidget::soundEffectCheckBoxChanged(bool state)
{
//    foreach(EffectFactory *factory, Effect::factories())
//    {
//        Effect::setEnabled(factory, false);
//    }
    QString plugin( transformQStringFromEnum() );
    foreach(EffectFactory *factory, Effect::factories())
    {
        if(factory->properties().name.contains(plugin))
        {
            Effect::setEnabled(factory, state);
            break;
        }
    }
}

void MusicSoundEffectsItemWidget::soundEffectValueChanged()
{
    QString plugin( transformQStringFromEnum() );
    foreach(EffectFactory *factory, Effect::factories())
    {
        if(factory->properties().name.contains(plugin))
        {
            factory->showSettings(this);
            break;
        }
    }
}

QString MusicSoundEffectsItemWidget::transformQStringFromEnum()
{
    QString plugin;
    switch(m_type)
    {
        case BS2B:      plugin = "BS2B"; break;
        case Crossfade: plugin = "Crossfade"; break;
        case Stereo:    plugin = "Stereo"; break;
        case LADSPA:    plugin = "LADSPA"; break;
        case SRC:       plugin = "SRC"; break;
        default:        plugin = "Unknow"; break;
    }
    return plugin;
}



MusicSoundEffectsWidget::MusicSoundEffectsWidget(QWidget *parent)
    : MusicAbstractMoveDialog(parent),
      ui(new Ui::MusicSoundEffectsWidget)
{
    ui->setupUi(this);

    ui->topTitleCloseButton->setIcon(QIcon(":/functions/btn_close_hover"));
    ui->topTitleCloseButton->setStyleSheet(MusicUIObject::MToolButtonStyle03);
    ui->topTitleCloseButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->topTitleCloseButton->setToolTip(tr("Close"));
    connect(ui->topTitleCloseButton, SIGNAL(clicked()), SLOT(close()));

    ui->backgroundMask->setStyleSheet(QString("#backgroundMask{%1}").arg(MusicUIObject::MBackgroundStyle11));
    ui->volumeLeftlabel->setStyleSheet(MusicUIObject::MColorStyle06);
    ui->volumeRightLabel->setStyleSheet(MusicUIObject::MColorStyle06);

    ui->volumeSlider->setStyleSheet(MusicUIObject::MSliderStyle06);
    ui->volumeSlider->setRange(-100, 100);
    ui->volumeSlider->setValue(0);
    ui->volumeSlider->setToolTip("0");
    connect(ui->volumeSlider, SIGNAL(valueChanged(int)), SLOT(volumeSliderChanged(int)));

    ui->stateComboBox->setItemDelegate(new QStyledItemDelegate(ui->stateComboBox));
    ui->stateComboBox->setStyleSheet(MusicUIObject::MComboBoxStyle01 + MusicUIObject::MItemView01);
    ui->stateComboBox->view()->setStyleSheet(MusicUIObject::MScrollBarStyle01);
    ui->stateComboBox->addItems( QStringList() << tr("OperatorAll") << tr("All On") << tr("All Off"));
    connect(ui->stateComboBox, SIGNAL(currentIndexChanged(int)), SLOT(stateComboBoxChanged(int)));

    ui->eqButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    ui->eqEffectButton->setStyleSheet(MusicUIObject::MPushButtonStyle04);
    ui->eqButton->setCursor(QCursor(Qt::PointingHandCursor));
    ui->eqEffectButton->setCursor(QCursor(Qt::PointingHandCursor));


    ////////////////////////////////////////////////////////////////////
    ui->BS2BWidget->setText("BS2B");
    ui->BS2BWidget->setType(MusicSoundEffectsItemWidget::BS2B);

    ui->CrossfadeWidget->setText("Crossfade");
    ui->CrossfadeWidget->setType(MusicSoundEffectsItemWidget::Crossfade);

    ui->StereoWidget->setText("Stereo");
    ui->StereoWidget->setType(MusicSoundEffectsItemWidget::Stereo);
#ifdef Q_OS_UNIX
    ui->LADSPAWidget->setText("LADSPA");
    ui->LADSPAWidget->setType(MusicSoundEffectsItemWidget::LADSPA);

    ui->SRCWidget->setText("SRC");
    ui->SRCWidget->setType(MusicSoundEffectsItemWidget::SRC);
#endif
}

MusicSoundEffectsWidget::~MusicSoundEffectsWidget()
{
    delete ui;
}

QString MusicSoundEffectsWidget::getClassName()
{
    return staticMetaObject.className();
}

void MusicSoundEffectsWidget::stateComboBoxChanged(int index)
{
    ui->stateComboBox->blockSignals(true);
    ui->stateComboBox->setCurrentIndex(0);
    ui->stateComboBox->blockSignals(false);

    if(index == 1)
    {
        ui->BS2BWidget->setPluginEnable(true);
        ui->CrossfadeWidget->setPluginEnable(true);
        ui->StereoWidget->setPluginEnable(true);
#ifdef Q_OS_UNIX
        ui->LADSPAWidget->setPluginEnable(true);
        ui->SRCWidget->setPluginEnable(true);
#endif
    }
    else if(index == 2)
    {
        ui->BS2BWidget->setPluginEnable(false);
        ui->CrossfadeWidget->setPluginEnable(false);
        ui->StereoWidget->setPluginEnable(false);
#ifdef Q_OS_UNIX
        ui->LADSPAWidget->setPluginEnable(false);
        ui->SRCWidget->setPluginEnable(false);
#endif
    }
}

void MusicSoundEffectsWidget::volumeSliderChanged(int value)
{
    ui->volumeSlider->setToolTip(QString::number(value));
}

int MusicSoundEffectsWidget::exec()
{
    setBackgroundPixmap(ui->background, size());
    return MusicAbstractMoveDialog::exec();
}
