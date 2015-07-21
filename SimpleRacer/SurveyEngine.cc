#include "SurveyEngine.hh"
#include <QWebView>
#include "Common.hh"
#include "RenderingWidget.hh"
#include "StatisticsEngine.hh"
#include <QNetworkRequest>
#include "SimpleRacer.hh"
#include <QWebFrame>

SurveyEngine::SurveyEngine(QWebView *_webView, RenderingWidget *_renderWidget)
  : mWebView(_webView), mRendering(_renderWidget)
{
   SR_ASSERT(nullptr != _webView && "Nullptr exception.");
   connect(_webView->page(), &QWebPage::downloadRequested, this, &SurveyEngine::onDownloadRequested);
   mCurrSurvey = CurrentSurvey{SurveyType::INVALID, SurveyLanguage::INVALID};
#ifdef QT_DEBUG
   QWebSettings::globalSettings()->setAttribute(QWebSettings::DeveloperExtrasEnabled, true);
#endif
}

SurveyEngine::~SurveyEngine()
{
   mWebView = nullptr;
   mRendering = nullptr;
}

void SurveyEngine::makeSurvey(SurveyType _type)
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
   switch (mLanguage)
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
   mCurrSurvey.lang = mLanguage;
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

bool SurveyEngine::showInstructions(Instruction _instruction)
{
   switch (_instruction)
   {
   case Instruction::NONE:
      mWebView->load(QUrl("qrc:/htdocs/instruction0.html"));
      break;
   case Instruction::FEW_COLLISION:
      mWebView->load(QUrl("qrc:/htdocs/instruction1.html"));
      break;
   case Instruction::MUCH_COLLISION:
      mWebView->load(QUrl("qrc:/htdocs/instruction2.html"));
      break;
   default:
      return false;
      break;
   }
   return true;
}

bool SurveyEngine::showInstructionsAndTrainingsPhase(Instruction _instruction)
{
   bool ok = showInstructions(_instruction);
   if (!ok)
      return false;
   connect(
       mWebView, &QWebView::loadFinished, [this](bool ok)
       {
          if (!ok)
             return;
          const _ phase = int(Settings::the()->getUserTrainingState());
          mWebView->page()->mainFrame()->evaluateJavaScript(
              "document.getElementById('addHeading').innerHTML=\"Trainings-Phase: " + QString::number(phase) + "/8\"");
       });

   return true;
}

void SurveyEngine::onDownloadRequested(const QNetworkRequest &_request)
{
   // magic!
   // Abuse QByteArray since it can convert base64
   _ data = QString(QByteArray::fromBase64(_request.url().toEncoded().split(',').at(1)));
   // store results in statistics engine
   _ type = mCurrSurvey.type;
   switch (type)
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
   emit surveyFinished(type);
}
