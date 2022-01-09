#pragma once

#include "juce_core/juce_core.h"
#include "juce_gui_basics/juce_gui_basics.h"

#include "Origin.h"

using namespace juce;

namespace juce { class XmlElement; }
class ZoomableShiftableComponent;

class ZoomingShiftingComponent : public juce::Component {
  public:
    juce::XmlElement* getXml(const String tagName);

    void setFromXml(juce::XmlElement * data);

    void dragOrigin(const MouseEvent & e);

    void startDragOrigin(const MouseEvent& e);


  private:
    Origin origin;


  public:
    void zoom(float multX, float multY, float x, float y);

    //drag the origin around
    //call the reArrangeChildren
    void shift(const juce::Point<int> newPosition);

    //call all childrens reposition method
    void rePositionChildren();

    float getXOffset();

    float getYOffset();


  private:
    float zoomFactorX;

    float zoomFactorY;

    OwnedArray<ZoomableShiftableComponent> zoomedComponents;


  public:
    float getZoomFactorX();

    float getZoomFactorY();

    void childBoundsChanged(juce::Component * component) override;


  private:
    juce::ComponentDragger dragger;


  public:
    ZoomingShiftingComponent();

    void deleteAllZoomedComps();

    void addZoomedComp(ZoomableShiftableComponent * zsComp, bool doZoom =true);

    void removeZoomedComp(ZoomableShiftableComponent * comp);

};
