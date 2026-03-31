#pragma once
#include <vector>
#include <string>
#include "../models/note.h"

class NoteStore
{
private:
    std::vector<Note> notes;
    int currentId = 1;

public:
    std::vector<Note> getAll()
    {
        return notes;
    }

    Note add(const std::string &content)
    {
        Note note;
        note.id = currentId++;
        note.content = content;

        notes.push_back(note);
        return note;
    }

    bool remove(int id)
    {
        for (auto it = notes.begin(); it != notes.end(); ++it)
        {
            if (it->id == id)
            {
                notes.erase(it);
                return true;
            }
        }
        return false;
    }

    bool update(int id, const std::string &content)
    {
        for (auto &note : notes)
        {
            if (note.id == id)
            {
                note.content = content;
                return true;
            }
        }
        return false;
    }
};