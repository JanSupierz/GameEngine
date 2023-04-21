#pragma once
namespace dae
{
	template <class EventType>
	class Subject;

	template <class EventType>
	class Observer
	{
	public:
		virtual ~Observer() = default;
		virtual void OnNotify(const EventType& event) = 0;
		virtual void OnSubjectDestroy(Subject<EventType>* pSubject) = 0;
	};
}

