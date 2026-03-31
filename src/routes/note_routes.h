#pragma once
#include "crow.h"
#include "../storage/note_store.h"

template <typename App>
void registerNoteRoutes(App &app, NoteStore &store)
{
    // GET /notes
    CROW_ROUTE(app, "/notes")
        .methods("GET"_method)([&store]()
                               {
        auto notes = store.getAll();

        crow::json::wvalue result = crow::json::wvalue::list();

        for (size_t i = 0; i < notes.size(); i++)
        {
            result[i]["id"] = notes[i].id;
            result[i]["content"] = notes[i].content;
        }

        return result; });

    // POST /notes
    CROW_ROUTE(app, "/notes")
        .methods("POST"_method)([&store](const crow::request &req)
                                {
        auto body = crow::json::load(req.body);

        if (!body || !body.has("content"))
        {
            return crow::response(400, "Invalid request");
        }

        auto note = store.add(body["content"].s());

        crow::json::wvalue result;
        result["id"] = note.id;
        result["content"] = note.content;

        return crow::response{result}; });

    // DELETE
    CROW_ROUTE(app, "/notes/<int>")
        .methods("DELETE"_method)([&store](int id)
                                  {
        bool success = store.remove(id);

        if (!success)
        {
            return crow::response(404, "Not found");
        }

        return crow::response(200, "Deleted"); });

    // PUT
    CROW_ROUTE(app, "/notes/<int>")
        .methods("PUT"_method)([&store](const crow::request &req, int id)
                               {
        auto body = crow::json::load(req.body);

        if (!body || !body.has("content"))
        {
            return crow::response(400, "Invalid request");
        }

        bool success = store.update(id, body["content"].s());

        if (!success)
        {
            return crow::response(404, "Not found");
        }

        return crow::response(200, "Updated"); });
}