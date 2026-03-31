#define ASIO_STANDALONE
#include "crow.h"

#include "storage/note_store.h"
#include "routes/note_routes.h"
#include "crow/middlewares/cors.h"

int main()
{
    crow::App<crow::CORSHandler> app;

    NoteStore store;

    // ✅ CORS CONFIG (PHẢI đặt trước run)
    auto &cors = app.get_middleware<crow::CORSHandler>();

    cors.global()
        .headers("Content-Type")
        .methods("GET"_method, "POST"_method, "PUT"_method, "DELETE"_method)
        .origin("*"); // ⚠️ dùng origin, không phải origins

    // test route
    CROW_ROUTE(app, "/")([]()
                         { return "Fresh Start Backend is running!"; });

    // register routes
    registerNoteRoutes(app, store);

    app.port(18080).multithreaded().run();
}