#include "httplib.h"
#include <nlohmann/json.hpp>
#include <sqlite3.h>

#include <cstdlib>
#include <iostream>
#include <string>

using json = nlohmann::json;

sqlite3* db = nullptr;

bool executeSQL(const std::string& sql) {
    char* error = nullptr;
    int rc = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &error);
    if (rc != SQLITE_OK) {
        std::cerr << "SQLite error: " << (error ? error : "unknown") << "\n";
        sqlite3_free(error);
        return false;
    }
    return true;
}

json expenseRows() {
    json result = json::array();
    sqlite3_stmt* stmt = nullptr;
    const char* sql = "SELECT id, category, amount, expense_date, note FROM expenses ORDER BY id DESC";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            result.push_back({
                {"id", sqlite3_column_int(stmt, 0)},
                {"category", reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1))},
                {"amount", sqlite3_column_double(stmt, 2)},
                {"date", reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3))},
                {"note", reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4))}
            });
        }
    }
    sqlite3_finalize(stmt);
    return result;
}

double getBudget() {
    sqlite3_stmt* stmt = nullptr;
    double budget = 10000.0;
    if (sqlite3_prepare_v2(db, "SELECT value FROM settings WHERE key='budget'", -1, &stmt, nullptr) == SQLITE_OK &&
        sqlite3_step(stmt) == SQLITE_ROW) {
        budget = sqlite3_column_double(stmt, 0);
    }
    sqlite3_finalize(stmt);
    return budget;
}

void setBudget(double budget) {
    sqlite3_stmt* stmt = nullptr;
    const char* sql = "INSERT INTO settings(key,value) VALUES('budget',?) "
                      "ON CONFLICT(key) DO UPDATE SET value=excluded.value";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_double(stmt, 1, budget);
        sqlite3_step(stmt);
    }
    sqlite3_finalize(stmt);
}

double totalSpent() {
    sqlite3_stmt* stmt = nullptr;
    double total = 0;
    if (sqlite3_prepare_v2(db, "SELECT COALESCE(SUM(amount),0) FROM expenses", -1, &stmt, nullptr) == SQLITE_OK &&
        sqlite3_step(stmt) == SQLITE_ROW) {
        total = sqlite3_column_double(stmt, 0);
    }
    sqlite3_finalize(stmt);
    return total;
}

void replyJson(const httplib::Request&, httplib::Response& res, const json& body, int status = 200) {
    res.status = status;
    res.set_content(body.dump(2), "application/json; charset=utf-8");
}

int main() {
    const char* dbPath = std::getenv("DB_PATH");
    if (!dbPath || std::string(dbPath).empty()) dbPath = "budget_tracker.db";
    if (sqlite3_open(dbPath, &db) != SQLITE_OK) {
        std::cerr << "Could not open SQLite database.\n";
        return 1;
    }

    executeSQL("CREATE TABLE IF NOT EXISTS expenses ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT,"
               "category TEXT NOT NULL,"
               "amount REAL NOT NULL CHECK(amount > 0),"
               "expense_date TEXT NOT NULL,"
               "note TEXT NOT NULL DEFAULT '')");
    executeSQL("CREATE TABLE IF NOT EXISTS settings (key TEXT PRIMARY KEY, value REAL NOT NULL)");
    if (getBudget() <= 0) setBudget(10000.0);

    httplib::Server server;

    server.set_pre_routing_handler([](const httplib::Request&, httplib::Response& res) {
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Methods", "GET, POST, DELETE, OPTIONS");
        res.set_header("Access-Control-Allow-Headers", "Content-Type");
        return httplib::Server::HandlerResponse::Unhandled;
    });

    server.Options(R"(.*)", [](const httplib::Request&, httplib::Response& res) {
        res.status = 204;
    });

    server.Get("/api/health", [](const httplib::Request& req, httplib::Response& res) {
        replyJson(req, res, {{"status", "ok"}, {"message", "Student Smart Budget Tracker backend is running"}});
    });

    server.Get("/api/summary", [](const httplib::Request& req, httplib::Response& res) {
        double budget = getBudget();
        double spent = totalSpent();
        replyJson(req, res, {{"budget", budget}, {"spent", spent}, {"remaining", budget - spent}, {"expenses", expenseRows()}});
    });

    server.Post("/api/budget", [](const httplib::Request& req, httplib::Response& res) {
        try {
            auto body = json::parse(req.body);
            double budget = body.at("budget").get<double>();
            if (budget <= 0 || budget > 1000000000) {
                replyJson(req, res, {{"error", "Budget must be greater than zero."}}, 400);
                return;
            }
            setBudget(budget);
            replyJson(req, res, {{"ok", true}, {"budget", budget}});
        } catch (...) {
            replyJson(req, res, {{"error", "Invalid JSON or budget."}}, 400);
        }
    });

    server.Post("/api/expenses", [](const httplib::Request& req, httplib::Response& res) {
        try {
            auto body = json::parse(req.body);
            std::string category = body.at("category").get<std::string>();
            double amount = body.at("amount").get<double>();
            std::string date = body.at("date").get<std::string>();
            std::string note = body.value("note", "");
            if (category.empty() || date.empty() || amount <= 0 || amount > 1000000000) {
                replyJson(req, res, {{"error", "Enter a category, valid date, and amount greater than zero."}}, 400);
                return;
            }
            sqlite3_stmt* stmt = nullptr;
            const char* sql = "INSERT INTO expenses(category,amount,expense_date,note) VALUES(?,?,?,?)";
            if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
                replyJson(req, res, {{"error", "Could not prepare database insert."}}, 500);
                return;
            }
            sqlite3_bind_text(stmt, 1, category.c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_double(stmt, 2, amount);
            sqlite3_bind_text(stmt, 3, date.c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_text(stmt, 4, note.c_str(), -1, SQLITE_TRANSIENT);
            int rc = sqlite3_step(stmt);
            sqlite3_finalize(stmt);
            if (rc != SQLITE_DONE) {
                replyJson(req, res, {{"error", "Could not save expense."}}, 500);
                return;
            }
            replyJson(req, res, {{"ok", true}, {"id", static_cast<int>(sqlite3_last_insert_rowid(db))}}, 201);
        } catch (...) {
            replyJson(req, res, {{"error", "Invalid JSON. Check the expense fields."}}, 400);
        }
    });

    server.Delete(R"(/api/expenses/(\d+))", [](const httplib::Request& req, httplib::Response& res) {
        int id = std::stoi(req.matches[1]);
        sqlite3_stmt* stmt = nullptr;
        sqlite3_prepare_v2(db, "DELETE FROM expenses WHERE id=?", -1, &stmt, nullptr);
        sqlite3_bind_int(stmt, 1, id);
        int rc = sqlite3_step(stmt);
        sqlite3_finalize(stmt);
        if (rc != SQLITE_DONE) {
            replyJson(req, res, {{"error", "Could not delete expense."}}, 500);
            return;
        }
        replyJson(req, res, {{"ok", true}});
    });

    server.Delete("/api/expenses", [](const httplib::Request& req, httplib::Response& res) {
        executeSQL("DELETE FROM expenses");
        replyJson(req, res, {{"ok", true}});
    });

    const char* portEnv = std::getenv("PORT");
    int port = portEnv ? std::atoi(portEnv) : 8080;
    const char* host = std::getenv("HOST");
    if (!host || std::string(host).empty()) host = "0.0.0.0";
    std::cout << "Student Smart Budget Tracker backend running on port " << port << "\\n";
    std::cout << "Database: budget_tracker.db (created automatically)\\n";
    if (!server.listen(host, port)) {
        std::cerr << "Could not start server on port 8080. Is it already in use?\n";
        sqlite3_close(db);
        return 1;
    }
    sqlite3_close(db);
    return 0;
}
