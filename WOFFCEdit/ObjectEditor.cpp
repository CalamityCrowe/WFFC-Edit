#include "ObjectEditor.h"

#include "InputCommands.h"
#include "DisplayObject.h"

ObjectEditor::ObjectEditor()
{
	selection = -1;

}

ObjectEditor::~ObjectEditor()
{
}

void ObjectEditor::Copy(int i, std::vector<DisplayObject> displayList)
{
	if (i < 0)
	{
		return;
	}
	CopyObject = std::make_unique<DisplayObject>(displayList[i]); // this copies the object at this point
}


void ObjectEditor::undo_redo_handler(ObjectData& tempUndo, std::vector<DisplayObject>& displayList)
{
	switch (tempUndo.command)
	{
	case CommandType::PASTE:

		for (auto it = displayList.begin(); it != displayList.end(); ++it) // loops through the display list
		{
			if (it->m_ID == tempUndo.object.m_ID) // checks if the ID of the object is the same as the ID of the object in the display list
			{
				Delete(it - displayList.begin(), displayList,); // calls the delete function
				break; // breaks the loop
			}
		}
		break;
	case CommandType::DEL: // checks if the command type is delete

		DisplayObject a = tempUndo.object; // this gets the object at this point
		DisplayObject swap; // this creates a new object to handle the swapping
		for (int i = 0; i < displayList.size(); ++i) // loops through all the objects in the list
		{
			if (displayList[i].m_ID == a.m_ID) // checks if the ID of the object is the same as the ID of the object in the display list
			{
				swap = displayList[i]; // this gets the object at this point
				displayList[i] = a; // this sets the object to the object at this point
				a = swap; // this sets the object to the swap object
				a.m_ID++; // increments the ID of the object
			}
		}
		displayList.push_back(a); // this pushes the object to the display list
		break;
	}
}

void ObjectEditor::Undo(std::vector<DisplayObject>& displayList)
{
	if (UndoStack.empty() == false) // checks if the undo stack is not empty
	{
		ObjectData tempUndo = UndoStack.top(); // this gets the object at the top of the undo stack
		undo_redo_handler(tempUndo, displayList); // this calls the undo redo handler

		UndoStack.pop(); // this pops the object from the undo stack
		if (tempUndo.command == CommandType::DEL) // checks if the command type is delete
		{
			tempUndo.command = CommandType::PASTE; // this sets the command type to paste
		}
		else
		{
			tempUndo.command = CommandType::DEL; // this sets the command type to delete
		}
		RedoStack.push(tempUndo); // this pushes the object to the redo stack
	}
}

void ObjectEditor::Redo(std::vector<DisplayObject>& displayList)
{
	if (RedoStack.empty() == false) // checks if the redo stack is not empty
	{
		ObjectData tempRedo = RedoStack.top(); // this gets the object at the top of the redo stack
		undo_redo_handler(tempRedo, displayList); // this calls the undo redo handler

		RedoStack.pop(); // this pops the object from the redo stack
		if (tempRedo.command == CommandType::DEL) // checks if the command type is delete
		{
			tempRedo.command = CommandType::PASTE; // this sets the command type to paste
		}
		else
		{
			tempRedo.command = CommandType::DEL; // this sets the command type to delete
		}
		UndoStack.push(tempRedo); // this pushes the object to the undo stack
	}
}

void ObjectEditor::Delete(int i, std::vector<DisplayObject>& displayList, bool flushRedo)
{
	if (i < 0) // checks if the object is valid
	{
		return;
	}
	ObjectData newPair; // this creates a new pair object
	newPair.object = displayList[i]; // this sets the object to the object at this point
	newPair.command = CommandType::DEL; // this sets the command type to delete
	UndoStack.push(newPair); // this pushes the object to the undo stack
	displayList.erase(displayList.begin() + i); // this deletes the object at this point
	for (int newID = i; newID < displayList.size(); ++newID) // this loops through the objects
	{
		displayList[newID].m_ID = newID; // this sets the ID of the object to the new ID
	}
	selection = -1; // resets this to -1 so that the object is no longer selected for any other operations
	if(flushRedo)
	{
		while (RedoStack.empty() == false) // checks if the redo stack is not empty
		{
						RedoStack.pop(); // this pops the object from the redo stack
		}
	}
}

void ObjectEditor::Paste(std::vector<DisplayObject>& displayList)
{
	if (CopyObject == nullptr) // checks if the object is valid
	{
		return; // returns if the object is not valid
	}
	else
	{

		DisplayObject a = *CopyObject; // this gets the object it is meant to copy from the pointer

		bool b = false; // this sets the bool to false
		int newID = 0; // this sets the new ID to 0
		if (displayList.size() > 0) // checks if the display list is greater than 0
		{
			for (int i = 0; i < displayList.size(); ++i) // this loops through the display list
			{
				if (a.m_ID == displayList[i].m_ID) // checks if the ID of the object is the same as the ID of the object in the display list
				{
					b = true; // sets the bool to true
					break; // breaks the loop
				}
				newID = std::max(newID, displayList[i].m_ID); // this sets the new ID to the max of the new ID and the ID of the object in the display list
			}
		}
		if (b)// checks if the bool is true
		{
			while (true) // loops through this till it is broken or eternity (whichever comes first)
			{
				bool idExists = false; // sets the bool to false
				for (const auto& obj : displayList) // loops through the display list
				{
					if (newID == obj.m_ID) // checks if the new ID is the same as the ID of the object in the display list
					{
						idExists = true; // sets the bool to true
						break; // breaks the loop
					}
				}
				if (!idExists) // checks if the bool is false
				{
					break; // breaks the loop
				}
				newID++; // increments the new ID
			}

			a.m_ID = newID + 1; // sets the ID of the object to the new ID + 1
		}


		displayList.push_back(a); // this pushes the object to the display list
		ObjectData newPair; // this creates a new pair object
		newPair.object = a; // this sets the object to the object at this point
		newPair.command = CommandType::PASTE;	// this sets the command type to paste
		UndoStack.push(newPair); // this pushes the object to the undo stack
		CopyObject = nullptr; // this sets the copy object to null
	}
}

void ObjectEditor::HandleKeyInput(InputCommands* Inputs, std::vector<DisplayObject>& displayList)
{
	if (Inputs) // checks that the inputs are valid
	{
		if (Inputs->deleteSelected) // checks if the delete key is pressed
		{
			Delete(selection, displayList); // calls the delete function
		}
		if (Inputs->copy) // checks if the copy key is pressed
		{
			Copy(selection, displayList); // calls the copy function
		}
		if (Inputs->paste) // checks if the paste key is pressed
		{
			Paste(displayList); // calls the paste function
		}
		if (Inputs->undo) // checks if the undo key is pressed
		{
			Undo(displayList); // calls the undo function
		}
		if (Inputs->redo) // checks if the redo key is pressed
		{
			Redo(displayList); // calls the redo function
		}
	}

}
