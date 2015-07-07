#pragma once

#include <QObject>

class ScreenRecorder : public QObject
{
   Q_OBJECT
public:
   explicit ScreenRecorder(QObject *parent = 0);
   ~ScreenRecorder();

   /// Start recording the screen
   void startRecording(const QString &_filename);
   /// Stops recording the screen
   void stopRecording();
   /// Returns true iff screen recording is active
   bool isRecording() const;

signals:

public slots:
};
