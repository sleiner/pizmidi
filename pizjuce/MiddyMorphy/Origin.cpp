
#include "Origin.h"
#include "ZoomingShiftingComponent.h"

//==============================================================================

void Origin::paint(Graphics & g) {
  // Bouml preserved body begin 0003238D
	g.drawLine(0,0,(float)getWidth(),0);
	g.drawLine(0,0,0,(float)getHeight());
  // Bouml preserved body end 0003238D
}

Origin::Origin() {
  // Bouml preserved body begin 0003240D
	this->setSize(10,10);
  // Bouml preserved body end 0003240D
}

void Origin::resized() {
  // Bouml preserved body begin 0003B08D
  // Bouml preserved body end 0003B08D
}

void Origin::moved() {
  // Bouml preserved body begin 0003C38D
  // Bouml preserved body end 0003C38D
}

