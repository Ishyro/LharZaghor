#include "data/Adventure.h"
#include "data/Character.h"
#include "data/Map.h"
#include "data/World.h"

#include "data/actions/Action.h"
#include "data/actions/BaseAction.h"

#include "communication/Socket.h"
#include "communication/ServerSocket.h"
#include "communication/Server.h"

#include "server/Link.h"

void Link::playerChoices() {
  try {
    Server::sendWaysIncompabilities(s, adventure);
    Server::sendStartingPossibilites(s, adventure);
    Server::sendTranslationPaths(s, adventure);
    player = Server::receiveChoices(s, adventure);
  } catch (const CloseException &e) {
    markClosed();
    throw e;
  }
}

void Link::sendMap() {
  if(!isClosed()) {
    Map * map = new Map(adventure->getWorld()->getMap(player->getCurrentMapId()), player, adventure->getDatabase());
    try {
      Server::sendMap(s, map, player, adventure);
    } catch (const CloseException &e) {
      markClosed();
    }
    delete map;
  }
}

Action * Link::receiveAction() {
  if(!isClosed()) {
    try {
      return Server::receiveAction(s, player, adventure);
    } catch (const CloseException &e) {
      markClosed();
    }
  }
  return new BaseAction(IDLE, adventure, nullptr, player);
}

bool Link::isClosed() { return closed; }
void Link::markClosed() { closed = true; }
Character * Link::getPlayer() { return player; }
void Link::changeSocket(Socket s) { this->s = s; closed = false; }
