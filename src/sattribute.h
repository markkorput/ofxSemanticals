//
//  Sattribute.h
//  ofxSemanticals
//
//  Created by Mark van de Korput on 2015-01-15.
//
//

#ifndef __ofxSemanticals__Sattribute__
#define __ofxSemanticals__Sattribute__

#include "ofMain.h"


//
//
// Class Definition
//
//


template <typename AttrType>
class Sattribute{

public: // functions

    Sattribute() : maxHistory(1){}

    AttrType get();
    AttrType& getByRef();
    void set(const AttrType &newValue, bool silent = false);
    AttrType prev(){ return valueHistory.back(); }
    void syncWith(Sattribute<AttrType> &otherAttribute);

    void setHistoryLength(unsigned int len){
        maxHistory = len;
        while(valueHistory.size() > maxHistory)
            valueHistory.pop_front();
    }

public: // events

    ofEvent<Sattribute> beforeChange;
    ofEvent<Sattribute> afterChange;
    ofEvent<Sattribute> beforeSet;
    ofEvent<Sattribute> afterSet;
    // ofEvent<Sattribute> getEvent; // not implemented for now

private: // callbacks

    void onSyncSourceChange(Sattribute<AttrType> &sourceAttribute);
    
private: // helper methods

    void addToHistory(const AttrType &oldValue);

private: // attributes

    AttrType value; // current value
    deque<AttrType> valueHistory; // list of previous values
    unsigned int maxHistory; // length of the history we should keep track of, default: 1

}; // class Sattribute


//
//
// Class Implementation
//
//


template <typename AttrType>
AttrType Sattribute<AttrType>::get(){
    // ofNotifyEvent(getEvent, *this, this);
    return value;
}

template <typename AttrType>
AttrType& Sattribute<AttrType>::getByRef(){
    // ofNotifyEvent(getEvent, *this, this);
    return value;
}

template <typename AttrType>
void Sattribute<AttrType>::set(const AttrType &newValue, bool silent){
    bool changing = (value != newValue);
    
    if(!silent) ofNotifyEvent(beforeSet, *this, this);
    if(!silent && changing) ofNotifyEvent(beforeChange, *this, this);
    
    // keep track of old value
    addToHistory(value);
    value = newValue;

    if(!silent && changing) ofNotifyEvent(afterChange, *this, this);
    if(!silent) ofNotifyEvent(afterSet, *this, this);
}

template <typename AttrType>
void Sattribute<AttrType>::syncWith(Sattribute<AttrType> &otherAttribute){
    ofAddListener(otherAttribute.afterChange, this, &Sattribute<AttrType>::onSyncSourceChange);
}

template <typename AttrType>
void Sattribute<AttrType>::onSyncSourceChange(Sattribute<AttrType> &sourceAttribute){
    // TODO; this will trigger our own change events, this should become optional
    set(sourceAttribute.value());
}

template <typename AttrType>
void Sattribute<AttrType>::addToHistory(const AttrType &oldValue){
    // special case optimization; no need to pop and push if the size is always 1
    if(maxHistory == 1 && valueHistory.size() == 1){
        valueHistory[0] = oldValue;
        return;
    }

    while(valueHistory.size() > 0 && valueHistory.size() >= maxHistory) valueHistory.pop_front();
    // note that if maxHistory == 0, we're still keeping track of one previous value, as prev() is
    // an integral part of the Sattribute functionality
    valueHistory.push_back(oldValue);
}

#endif // defined(__ofxSemanticals__Sattribute__)
