#include "Subject.h"
#include "Observer.h"
#include <memory>

using dae::Observer, dae::Subject;

void Subject::Notify(const void* pData, const std::string& event) const
{
    for (std::weak_ptr<Observer> wpObserver: m_wpObservers)
    {
        if (auto pObserver = wpObserver.lock())
        {
            pObserver->OnNotify(pData, event);
        }
    }
}

dae::Subject::~Subject()
{
    Notify(this, "RemovedObserver");
}

void Subject::AddObserver(std::weak_ptr<Observer> wpObserver)
{
    if (auto pObserver = wpObserver.lock())
    {
        pObserver->OnNotify(this, "AddedObserver");
        m_wpObservers.push_back(wpObserver);
    }
}

void Subject::RemoveObserver(std::weak_ptr<Observer>  wpObserver)
{
    auto it = std::find_if(m_wpObservers.begin(), m_wpObservers.end(),
        [&](const std::weak_ptr<Observer>& wpObs) 
        {
            return wpObs.owner_before(wpObserver) || wpObserver.owner_before(wpObs);
        });

    if (it != m_wpObservers.end())
    {
        if (auto pObserver = it->lock())
        {
            pObserver->OnNotify(this, "RemovedObserver");
        }

        m_wpObservers.erase(it);
    }
}
