#include "SurveyEngine.hh"
#include <QWebView>
#include "Common.hh"
#include "RenderingWidget.hh"
#include "StatisticsEngine.hh"
#include <QNetworkRequest>
#include "SimpleRacer.hh"

SurveyEngine::SurveyEngine(QWebView *_webView, RenderingWidget *_renderWidget)
  : mWebView(_webView), mRendering(_renderWidget)
{
   SR_ASSERT(nullptr != _webView && "Nullptr exception.");
   connect(_webView->page(), &QWebPage::downloadRequested, this, &SurveyEngine::onDownloadRequested);
   mCurrSurvey = CurrentSurvey{SurveyType::INVALID, SurveyLanguage::INVALID};
}

SurveyEngine::~SurveyEngine()
{
   mWebView = nullptr;
   mRendering = nullptr;
}

void SurveyEngine::makeSurvey(SurveyType _type, SurveyLanguage _language)
{
   // show webView, hide game
   toogleWebWindow(true);
   // assemble url
   QString url = "qrc:/htdocs/geq_";
   // Survey-Type
   switch (_type)
   {
   case SurveyType::CORE:
      url += "core";
      break;
   case SurveyType::INGAME:
      url += "ingame";
      break;
   case SurveyType::POSTGAME:
      url += "post-game";
      break;
   default:
      SR_ASSERT(0 && "unhandled case");
      return;
   }
   url += "_";
   // Survey-Language
   switch (_language)
   {
   case SurveyLanguage::ENGLISH:
      url += "english";
      break;
   case SurveyLanguage::GERMAN:
      url += "german";
      break;
   default:
      SR_ASSERT(0 && "unhandled case");
      return;
   }
   url += ".html";
   // Load url in webview
   mWebView->load(QUrl(url));
   // Store configuration
   mCurrSurvey.lang = _language;
   mCurrSurvey.type = _type;
   // Set state
   SimpleRacer::the()->setGameState(GameState::SURVEY);
}

void SurveyEngine::toogleWebWindow(bool _showWebviewHideGame)
{
   if (_showWebviewHideGame)
   {
      mWebView->show();
      mRendering->hide();
   }
   else
   {
      mWebView->hide();
      mRendering->show();
   }
}

void SurveyEngine::showWaitingScreen()
{
   mWebView->load(QUrl("qrc:/htdocs/wait.html"));
}

void SurveyEngine::onDownloadRequested(const QNetworkRequest &_request)
{
   // magic!
   // Abuse QByteArray since it can convert base64
   _ data = QString(QByteArray::fromBase64(_request.url().toEncoded().split(',').at(1)));
   // store results in statistics engine
   switch (mCurrSurvey.type)
   {
   case SurveyType::CORE:
      StatisticsEngine::the()->tellCoreSurvey(std::move(data));
      break;
   case SurveyType::INGAME:
      StatisticsEngine::the()->tellInGameSurvey(std::move(data));
      break;
   case SurveyType::POSTGAME:
      StatisticsEngine::the()->tellPostGameSurvey(std::move(data));
      break;
   default:
      SR_ASSERT(0 && "unhandled case");
      return;
   }
   // show "waiting"-screen to user
   showWaitingScreen();
   // reset state
   mCurrSurvey = CurrentSurvey{SurveyType::INVALID, SurveyLanguage::INVALID};
   // state
   SimpleRacer::the()->setGameState(GameState::WAITING);
}
