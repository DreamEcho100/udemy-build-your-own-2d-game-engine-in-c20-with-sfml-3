#include <fstream>

#include "SaveManager.h"

SaveManager::SaveManager() {
  std::ifstream file("assets/Save.json");
  if (file) {
    values_ = nlohmann::json::parse(file);
  } else {
    values_ = nlohmann::json::object();
  }
}

SaveManager::~SaveManager() {
  if (values_.empty()) {
    return;
  }

  std::ofstream file("assets/Save.json");
  if (file) {
    file << values_.dump(4);
  }
}

bool SaveManager::Has(std::string_view key) const { return values_.contains(key); }

void SaveManager::Erase(std::string_view key) { values_.erase(key); }

void SaveManager::Clear() { values_.clear(); }
