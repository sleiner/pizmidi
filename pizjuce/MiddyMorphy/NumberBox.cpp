
#include "NumberBox.h"

// Component::keyPressed  (  const KeyPress &  key   ) 
bool NumberBox::keyPressed(const KeyPress & key) {
  // Bouml preserved body begin 0004868D
	if(String("0123456789.,-+").indexOfChar(key.getTextCharacter()) > -1 || key == KeyPress::returnKey  || key.isKeyCode (KeyPress::escapeKey) )
	{
		TextEditor::keyPressed(key);
	}
	return false;
  // Bouml preserved body end 0004868D
}

