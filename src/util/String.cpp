#include <algorithm>
#include <iostream>

#include "util/String.h"

std::string String::extract(std::stringstream * ss) {
  std::string to_return = "";
  std::string extracted;
  int open_count = 0;
  int close_count = 0;
  do {
    getline(*ss, extracted, '}');
    // the '}' has been removed by the getline
    to_return += extracted + "}";
    close_count++;
    open_count += std::count(extracted.begin(), extracted.end(), '{');
    if(open_count == 0) {
      std::cerr << "Broken message: " << to_return << std::endl;
      return "ERROR";
    }
  } while(open_count != close_count);
  return to_return.substr(1, to_return.length() - 2);
}

bool String::extract_bool(std::stringstream * ss) {
  std::istringstream ss_bool(extract(ss));
  bool result;
  ss_bool >> result;
  return result;
}

int String::extract_int(std::stringstream * ss) {
  return stoi(extract(ss));
}

long String::extract_long(std::stringstream * ss) {
  return stol(extract(ss));
}

float String::extract_float(std::stringstream * ss) {
  std::string msg = extract(ss);
  float result = stof(msg);
  int int_part = (int) result;
  if(result - int_part == 0.) {
    // float formating is not the same everywhere
    std::replace(msg.begin(), msg.end(), '.', ',');
    result = stof(msg);
  }
  return result;
}

void String::insert(std::stringstream * ss, std::string data) { *ss << '{' << data << '}'; }
void String::insert_bool(std::stringstream * ss, bool data) { *ss << '{' << data << '}'; }
void String::insert_int(std::stringstream * ss, int data) { *ss << '{' << data << '}' ; }
void String::insert_long(std::stringstream * ss, long data) { *ss << '{' << data << '}' ; }
void String::insert_float(std::stringstream * ss, float data) { *ss << '{' << data << '}'; }
