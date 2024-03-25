#pragma once
#include <stack>

#include "DisplayChunk.h"

class DisplayObject;
class InputCommands;

enum CommandType
{
	COPY,
	UNDO,
	REDO,
	DEL,
	PASTE
};

class ObjectEditor
{
public:
	ObjectEditor();
	~ObjectEditor();
	void HandleKeyInput(InputCommands* Input,std::vector<DisplayObject>);
	void SetSelection(int i) { selection = i; }


private:

	void Copy(int i, std::vector<DisplayObject>);
	void undo_redo_handler(std::pair<DisplayObject, CommandType> tempUndo, std::vector<DisplayObject> displayList);
	void Undo(std::vector<DisplayObject> displayList);
	void Redo(std::vector<DisplayObject> displayList);
	void Delete(int i, std::vector<DisplayObject>);
	void Paste(std::vector<DisplayObject> displayList);
	int selection;
	std::unique_ptr<DisplayObject> CopyObject;
	std::stack<std::pair<DisplayObject,CommandType>> RedoStack;
	std::stack<std::pair<DisplayObject,CommandType>> UndoStack;

};

