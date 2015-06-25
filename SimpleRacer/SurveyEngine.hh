#pragma once
#include <QObject>
#include "Common.hh"

class QWebView;
class RenderingWidget;
class QNetworkRequest;

enum class SurveyType : char
{
   INVALID = 0,
   CORE,
   INGAME,
   POSTGAME
};

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

private slots:
   void onDownloadRequested(const QNetworkRequest &_request);

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

public:
   SETTER(Language);
};
