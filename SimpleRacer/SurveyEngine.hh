#pragma once
#include <QObject>
#include "Common.hh"
#include "Settings.hh"

class QWebView;
class RenderingWidget;
class QNetworkRequest;

enum class SurveyLanguage : char
{
   INVALID = 0,
   ENGLISH,
   GERMAN
};

class SurveyEngine : public QObject
{
   Q_OBJECT

public:
   SurveyEngine(QWebView *_webView, RenderingWidget *_renderWidget);
   ~SurveyEngine();

   /// Starts a survey
   void makeSurvey(SurveyType _type);
   /// Toogle web and game window
   void toogleWebWindow(bool _showWebviewHideGame);
   /// Show waiting screen
   void showWaitingScreen();
   /// Show instructions
   /// @return False if the instruction is invalid
   bool showInstructions(Instruction _instruction, bool _hideTrainingsphaseIndicatior = true);
   /// Show instructions and the current training phase
   /// @return False if the instruction is invalid
   bool showInstructionsAndTrainingsPhase(Instruction _instruction);

private slots:
   void onDownloadRequested(const QNetworkRequest &_request);

signals:
   void surveyFinished(SurveyType);

private:
   struct CurrentSurvey
   {
      SurveyType type;
      SurveyLanguage lang;
   };
   CurrentSurvey mCurrSurvey;                         ///< Which survey is currently in progess?
   QWebView *mWebView = nullptr;                      ///< Owned by Qt
   RenderingWidget *mRendering = nullptr;             ///< Owned by SimpleRacer-class
   SurveyLanguage mLanguage = SurveyLanguage::GERMAN; ///< Language of the survey
   int mLastLoadedCondition = -1;                     ///< Last loaded condition

public:
   SETTER(Language);
   PROPERTY(LastLoadedCondition);
};
