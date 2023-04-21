#pragma once
template<typename EventType>
class EventListener 
{
public:
    virtual ~EventListener() = default;
    virtual void OnEvent(const EventType& event) = 0;
};

