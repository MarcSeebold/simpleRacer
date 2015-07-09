#pragma once

#include "Common.hh"
#include <QObject>
#include <QThread>

SHARED(class, ScreenRecordWorker);

/**
 * @brief The ScreenRecorder class performs screen capturing
 * @todo Currently only the first monitor is caputured
 */
class ScreenRecorder : public QObject
{
   Q_OBJECT
public:
   explicit ScreenRecorder(QObject *parent = 0);
   ~ScreenRecorder();

   /// Start recording the screen
   /// @param _waitForStarted If true, this function will block until recording is started.
   ///        Otherwise, recording may start some time after this function was called.
   ///        If this is the case please use the signal startedRecording.
   void startRecording(const QString &_filename, bool _waitForStarted = true);

   /// Stops recording the screen
   /// @param _waitForStopped If true, this function will block until the recording is stopped
   ///        Otherwise, recording may continue for some time after this function is called.
   ///        Please use the stoppedRecording signal then.
   void stopRecording(bool _waitForStopped = true);

   /// Returns true iff screen recording is active
   bool isRecording() const;

signals:
   /// Emitted when the recording has stopped
   void stoppedRecording();
   /// Emitted when the recording has started
   void startedRecording();

private slots:
   void workerFinished();
   void workerStarted();

private:
   bool mWorkerRunning = false;
   UniqueScreenRecordWorker mWorker;
};

class ScreenRecordWorker : public QThread
{
   Q_OBJECT

public:
   ScreenRecordWorker(const QString &_filename);

public slots:
   /// Main loop
   void run() override;

signals:
   void recordingStarted();

private:
   QString mFilename;
};
