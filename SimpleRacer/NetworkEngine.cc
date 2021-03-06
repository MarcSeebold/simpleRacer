#include "NetworkEngine.hh"
#include "Settings.hh"
#include <QTcpServer>
#include <QTcpSocket>
#include <QNetworkSession>
#include <QDebug>
#include "SimpleRacer.hh"
#include "SurveyEngine.hh"

NetworkEngine::NetworkEngine()
{
}

NetworkEngine::~NetworkEngine()
{
   mTcpServer = nullptr;
   mNetworkSession = nullptr;
   mClient = nullptr;
}

bool NetworkEngine::listen()
{
   mTcpServer = UniqueQTcpServer(new QTcpServer(this));
   _ port = Settings::the()->getNetworkPort();
   // IPv4 only, port specified in settings
   if (!mTcpServer->listen(QHostAddress::AnyIPv4, port))
      return false;
   connect(mTcpServer.get(), &QTcpServer::newConnection, this, &NetworkEngine::newConnection);
   return true;
}

bool NetworkEngine::send(const QByteArray &_data)
{
   return (mClient->write(_data) != -1);
}

void NetworkEngine::newConnection()
{
   mClient = mTcpServer->nextPendingConnection();
   connect(mClient, &QTcpSocket::disconnected, this, &NetworkEngine::onDisconnect);
   connect(mClient, &QTcpSocket::readyRead, this, &NetworkEngine::onData);
}

void NetworkEngine::onData()
{
   while (mClient->bytesAvailable())
   {
      _ data = mClient->readAll();
      bool ok = true;
      _ cmd = data.toShort(&ok);
      if (!ok)
      {
         qDebug() << "Unknown command: " << data;
         send("NACK");
         return;
      }
      switch ((NetworkCommand)cmd)
      {
      case NetworkCommand::START_GAME:
         SimpleRacer::the()->showInstructionsAndStartGame();
         send("ACK");
         break;
      case NetworkCommand::START_TRAINING:
         SimpleRacer::the()->startTraining();
         send("ACK");
         break;
      case NetworkCommand::OPEN_SURVEY_PREGAME:
         if (SimpleRacer::the()->isRunning())
            SimpleRacer::the()->stopGame();
         SimpleRacer::survey()->makeSurvey(SurveyType::CORE);
         send("ACK");
         break;
      case NetworkCommand::OPEN_SURVEY_INGAME:
         if (SimpleRacer::the()->isRunning())
            SimpleRacer::the()->stopGame();
         SimpleRacer::survey()->makeSurvey(SurveyType::INGAME);
         send("ACK");
         break;
      case NetworkCommand::OPEN_SURVEY_POSTGAME:
         if (SimpleRacer::the()->isRunning())
            SimpleRacer::the()->stopGame();
         SimpleRacer::survey()->makeSurvey(SurveyType::POSTGAME);
         send("ACK");
         break;
      case NetworkCommand::LOAD_CONDITION_1:
         Settings::the()->loadCondition(1);
         send("ACK");
         break;
      case NetworkCommand::LOAD_CONDITION_2:
         Settings::the()->loadCondition(2);
         send("ACK");
         break;
      case NetworkCommand::LOAD_CONDITION_3:
         Settings::the()->loadCondition(3);
         send("ACK");
         break;
      case NetworkCommand::LOAD_CONDITION_4:
         Settings::the()->loadCondition(4);
         send("ACK");
         break;
      case NetworkCommand::LOAD_CONDITION_5:
         Settings::the()->loadCondition(5);
         send("ACK");
         break;
      case NetworkCommand::LOAD_CONDITION_6:
         Settings::the()->loadCondition(6);
         send("ACK");
         break;
      case NetworkCommand::LOAD_CONDITION_7:
         Settings::the()->loadCondition(7);
         send("ACK");
         break;
      case NetworkCommand::LOAD_CONDITION_8:
         Settings::the()->loadCondition(8);
         send("ACK");
         break;
      case NetworkCommand::LOAD_CONDITION_9:
         Settings::the()->loadCondition(9);
         send("ACK");
         break;
      case NetworkCommand::LOAD_CONDITION_10:
         Settings::the()->loadCondition(10);
         send("ACK");
         break;
      case NetworkCommand::LOAD_CONDITION_11:
         Settings::the()->loadCondition(11);
         send("ACK");
         break;
      case NetworkCommand::LOAD_CONDITION_12:
         Settings::the()->loadCondition(12);
         send("ACK");
         break;
      case NetworkCommand::LOAD_CONDITION_13:
         Settings::the()->loadCondition(13);
         send("ACK");
         break;
      case NetworkCommand::LOAD_CONDITION_14:
         Settings::the()->loadCondition(14);
         send("ACK");
         break;
      case NetworkCommand::START_TESTPLAY:
         Settings::the()->setTestPlay(true);
         send("ACK");
         break;
      case NetworkCommand::STOP_TESTPLAY:
         Settings::the()->setTestPlay(false);
         send("ACK");
         break;
      case NetworkCommand::GET_STATE:
      {
         _ state = SimpleRacer::the()->getGameState();
         _ res = NetworkCommand::INVALID;
         switch (state)
         {
         case GameState::PLAYING:
            res = NetworkCommand::STATE_PLAYING;
            break;
         case GameState::SURVEY:
            res = NetworkCommand::STATE_SURVEY;
            break;
         case GameState::WAITING:
            res = NetworkCommand::STATE_WAITING;
            break;
         default:
            SR_ASSERT(0 && "Unhandled case.");
            break;
         }
         send(QByteArray::number((int)res));
      }
      break;
      case NetworkCommand::SET_SURVEY_LANG_ENG:
         SimpleRacer::survey()->setLanguage(SurveyLanguage::ENGLISH);
         break;
      case NetworkCommand::SET_SURVEY_LANG_GER:
         SimpleRacer::survey()->setLanguage(SurveyLanguage::GERMAN);
         break;
      default:
         send("NACK");
         break;
      }
   }
}

void NetworkEngine::onDisconnect()
{
   // asynch delete by Qt
   mClient->deleteLater();
   mClient = nullptr;
}
