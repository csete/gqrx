/* -*- c++ -*- */
/*
 * Copyright 2011-2012 Alexandru Csete OZ9AEC.
 *
 * Gqrx is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * Gqrx is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Gqrx; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPointer>
#include <QSettings>
#include <QString>
#include <QTimer>

#include "qtgui/dockrxopt.h"
#include "qtgui/dockaudio.h"
#include "qtgui/dockinputctl.h"
#include "qtgui/dockiqplayer.h"
#include "qtgui/dockfft.h"
#include "qtgui/afsk1200win.h"
#include "qtgui/bpsk1000win.h"

#include "applications/gqrx/receiver.h"


namespace Ui {
    class MainWindow;  /*! The main window UI */
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(const QString cfgfile="default.conf", QWidget *parent = 0);
    ~MainWindow();

    bool loadConfig(const QString cfgfile);
    bool saveConfig(const QString cfgfile);
    void storeSession();

    bool configOk; /*!< Main app uses this flag to know whether we should abort or continue. */

signals:
    void configChanged(QSettings *settings); /*!< New configuration has been loaded. */

public slots:
    void setNewFrequency(qint64 freq);

private:
    Ui::MainWindow *ui;

    QPointer<QSettings> m_settings;  /*!< Application wide settings. */
    QString             m_cfg_dir;   /*!< Default config dir, e.g. XDG_CONFIG_HOME. */
    QString             m_last_dir;

    qint64 d_lnb_lo;  /* LNB LO in Hz. */

    enum receiver::filter_shape d_filter_shape;
    std::complex<float>* d_fftData;
    double *d_realFftData;
    //double *d_audioFttData;

    /* dock widgets */
    DockRxOpt      *uiDockRxOpt;
    DockAudio      *uiDockAudio;
    DockInputCtl   *uiDockInputCtl;
    //DockIqPlayer   *uiDockIqPlay;
    DockFft        *uiDockFft;

    /* data decoders */
    Bpsk1000Win    *dec_bpsk1000;
    Afsk1200Win    *dec_afsk1200;

    QTimer   *dec_timer;
    QTimer   *meter_timer;
    QTimer   *iq_fft_timer;
    QTimer   *audio_fft_timer;

    receiver *rx;

private slots:
    /* rf */
    void setLnbLo(double freq_mhz);

    /* baseband receiver */
    void setFilterOffset(qint64 freq_hz);
    void setRfGain(double gain);
    void setFreqCorr(int ppm);
    void setDcCorr(double dci, double dcq);
    void setIqCorr(double gain, double phase);
    void selectDemod(int index);
    void setFmMaxdev(float max_dev);
    void setFmEmph(double tau);
    void setAmDcrStatus(bool enabled);
    void setAgcOn(bool agc_on);
    void setAgcHang(bool use_hang);
    void setAgcThreshold(int threshold);
    void setAgcSlope(int factor);
    void setAgcDecay(int msec);
    void setAgcGain(int gain);
    void setNoiseBlanker(int nbid, bool on, float threshold);
    void setSqlLevel(double level_db);
    void setAudioGain(float gain);

    /* audio recording and playback */
    void startAudioRec(const QString filename);
    void stopAudioRec();
    void startAudioPlayback(const QString filename);
    void stopAudioPlayback();

    void toggleIqPlayback(bool play, const QString filename);

    /* FFT settings */
    void setIqFftSize(int size);
    void setIqFftRate(int fps);
    void setIqFftSplit(int pct_wf);
    void setIqFftZoom(int zoom);
    void setAudioFftRate(int fps);

    void on_plotter_NewDemodFreq(qint64 freq, qint64 delta);   /*! New demod freq (aka. filter offset). */
    void on_plotter_NewFilterFreq(int low, int high);    /*! New filter width */
    void on_plotter_NewCenterFreq(qint64 f);

    /* menu and toolbar actions */
    void on_actionDSP_triggered(bool checked);
    int  on_actionIoConfig_triggered();
    void on_actionLoadSettings_triggered();
    void on_actionSaveSettings_triggered();
    void on_actionIqRec_triggered(bool checked);
    void on_actionFullScreen_triggered(bool checked);
    void on_actionAFSK1200_triggered();
    void on_actionBPSK1000_triggered();
    void on_actionUserGroup_triggered();
    void on_actionAbout_triggered();
    void on_actionAboutQt_triggered();

    /* window close signals */
    void afsk1200win_closed();
    void bpsk1000win_closed();

    /* cyclic processing */
    void decoderTimeout();
    void meterTimeout();
    void iqFftTimeout();
    void audioFftTimeout();

};

#endif // MAINWINDOW_H
