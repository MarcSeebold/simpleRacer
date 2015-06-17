#pragma once
#include <QObject>

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
   void makeSurvey(SurveyType _type, SurveyLanguage _language);
   /// Toogle survey and game window
   void toogleSurveyWindow(bool _showSurveyHideGame);
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
   CurrentSurvey mCurrSurvey;             ///< Which survey is currently in progess?
   QWebView *mWebView = nullptr;          ///< Owned by Qt
   RenderingWidget *mRendering = nullptr; ///< Owned by SimpleRacer-class
};
