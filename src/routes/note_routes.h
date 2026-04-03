#pragma once
#include "crow.h"
#include "../storage/note_store.h"

template <typename App>
void registerNoteRoutes(App &app, NoteStore &store)
{
    // ========================
    // CORS
    // ========================
    CROW_ROUTE(app, "/notes").methods("OPTIONS"_method)([]()
                                                        {
        crow::response res;
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
        res.set_header("Access-Control-Allow-Headers", "Content-Type");
        return res; });

    // ========================
    // GET /notes
    // ========================
    CROW_ROUTE(app, "/notes").methods("GET"_method)([&store]()
                                                    {
        auto notes = store.getAll();
        crow::json::wvalue result = crow::json::wvalue::list();

        for (size_t i = 0; i < notes.size(); i++)
        {
            result[i]["id"] = notes[i].id;
            result[i]["title"] = notes[i].title;
            result[i]["content"] = notes[i].content;
        }

        crow::response res(result);
        res.set_header("Access-Control-Allow-Origin", "*");
        return res; });

    // ========================
    // POST /notes
    // ========================
    CROW_ROUTE(app, "/notes").methods("POST"_method)([&store](const crow::request &req)
                                                     {
        auto body = crow::json::load(req.body);

        if (!body || !body.has("content"))
        {
            crow::json::wvalue err;
            err["error"] = "Invalid JSON";
            return crow::response(400, err);
        }

        std::string content = std::string(body["content"].s());
        std::string title = body.has("title") ? std::string(body["title"].s()) : "";

        if (content.empty())
        {
            crow::json::wvalue err;
            err["error"] = "Content is required";
            return crow::response(400, err);
        }

        auto note = store.add(title, content);

        crow::json::wvalue res;
        res["id"] = note.id;
        res["title"] = note.title;
        res["content"] = note.content;

        crow::response response(res);
        response.set_header("Content-Type", "application/json");
        response.set_header("Access-Control-Allow-Origin", "*");

        return response; });

    // ========================
    // DELETE /notes/:id
    // ========================
    CROW_ROUTE(app, "/notes/<int>").methods("DELETE"_method)([&store](int id)
                                                             {
        bool success = store.remove(id);

        if (!success)
        {
            crow::json::wvalue err;
            err["error"] = "Not found";
            return crow::response(404, err);
        }

        crow::json::wvalue res;
        res["message"] = "Deleted";

        crow::response response(res);
        response.set_header("Access-Control-Allow-Origin", "*");

        return response; });

    // ========================
    // PUT /notes/:id
    // ========================
    CROW_ROUTE(app, "/notes/<int>").methods("PUT"_method)([&store](const crow::request &req, int id)
                                                          {
        auto body = crow::json::load(req.body);

        if (!body)
        {
            crow::json::wvalue err;
            err["error"] = "Invalid JSON";
            return crow::response(400, err);
        }

        // ✅ FIX kiểu dữ liệu
        std::string title = body.has("title") ? std::string(body["title"].s()) : "";
        std::string content = body.has("content") ? std::string(body["content"].s()) : "";

        bool success = store.update(id, title, content);

        if (!success)
        {
            crow::json::wvalue err;
            err["error"] = "Not found";
            return crow::response(404, err);
        }

        crow::json::wvalue res;
        res["message"] = "Updated";

        crow::response response(res);
        response.set_header("Access-Control-Allow-Origin", "*");

        return response; });
}