#include <string>

#include "communication/Client.h"
#include "communication/Socket.h"

#include "data/Attributes.h"
#include "data/Character.h"
#include "data/Map.h"
#include "data/Projectile.h"
#include "data/Tile.h"
#include "data/Way.h"

#include "clients/terminal/Display.h"

namespace Display {

  void displayMap(MapDisplay * display, WINDOW * screen) {
    int lines = 0;
    int cols = 0;
    getmaxyx(screen, lines, cols);
    mvwprintw(screen, 1, cols / 2 - display->name.length() / 2, display->name.c_str());
    for(int y = display->sizeY - 1; y >= 0 ; y--) {
      for(int x = 0; x < display->sizeX; x++) {
        std::string to_print = ".\0"; // middle dot - ·
        if(display->tiles[x][y]->untraversable) {
          to_print = "#\0";
        }
        if(display->tiles[x][y]->name == "mist") { // unseen
          to_print = " \0";
        }
        mvwprintw(screen, lines / 2 - display->sizeY / 2 + display->sizeY -1 - y, x + cols / 2 - display->sizeX / 2, to_print.c_str());
      }
    }
    for(CharacterDisplay * character : display->characters) {
      std::string to_print;
      char truc = character->name.at(0);
      to_print = truc;
      wattron(screen, COLOR_PAIR(BLUE));
      mvwprintw(screen, lines / 2 - display->sizeY / 2 + display->sizeY -1 - character->y, character->x + cols / 2 - display->sizeX / 2, to_print.c_str());
      wattroff(screen, COLOR_PAIR(BLUE));
    }
    for(ProjectileDisplay * projectile : display->projectiles) {
      std::string to_print = "~\0";
      wattron(screen, COLOR_PAIR(RED));
      mvwprintw(screen, lines / 2 - display->sizeY / 2 + display->sizeY -1 - projectile->y, projectile->x + cols / 2 - display->sizeX / 2, to_print.c_str());
      wattroff(screen, COLOR_PAIR(RED));
    }
    wrefresh(screen);
  }

  void displayTileMap(MapDisplay * display, WINDOW * screen) {
    int lines = 0;
    int cols = 0;
    getmaxyx(screen, lines, cols);
    mvwprintw(screen, 1, cols / 2 - display->name.length() / 2, display->name.c_str());
    for(int y = display->sizeY - 1; y >= 0 ; y--) {
      for(int x = 0; x < display->sizeX; x++) {
        char * to_print = new char [2];
        to_print[1] = '\0';
        to_print[0] = display->tiles[x][y]->name.at(0);
        mvwprintw(screen, lines / 2 - display->sizeY / 2 + display->sizeY -1 - y, x + cols / 2 - display->sizeX / 2, to_print);
        delete to_print;
      }
    }
    wrefresh(screen);
  }

  void displayLightMap(MapDisplay * display, WINDOW * screen) {
    int lines = 0;
    int cols = 0;
    getmaxyx(screen, lines, cols);
    mvwprintw(screen, 1, cols / 2 - display->name.length() / 2, display->name.c_str());
    for(int y = display->sizeY - 1; y >= 0 ; y--) {
      for(int x = 0; x < display->sizeX; x++) {
        char * to_print = new char [2];
        to_print[1] = '\0';
        to_print[0] = std::to_string(display->tiles[x][y]->light).at(0);
        mvwprintw(screen, lines / 2 - display->sizeY / 2 + display->sizeY -1 - y, x + cols / 2 - display->sizeX / 2, to_print);
        delete to_print;
      }
    }
    wrefresh(screen);
  }

  WINDOW * displayAttributes(Attributes * attributes, int place, int color, WINDOW * screen, int offsetY, int offsetX) {
    int lines = 0;
    int cols = 0;
    getmaxyx(screen, lines, cols);
    int numberX = cols / ATTRIBUTES_LENGTH;
    int numberY = lines / ATTRIBUTES_HEIGHT;
    int placeX = place;
    int placeY = 0;
    while((ATTRIBUTES_LENGTH + 1) * (placeX + 1) > cols) {
      placeY++;
      placeX -= numberX;
    }
    WINDOW * attributesScreen = subwin(screen, ATTRIBUTES_HEIGHT, ATTRIBUTES_LENGTH, offsetY + 2 + ATTRIBUTES_HEIGHT * placeY, offsetX + 1 + ATTRIBUTES_LENGTH * placeX);
    wattron(attributesScreen, COLOR_PAIR(color));
    box(attributesScreen, ACS_VLINE, ACS_HLINE);
    mvwprintw(attributesScreen, 1, 1, (std::string("Name: ") + attributes->name).c_str());
    mvwprintw(attributesScreen, 2, 1, (std::string("Hp: ") + std::to_string(attributes->baseHp)).c_str());
    mvwprintw(attributesScreen, 3, 1, (std::string("Mana: ") + std::to_string(attributes->baseMana)).c_str());
    mvwprintw(attributesScreen, 4, 1, (std::string("Armor: ") + std::to_string(attributes->baseArmor)).c_str());
    mvwprintw(attributesScreen, 5, 1, (std::string("Soulburn: ") + std::to_string(attributes->baseSoulBurn)).c_str());
    mvwprintw(attributesScreen, 6, 1, (std::string("Flow: ") + std::to_string(attributes->baseFlow)).c_str());
    mvwprintw(attributesScreen, 7, 1, (std::string("Vision Range: ") + std::to_string(attributes->baseVisionRange)).c_str());
    mvwprintw(attributesScreen, 8, 1, (std::string("Vision Power: ") + std::to_string(attributes->baseVisionPower)).c_str());
    mvwprintw(attributesScreen, 9, 1, (std::string("Detection Range: ") + std::to_string(attributes->baseDetectionRange)).c_str());
    wattroff(attributesScreen, COLOR_PAIR(color));
    touchwin(screen);
    return attributesScreen;
  }

  WINDOW * displayWay(Way * way, int place, int color, WINDOW * screen, int offsetY, int offsetX) {
    int lines = 0;
    int cols = 0;
    getmaxyx(screen, lines, cols);
    int numberX = cols / WAY_LENGTH;
    int numberY = lines / WAY_HEIGHT;
    int placeX = place;
    int placeY = 0;
    while((WAY_LENGTH + 1) * (placeX + 1) > cols) {
      placeY++;
      placeX -= numberX;
    }
    WINDOW * wayScreen = subwin(screen, WAY_HEIGHT, WAY_LENGTH, offsetY + 2 + WAY_HEIGHT * placeY, offsetX + 1 + WAY_LENGTH * placeX);
    wattron(wayScreen, COLOR_PAIR(color));
    box(wayScreen, ACS_VLINE, ACS_HLINE);
    mvwprintw(wayScreen, 1, 1, (std::string("Name: ") + way->name).c_str());
    mvwprintw(wayScreen, 2, 1, (std::string("+Hp: ") + std::to_string(way->hpIncr)).c_str());
    mvwprintw(wayScreen, 3, 1, (std::string("+Mana: ") + std::to_string(way->manaIncr)).c_str());
    mvwprintw(wayScreen, 4, 1, (std::string("+Armor: ") + std::to_string(way->armorIncr)).c_str());
    mvwprintw(wayScreen, 5, 1, (std::string("+Soulburn: ") + std::to_string(way->soulBurnIncr)).c_str());
    mvwprintw(wayScreen, 6, 1, (std::string("+Flow: ") + std::to_string(way->flowIncr)).c_str());
    wattroff(wayScreen, COLOR_PAIR(color));
    return wayScreen;
  }

  WINDOW * displayCharacter(Attributes * attributes, Way * race, Way * origin, Way * culture, Way * religion, Way * profession, int color, WINDOW * screen, int sizeX, int offsetY, int offsetX) {
    int lines = 0;
    int cols = 0;
    getmaxyx(screen, lines, cols);
    WINDOW * characterScreen = subwin(screen, ATTRIBUTES_HEIGHT - 1, sizeX, offsetY, offsetX);
    int hpIncr = 0;
    int manaIncr = 0;
    int armorIncr = 0;
    int soulBurnIncr = 0;
    int flowIncr = 0;
    if(race != nullptr) {
      hpIncr += race->hpIncr;
      manaIncr += race->manaIncr;
      armorIncr += race->armorIncr;
      soulBurnIncr += race->soulBurnIncr;
      flowIncr += race->flowIncr;
    }
    if(origin != nullptr) {
      hpIncr += origin->hpIncr;
      manaIncr += origin->manaIncr;
      armorIncr += origin->armorIncr;
      soulBurnIncr += origin->soulBurnIncr;
      flowIncr += origin->flowIncr;
    }
    if(culture != nullptr) {
      hpIncr += culture->hpIncr;
      manaIncr += culture->manaIncr;
      armorIncr += culture->armorIncr;
      soulBurnIncr += culture->soulBurnIncr;
      flowIncr += culture->flowIncr;
    }
    if(religion != nullptr) {
      hpIncr += religion->hpIncr;
      manaIncr += religion->manaIncr;
      armorIncr += religion->armorIncr;
      soulBurnIncr += religion->soulBurnIncr;
      flowIncr += religion->flowIncr;
    }
    if(profession != nullptr) {
      hpIncr += profession->hpIncr;
      manaIncr += profession->manaIncr;
      armorIncr += profession->armorIncr;
      soulBurnIncr += profession->soulBurnIncr;
      flowIncr += profession->flowIncr;
    }
    wattron(characterScreen, COLOR_PAIR(color));
    box(characterScreen, ACS_VLINE, ACS_HLINE);
    if(attributes != nullptr) {
      mvwprintw(characterScreen, 1, 1, (std::string("Hp: ") + std::to_string(attributes->baseHp)).c_str());
      mvwprintw(characterScreen, 2, 1, (std::string("Mana: ") + std::to_string(attributes->baseMana)).c_str());
      mvwprintw(characterScreen, 3, 1, (std::string("Armor: ") + std::to_string(attributes->baseArmor)).c_str());
      mvwprintw(characterScreen, 4, 1, (std::string("Soulburn: ") + std::to_string(attributes->baseSoulBurn)).c_str());
      mvwprintw(characterScreen, 5, 1, (std::string("Flow: ") + std::to_string(attributes->baseFlow)).c_str());
      mvwprintw(characterScreen, 6, 1, (std::string("Vision Range: ") + std::to_string(attributes->baseVisionRange)).c_str());
      mvwprintw(characterScreen, 7, 1, (std::string("Vision Power: ") + std::to_string(attributes->baseVisionPower)).c_str());
      mvwprintw(characterScreen, 8, 1, (std::string("Detection Range: ") + std::to_string(attributes->baseDetectionRange)).c_str());
    }
    mvwprintw(characterScreen, 1, ATTRIBUTES_LENGTH + 1, (std::string("Hp bonus on level up: ") + std::to_string(hpIncr)).c_str());
    mvwprintw(characterScreen, 2, ATTRIBUTES_LENGTH + 1, (std::string("Mana bonus on level up: ") + std::to_string(manaIncr)).c_str());
    mvwprintw(characterScreen, 3, ATTRIBUTES_LENGTH + 1, (std::string("Armor bonus on level up: ") + std::to_string(armorIncr)).c_str());
    mvwprintw(characterScreen, 4, ATTRIBUTES_LENGTH + 1, (std::string("Soulburn bonus on level up: ") + std::to_string(soulBurnIncr)).c_str());
    mvwprintw(characterScreen, 5, ATTRIBUTES_LENGTH + 1, (std::string("Flow bonus on level up: ") + std::to_string(flowIncr)).c_str());
    wattroff(characterScreen, COLOR_PAIR(color));
    return characterScreen;
  }

  std::vector<std::string> selectChoices(std::vector<Attributes *> startingAttributes, std::vector<Way *> startingWays) {
    Attributes * selectedAttributes = nullptr;
    Way * selectedRace = nullptr;
    Way * selectedOrigin = nullptr;
    Way * selectedCulture = nullptr;
    Way * selectedReligion = nullptr;
    Way * selectedProfession = nullptr;
    std::vector<Way *> races = std::vector<Way *>();
    std::vector<Way *> origins = std::vector<Way *>();
    std::vector<Way *> cultures = std::vector<Way *>();
    std::vector<Way *> religions = std::vector<Way *>();
    std::vector<Way *> professions = std::vector<Way *>();
    std::string characterName = "";
    std::vector<std::string> result = std::vector<std::string>(7);
    for(Way * way : startingWays) {
      switch(way->type) {
        case RACE:
          races.push_back(way);
          break;
        case ORIGIN:
          origins.push_back(way);
          break;
        case CULTURE:
          cultures.push_back(way);
          break;
        case RELIGION:
          religions.push_back(way);
          break;
        case PROFESSION:
          professions.push_back(way);
          break;
        default:
          ;
      }
    }
    bool done = false;
    int cursorX = 0;
    int cursorY = 0;
    clear();
    WINDOW * attributesScreen = subwin(stdscr, LINES, ATTRIBUTES_LENGTH + 2, 0, 0);
    WINDOW * raceScreen = subwin(stdscr, LINES, WAY_LENGTH + 2, 0, ATTRIBUTES_LENGTH + 2);
    WINDOW * originScreen = subwin(stdscr, LINES, WAY_LENGTH + 2, 0, ATTRIBUTES_LENGTH + 2 + WAY_LENGTH + 2);
    WINDOW * cultureScreen = subwin(stdscr, LINES, WAY_LENGTH + 2, 0, ATTRIBUTES_LENGTH + 2 + (WAY_LENGTH + 2) * 2);
    WINDOW * religionScreen = subwin(stdscr, LINES, WAY_LENGTH + 2, 0, ATTRIBUTES_LENGTH + 2 + (WAY_LENGTH + 2) * 3);
    WINDOW * professionScreen = subwin(stdscr, LINES, WAY_LENGTH + 2, 0, ATTRIBUTES_LENGTH + 2 + (WAY_LENGTH + 2) * 4);
    WINDOW * characterScreen = subwin(stdscr, LINES, COLS - (ATTRIBUTES_LENGTH + 2 + (WAY_LENGTH + 2) * 5), 0, ATTRIBUTES_LENGTH + 2 + (WAY_LENGTH + 2) * 5);
    box(attributesScreen, ACS_VLINE, ACS_HLINE);
    box(raceScreen, ACS_VLINE, ACS_HLINE);
    box(originScreen, ACS_VLINE, ACS_HLINE);
    box(cultureScreen, ACS_VLINE, ACS_HLINE);
    box(religionScreen, ACS_VLINE, ACS_HLINE);
    box(professionScreen, ACS_VLINE, ACS_HLINE);
    box(characterScreen, ACS_VLINE, ACS_HLINE);
    std::list<WINDOW *> screens = std::list<WINDOW *>();
    bool nameMode = false;
    while(!done) {
      int attributesCount = 0;
      int raceCount = 0;
      int originCount = 0;
      int cultureCount = 0;
      int religionCount = 0;
      int professionCount = 0;
      std::string to_print = "ATTRIBUTES";
      mvwprintw(attributesScreen, 1, 1 + ATTRIBUTES_LENGTH / 2 - to_print.length() / 2, to_print.c_str());
      to_print = "RACES";
      mvwprintw(raceScreen, 1, 1 + WAY_LENGTH / 2 - to_print.length() / 2, to_print.c_str());
      to_print = "ORIGINS";
      mvwprintw(originScreen, 1, 1 + WAY_LENGTH / 2 - to_print.length() / 2, to_print.c_str());
      to_print = "CULTURES";
      mvwprintw(cultureScreen, 1, 1 + WAY_LENGTH / 2 - to_print.length() / 2, to_print.c_str());
      to_print = "RELIGIONS";
      mvwprintw(religionScreen, 1, 1 + WAY_LENGTH / 2 - to_print.length() / 2, to_print.c_str());
      to_print = "PROFESSIONS";
      mvwprintw(professionScreen, 1, 1 + WAY_LENGTH / 2 - to_print.length() / 2, to_print.c_str());
      to_print = "CHARACTER";
      mvwprintw(characterScreen, 1, (COLS - (ATTRIBUTES_LENGTH + 2 + (WAY_LENGTH + 2) * 5)) / 2 - to_print.length() / 2, to_print.c_str());
      for(Attributes * attributes : startingAttributes) {
        int color = WHITE;
        if(selectedAttributes != nullptr && selectedAttributes->name == attributes->name) {
          color = GREEN;
        }
        if(cursorX == 0 && cursorY == attributesCount) {
          color = BLUE;
        }
        screens.push_back(displayAttributes(attributes, attributesCount++, color, attributesScreen, 0, 0));
      }
      wrefresh(attributesScreen);
      for(Way * way : races) {
        int color = WHITE;
        if(selectedRace != nullptr && selectedRace->name == way->name) {
          color = GREEN;
        }
        if(cursorX == 1 && cursorY == raceCount) {
          color = BLUE;
        }
        screens.push_back(displayWay(way, raceCount++, color, raceScreen, 0, ATTRIBUTES_LENGTH + 2));
      }
      wrefresh(raceScreen);
      for(Way * way : origins) {
        int color = WHITE;
        if(selectedOrigin != nullptr && selectedOrigin->name == way->name) {
          color = GREEN;
        }
        if(cursorX == 2 && cursorY == originCount) {
          color = BLUE;
        }
        screens.push_back(displayWay(way, originCount++, color, originScreen, 0, ATTRIBUTES_LENGTH + 2 + WAY_LENGTH + 2));
      }
      wrefresh(originScreen);
      for(Way * way : cultures) {
        int color = WHITE;
        if(selectedCulture != nullptr && selectedCulture->name == way->name) {
          color = GREEN;
        }
        if(cursorX == 3 && cursorY == cultureCount) {
          color = BLUE;
        }
        screens.push_back(displayWay(way, cultureCount++, color, cultureScreen, 0, ATTRIBUTES_LENGTH + 2 + (WAY_LENGTH + 2) * 2));
      }
      wrefresh(cultureScreen);
      for(Way * way : religions) {
        int color = WHITE;
        if(selectedReligion != nullptr && selectedReligion->name == way->name) {
          color = GREEN;
        }
        if(cursorX == 4 && cursorY == religionCount) {
          color = BLUE;
        }
        screens.push_back(displayWay(way, religionCount++, color, religionScreen, 0, ATTRIBUTES_LENGTH + 2 + (WAY_LENGTH + 2) * 3));
      }
      wrefresh(religionScreen);
      for(Way * way : professions) {
        int color = WHITE;
        if(selectedProfession != nullptr && selectedProfession->name == way->name) {
          color = GREEN;
        }
        if(cursorX == 5 && cursorY == professionCount) {
          color = BLUE;
        }
        screens.push_back(displayWay(way, professionCount++, color, professionScreen, 0, ATTRIBUTES_LENGTH + 2 + (WAY_LENGTH + 2) * 4));
      }
      wrefresh(professionScreen);
      int color = WHITE;
      if(characterName != "") {
        color = GREEN;
      }
      if(cursorX == 6 && cursorY == 0) {
        color = BLUE;
      }
      WINDOW * nameScreen = subwin(characterScreen, 3, COLS - (ATTRIBUTES_LENGTH + 2 + (WAY_LENGTH + 2) * 5) - 2, 2, 1 + ATTRIBUTES_LENGTH + 2 + (WAY_LENGTH + 2) * 5);
      screens.push_back(nameScreen);
      wattron(nameScreen, COLOR_PAIR(color));
      box(nameScreen, ACS_VLINE, ACS_HLINE);
      mvwprintw(nameScreen, 1, 1, (std::string("Name: ") + characterName).c_str());
      wattroff(nameScreen, COLOR_PAIR(color));
      color = WHITE;
      if(cursorX == 6 && cursorY == 1) {
        color = BLUE;
      }
      screens.push_back(displayCharacter(selectedAttributes, selectedRace, selectedOrigin, selectedCulture, selectedReligion, selectedProfession, color, characterScreen, COLS - (ATTRIBUTES_LENGTH + 2 + (WAY_LENGTH + 2) * 5) - 2, 5, 1 + ATTRIBUTES_LENGTH + 2 + (WAY_LENGTH + 2) * 5));
      color = WHITE;
      if(characterName != "" && selectedAttributes != nullptr && selectedRace != nullptr && selectedOrigin != nullptr && selectedCulture != nullptr && selectedReligion != nullptr && selectedProfession != nullptr) {
        color = GREEN;
      }
      if(cursorX == 6 && cursorY == 2) {
        color = BLUE;
      }
      WINDOW * confirmScreen = subwin(characterScreen, 3, COLS - (ATTRIBUTES_LENGTH + 2 + (WAY_LENGTH + 2) * 5) - 2, 5 + ATTRIBUTES_HEIGHT - 1, 1 + ATTRIBUTES_LENGTH + 2 + (WAY_LENGTH + 2) * 5);
      screens.push_back(confirmScreen);
      wattron(confirmScreen, COLOR_PAIR(color));
      box(confirmScreen, ACS_VLINE, ACS_HLINE);
      mvwprintw(confirmScreen, 1, 1, std::string("CONFIRM CHARACTER CREATION").c_str());
      wattroff(confirmScreen, COLOR_PAIR(color));
      wrefresh(characterScreen);
      bool done2 = false;
      while(!done2) {
        char keyPressed = getch();
        if(!nameMode) {
          switch(keyPressed) {
            case '4':
            case char(68): // LEFT ARROW
              cursorX = (cursorX - 1 + 7) % 7;
              done2 = true;
              break;
            case '8':
            case char(65): { // UP ARROW
              if(cursorX == 0) {
                cursorY = (cursorY - 1 + startingAttributes.size()) % startingAttributes.size();
              } else if(cursorX == 1) {
                cursorY = (cursorY - 1 + races.size()) % races.size();
              } else if(cursorX == 2) {
                cursorY = (cursorY - 1 + origins.size()) % origins.size();
              } else if(cursorX == 3) {
                cursorY = (cursorY - 1 + cultures.size()) % cultures.size();
              } else if(cursorX == 4) {
                cursorY = (cursorY - 1 + religions.size()) % religions.size();
              } else if(cursorX == 5) {
                cursorY = (cursorY - 1 + professions.size()) % professions.size();
              } else if(cursorX == 6) {
                cursorY = (cursorY - 1 + 3) % 3;
              }
              done2 = true;
              break;
            }
            case '6':
            case char(67): // RIGHT ARROW
              cursorX = (cursorX + 1) % 7;
              done2 = true;
              break;
            case '2':
            case char(66): { // DOWN ARROW
              if(cursorX == 0) {
                cursorY = (cursorY + 1) % startingAttributes.size();
              } else if(cursorX == 1) {
                cursorY = (cursorY + 1) % races.size();
              } else if(cursorX == 2) {
                cursorY = (cursorY + 1) % origins.size();
              } else if(cursorX == 3) {
                cursorY = (cursorY + 1) % cultures.size();
              } else if(cursorX == 4) {
                cursorY = (cursorY + 1) % religions.size();
              } else if(cursorX == 5) {
                cursorY = (cursorY + 1) % professions.size();
              } else if(cursorX == 6) {
                cursorY = (cursorY + 1) % 3;
              }
              done2 = true;
              break;
            }
            case char(10): { // ENTER
              if(cursorX == 0) {
                selectedAttributes = startingAttributes[cursorY];
              } else if(cursorX == 1) {
                selectedRace = races[cursorY];
              } else if(cursorX == 2) {
                selectedOrigin = origins[cursorY];
              } else if(cursorX == 3) {
                selectedCulture = cultures[cursorY];
              } else if(cursorX == 4) {
                selectedReligion = religions[cursorY];
              } else if(cursorX == 5) {
                selectedProfession = professions[cursorY];
              } else if(cursorX == 6) {
                if(cursorY == 0) {
                  nameMode = true;
                } else if(cursorY == 2) {
                  if(characterName != "" && selectedAttributes != nullptr && selectedRace != nullptr && selectedOrigin != nullptr && selectedCulture != nullptr && selectedReligion != nullptr && selectedProfession != nullptr) {
                    done = true;
                  }
                }
              }
              done2 = true;
              break;
            }
            default:
              ;
          }
        } else { // nameMode is true
          switch(keyPressed) {
            case char(10): // ENTER
              nameMode = false;
              done2 = true;
              break;
            case char(127): // BACK_SPACE
              if(characterName != "") {
                characterName = characterName.substr(0, characterName.length() - 1);
              }
              done2 = true;
              break;
            default:
              characterName += keyPressed;
              done2 = true;
          }
        }
      }
      for(WINDOW * screen : screens) {
        wclear(screen);
        delwin(screen);
      }
      screens.clear();
    }
    result[0] = characterName;
    result[1] = selectedAttributes->name;
    result[2] = selectedRace->name;
    result[3] = selectedOrigin->name;
    result[4] = selectedCulture->name;
    result[5] = selectedReligion->name;
    result[6] = selectedProfession->name;
    return result;
  }
}
