#include "ScreenRecorder.hh"
#include <QProcess>
#include <QApplication>
#include <QDesktopWidget> // get screen size

ScreenRecorder::ScreenRecorder(QObject *parent) : QObject(parent)
{
   // TODO: IMPLEMENT
}

ScreenRecorder::~ScreenRecorder()
{
   // TODO: IMPLEMENT
}

void ScreenRecorder::startRecording(const QString &_filename, bool _waitForStarted)
{
   stopRecording();
   mWorker.reset(new ScreenRecordWorker(_filename));
   connect(mWorker.get(), &ScreenRecordWorker::finished, this, &ScreenRecorder::workerFinished);
   connect(mWorker.get(), &ScreenRecordWorker::recordingStarted, this, &ScreenRecorder::workerStarted);
   mWorker->start();
   while (_waitForStarted && !isRecording())
   {
      qApp->processEvents();
      QThread::msleep(10); // NOP
   }
}

void ScreenRecorder::stopRecording(bool _waitForStopped)
{
   if (!isRecording())
      return;
   SR_ASSERT(mWorker != nullptr && "Running but worker is null.");
   mWorker->requestInterruption();
   while (_waitForStopped && isRecording())
   {
      qApp->processEvents();
      QThread::msleep(10); // NOP
   }
}

bool ScreenRecorder::isRecording() const
{
   return mWorkerRunning;
}

void ScreenRecorder::workerFinished()
{
   SR_ASSERT(true == mWorkerRunning && "Worker not running");
   mWorkerRunning = false;
   emit stoppedRecording();
}

void ScreenRecorder::workerStarted()
{
   SR_ASSERT(false == mWorkerRunning && "Worker already running");
   mWorkerRunning = true;
}


ScreenRecordWorker::ScreenRecordWorker(const QString &_filename) : mFilename(_filename)
{
}

void ScreenRecordWorker::run()
{
   const _ rec = QApplication::desktop()->screenGeometry();
   const QString screenSize = QString::number(rec.width()) + "x" + QString::number(rec.height());
   const QString program = "ffmpeg";
   const QStringList args
       = {"-video_size", screenSize, "-framerate", "60", "-f", "x11grab", "-i", ":0.0", "-c:v", "libx264", "-preset", "superfast", "-y", mFilename};
   QProcess proc;
   proc.start(program, args);
   if (!proc.waitForStarted())
   {
      SR_ASSERT(0 && "Failed to start ffmpeg");
      return;
   }
   emit recordingStarted();
   forever
   {
      if (isInterruptionRequested())
      {
         proc.write("q"); // "q" quits ffmpeg
         proc.closeWriteChannel();
         if (!proc.waitForFinished())
         {
            SR_ASSERT(0 && "Failed to quit ffmpeg");
            return;
         }
         return;
      }
      msleep(100); // NOP
   }
}
