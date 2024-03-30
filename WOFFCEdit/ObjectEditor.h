#pragma once
#include <memory>
#include <stack>
#include <vector>

#include "DisplayObject.h"

class DisplayObject;
class InputCommands;

enum CommandType
{
	DEL,
	PASTE
};

struct ObjectData
{
		DisplayObject object;
	CommandType command;
};

class ObjectEditor
{
public:
	ObjectEditor();
	~ObjectEditor();
	void HandleKeyInput(InputCommands* Input,std::vector<DisplayObject>&);
	void SetSelection(int i) { selection = i; }


private:

	std::unique_ptr<DisplayObject> CopyObject;
	std::stack<ObjectData>RedoStack;
	std::stack<ObjectData> UndoStack;

	void Copy(int i, std::vector<DisplayObject>);
	void undo_redo_handler(ObjectData &tempUndo, std::vector<DisplayObject> &displayList);
	void Undo(std::vector<DisplayObject>& displayList);
	void Redo(std::vector<DisplayObject>& displayList);
	void Delete(int i, std::vector<DisplayObject>&);
	void Paste(std::vector<DisplayObject>& displayList);
	int selection;

};

