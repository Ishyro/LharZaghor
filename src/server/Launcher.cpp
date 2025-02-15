#ifdef _WIN32_WINNT
  #include <winsock2.h>
  #include <windows.h>
  #include <thread>
#endif

#include <thread>
#include <vector>
#include <unistd.h>
#include <string>

#include "data/actions/Action.h"
#include "data/Adventure.h"
#include "data/Character.h"
#include "data/World.h"

#include "data/Settings.h"

#include "util/FileOpener.h"

#include "communication/Socket.h"
#include "communication/ServerSocket.h"
#include "communication/Server.h"
#include "communication/SpeechManager.h"

#include "server/Launcher.h"
#include "server/Link.h"

#include <random>

#include "Values.h"

// to remove after testing
#include <iostream>
#include <chrono>
#include <ctime>

// Windows is great
typedef struct StartCommunicationParameter {
  Link * link;
  ServerSocket * ss;
  Adventure * adventure;
} StartCommunicationParameter;

// Windows is great
typedef struct RelinkCommunicationParameter {
  std::vector<Link *> * links;
  ServerSocket * ss;
  Adventure * adventure;
  int32_t playersNumber;
} RelinkCommunicationParameter;

void listener(void * param) {
  Link * link = ((StartCommunicationParameter *) param)->link;
  ServerSocket * ss = ((StartCommunicationParameter *) param)->ss;
  Adventure * adventure = ((StartCommunicationParameter *) param)->adventure;
  bool done = false;
  while(!done) {
    try {
      link->initialize(ss->accept());
      done = true;
    } catch (CloseException &e) {
      delete link;
      link = nullptr;
    }
  }
  done = false;
  while(!link->isClosed()) {
    link->listen();
  }
}

void relinkCommunication(void * param) {
  std::vector<Link *> * links = ((RelinkCommunicationParameter *) param)->links;
  ServerSocket * ss = ((RelinkCommunicationParameter *) param)->ss;
  Adventure * adventure = ((RelinkCommunicationParameter *) param)->adventure;
  int32_t playersNumber = ((RelinkCommunicationParameter *) param)->playersNumber;
  while(true) {
    Socket newSocket = ss->accept();
    bool used = false;
    std::string playerName;
    try {
      newSocket.write(std::string("RECONNECT"));
      playerName = newSocket.read();
    } catch (CloseException &e) {
      continue;
    }
    /* TODO
    for(int32_t i = 0; i < playersNumber; i++) {
      if((*links)[i]->isClosed() && (*links)[i]->getPlayer()->name == playerName) {
        (*links)[i]->changeSocket(newSocket);
        used = true;
        try {
          newSocket.write(std::string("OK"));
          break;
        } catch (CloseException &e) {
          used = false;
          (*links)[i]->markClosed();
        }
      }
    }
    */
    if(!used) {
      newSocket.close();
    }
  }
}

int32_t main(int32_t argc, char ** argv) {

  if (argc < 3) {
    std::cerr << "Expected: ./Launcher <adventureFile> <multi>" << std::endl;
    return EXIT_FAILURE;
  }
  
  setlocale(LC_ALL, "");
  std::string adventureFile = argv[1];
  srand(Settings::getSeed());

  Adventure * adventure = FileOpener::AdventureOpener(adventureFile, true);

  int32_t playersNumber = adventure->maxPlayers;
  bool multiplayer = true;

  if (argc >= 3) {
    std::istringstream(argv[2]) >> multiplayer;
    if(!multiplayer) {
      playersNumber = 1;
    }
  }
  
  std::vector<StartCommunicationParameter *> params = std::vector<StartCommunicationParameter *>(playersNumber);
  #ifdef _WIN32_WINNT
    std::vector<HANDLE> threads = std::vector<HANDLE>(playersNumber);
  #else
    std::vector<std::thread> threads = std::vector<std::thread>(playersNumber);
  #endif
  std::vector<Link *> links = std::vector<Link *>(playersNumber);
  ServerSocket * ss = new ServerSocket(Settings::getPort(), playersNumber, multiplayer);
  for(int32_t i = 0; i < playersNumber; i++) {
    links[i] = new Link(adventure);
    params[i] = new StartCommunicationParameter();
    params[i]->link = links[i];
    params[i]->ss = ss;
    params[i]->adventure = adventure;
    #ifdef _WIN32_WINNT
      threads[i] = (HANDLE) _beginthreadex(NULL, 0, (_beginthreadex_proc_type) listener, (void *) params[i], 0, NULL);
    #else
      threads[i] = std::thread(listener, (void *) params[i]);
    #endif
  }
  bool started = true;
  bool noPlayers = false;
  bool shuting_down = false;
  do {
    usleep(1);
    if(playersNumber <= 0) {
      shuting_down = true;
    }
    std::vector<Link *>::iterator it = links.begin();
    while(it != links.end()) {
      if((*it)->isShutingDown()) {
        shuting_down = true;
        break;
      }
      if((*it)->isMaster() && (*it)->isReady()) {
        started = true;
        break;
      }
      if((*it)->isClosed()) {
        playersNumber--;
        it = links.erase(it);
        continue;
      }
      started &= (*it)->isReady();
    }
  } while (!started && !shuting_down);
  for(int32_t i = 0; i < playersNumber; i++) {
    links[i]->sendStart();
  }
  RelinkCommunicationParameter * param = new RelinkCommunicationParameter();
  param->links = &links;
  param->ss = ss;
  param->adventure = adventure;
  param->playersNumber = playersNumber;
  #ifdef _WIN32_WINNT
    HANDLE thread = (HANDLE) _beginthreadex(NULL, 0, (_beginthreadex_proc_type) relinkCommunication, (void *) param, 0, NULL);
  #else
    std::thread thread = std::thread(relinkCommunication, (void *) param);
  #endif
  std::chrono::_V2::system_clock::time_point start = std::chrono::system_clock::now();
  std::chrono::_V2::system_clock::time_point end;
  while(!noPlayers && !shuting_down) {
    adventure->applyIteration();
    for(int32_t i = 0; i < playersNumber; i++) {
      links[i]->sendState();
    }
    SpeechManager::clear();
    adventure->getNPCsActions();
    // wait for tick duration
    end = std::chrono::system_clock::now();
    std::chrono::duration<double> duration = end - start;
    if(duration < Settings::getTickRate()) {
      std::this_thread::sleep_for(Settings::getTickRate() - duration);
    }
    start = std::chrono::system_clock::now();
    // receive playerActions
    std::list<Action *> actionsPlayers = std::list<Action *>();
    for(int32_t i = 0; i < playersNumber; i++) {
      if(links[i]->getNeedToUpdateActions()) {
        actionsPlayers.merge(links[i]->getAction());
      }
    }
    adventure->mergeActions(actionsPlayers);
    actionsPlayers.clear();
    adventure->executeActions();
    adventure->actAllProjectiles();
    noPlayers = true;
    for(int32_t i = 0; i < playersNumber; i++) {
      if(!links[i]->isClosed()) {
        noPlayers = false;
      }
      if(links[i]->isShutingDown()) {
        shuting_down = true;
        break;
      }
    }
    adventure->incrTick();
  }
  delete adventure;
  ss->close();
  delete ss;
  return EXIT_SUCCESS;
}
