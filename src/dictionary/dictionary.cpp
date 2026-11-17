#include "kalaime/dictionary/dictionary.hpp"

#include <sqlite3.h>

#include <filesystem>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace kalaime::dictionary {

struct Dictionary::Impl {
    sqlite3* db = nullptr;

    explicit Impl(const std::string& path) {
        const std::filesystem::path database_path = path;
        if (!database_path.parent_path().empty()) {
            std::filesystem::create_directories(database_path.parent_path());
        }

        if (sqlite3_open(database_path.string().c_str(), &db) != SQLITE_OK) {
            throw std::runtime_error("Unable to open KALAIME dictionary database");
        }
    }

    ~Impl() {
        if (db != nullptr) {
            sqlite3_close(db);
            db = nullptr;
        }
    }
};

Dictionary::Dictionary(const std::string& database_path) : impl_(new Impl(database_path)) {
    initialize_schema();
}

Dictionary::~Dictionary() { delete impl_; }

void Dictionary::initialize_schema() {
    const char* sql =
        "CREATE TABLE IF NOT EXISTS dictionary_entries ("
        "  id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "  word TEXT NOT NULL UNIQUE,"
        "  normalized TEXT NOT NULL,"
        "  frequency INTEGER NOT NULL DEFAULT 1,"
        "  created_at TEXT NOT NULL DEFAULT CURRENT_TIMESTAMP);"
        "CREATE INDEX IF NOT EXISTS idx_dictionary_normalized ON dictionary_entries(normalized);";

    char* error = nullptr;
    const int rc = sqlite3_exec(impl_->db, sql, nullptr, nullptr, &error);
    if (rc != SQLITE_OK) {
        std::string message = error == nullptr ? "unknown SQLite error" : error;
        sqlite3_free(error);
        throw std::runtime_error("Unable to initialize dictionary schema: " + message);
    }
}

void Dictionary::add_entry(const std::string& word, const std::string& normalized, int frequency) {
    const std::string statement =
        "INSERT OR REPLACE INTO dictionary_entries(word, normalized, frequency) VALUES(?, ?, ?)";
    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(impl_->db, statement.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        throw std::runtime_error("Unable to prepare dictionary insert statement");
    }

    sqlite3_bind_text(stmt, 1, word.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, normalized.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 3, frequency);

    const int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    if (rc != SQLITE_DONE) {
        throw std::runtime_error("Dictionary insert failed");
    }
}

std::vector<std::string> Dictionary::find_matches(const std::string& normalized) const {
    const std::string query =
        "SELECT word FROM dictionary_entries WHERE normalized = ? ORDER BY frequency DESC, word "
        "ASC";
    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(impl_->db, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        return {};
    }

    sqlite3_bind_text(stmt, 1, normalized.c_str(), -1, SQLITE_TRANSIENT);
    std::vector<std::string> results;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        const unsigned char* value = sqlite3_column_text(stmt, 0);
        if (value != nullptr) {
            results.emplace_back(reinterpret_cast<const char*>(value));
        }
    }
    sqlite3_finalize(stmt);
    return results;
}

std::vector<std::string> Dictionary::words_for_prefix(const std::string& prefix) const {
    const std::string query =
        "SELECT word FROM dictionary_entries WHERE word LIKE ? ORDER BY frequency DESC, word ASC "
        "LIMIT 10";
    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(impl_->db, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        return {};
    }

    const std::string pattern = prefix + "%";
    sqlite3_bind_text(stmt, 1, pattern.c_str(), -1, SQLITE_TRANSIENT);

    std::vector<std::string> results;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        const unsigned char* value = sqlite3_column_text(stmt, 0);
        if (value != nullptr) {
            results.emplace_back(reinterpret_cast<const char*>(value));
        }
    }
    sqlite3_finalize(stmt);
    return results;
}

}  // namespace kalaime::dictionary
